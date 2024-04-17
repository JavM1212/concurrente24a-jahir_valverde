/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct shared_data_t {
  sem_t* can_greet;  /// tipo de semaforo
  int64_t thread_count;
} shared_data_t;

typedef struct private_data_t {
  int64_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

int64_t create_threads(shared_data_t* shared_data);
void* greet(void* data);

int main(int argc, char* argv[]) {
  int64_t error = EXIT_SUCCESS;

  int64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if  (argc == 2) {
    if (!((sscanf(argv[1], "%" SCNu64, &thread_count)) == 1)) {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = (shared_data_t*) calloc(1
    , sizeof(shared_data_t));
  if (shared_data) {
    /// se guarda memoria para el array de semaforos
    (*shared_data).can_greet = (sem_t*) calloc(thread_count, sizeof(sem_t));
    (*shared_data).thread_count = thread_count;

    for (int64_t thread_number = 0; thread_number < shared_data->thread_count
      ; thread_number++) {
      /// se usa sem_init para inicializar el semaforo
      error = sem_init(&shared_data->can_greet[thread_number], /*int pshared*/ 0
        , /*unsigned value*/ thread_number);  /// cambiar a que comiencen en 0
    }

  for (int64_t thread_number = 0; thread_number < shared_data->thread_count
    ; thread_number++) {
      sem_post(&shared_data->can_greet[thread_number]);
    }


    if ((*shared_data).can_greet) {  /// perguntamos si se pudo inicializar bien
      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = (finish_time.tv_sec - start_time.tv_sec)
                          + ((finish_time.tv_nsec - start_time.tv_nsec)
                            /(double)1000000000);

      fprintf(stdout, "elapsed_time: %.9fs\n", elapsed_time);

      free(shared_data->can_greet);
    } else {
      fprintf(stderr, "Error: could not allocate semaphores\n");
      error = 13;
    }
    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }

  fprintf(stdout, "thread count: %" PRIu64 "\n", thread_count);

  return error;
}

int64_t create_threads(shared_data_t* shared_data) {
  int64_t error = EXIT_SUCCESS;

  pthread_t* threads = (pthread_t*)
    calloc(shared_data->thread_count, sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));

  if (threads && private_data) {
    for (int64_t thread_number = 0; thread_number < shared_data->thread_count
      ; thread_number++) {
      if (error == EXIT_SUCCESS) {
        private_data[thread_number].thread_number = thread_number;
        private_data[thread_number].shared_data = shared_data;
        error = pthread_create(threads + thread_number, /*attr*/ NULL, greet
          , /*arg*/ private_data + thread_number);
        if (error == EXIT_SUCCESS) {
        } else {
          fprintf(stderr, "Error: could not create secondary thread\n");
          error = 21;
          break;
        }
      } else {
        fprintf(stderr, "Error: could not init semaphore\n");
        error = 21;
        break;
      }
    }

    fprintf(stdout, "Hello from main thread\n");

    // for (int64_t thread_number = 0; thread_number < shared_data->thread_count
    // ; thread_number++) {
    //   sem_post(&shared_data->can_greet[thread_number]);
    // }

    for (int64_t thread_number = 0; thread_number < shared_data->thread_count
      ; thread_number++) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
      /// liberar la memoria del semaforo
      sem_destroy(&shared_data->can_greet[thread_number]);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

void* greet(void* data) {
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  /// hacemos el wait
  sem_wait(&shared_data->can_greet[private_data->thread_number]);

  fprintf(stdout, "Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , (*private_data).thread_number, shared_data->thread_count);

  /// y el signal
  // const uint64_t next_thread =
  //   (private_data->thread_number + 1) % shared_data->thread_count;
  // sem_post(&shared_data->can_greet[next_thread]);

  return NULL;
}
