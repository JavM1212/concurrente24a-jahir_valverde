/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_GREET_LEN 256

typedef struct shared_data_t {
  /// greets va a ser un arreglo de arreglos de caracters, es decir, cada 
  /// indice va a tener un arreglo de caracteres correspondiente al
  /// thread
  char** greets;
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
    (*shared_data).greets = (char**) calloc(thread_count, sizeof(char*));
    (*shared_data).thread_count = thread_count;

    if ((*shared_data).greets) {
      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = (finish_time.tv_sec - start_time.tv_sec)
                          + ((finish_time.tv_nsec - start_time.tv_nsec)
                            /(double)1000000000);

      fprintf(stdout, "elapsed_time: %.9fs\n", elapsed_time);

      free(shared_data->greets);
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
      shared_data->greets[thread_number] = (char*) 
        malloc(MAX_GREET_LEN * sizeof(char));
      if (shared_data->greets[thread_number]) {
        /// el inicio '\0' marca un caracter vacio por lo que no hay
        /// problema con el resto de basura del malloc
        *shared_data->greets[thread_number] = '\0';
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
        error = 22;
        break;
      }
    }

    fprintf(stdout, "Hello from main thread\n");

    for (int64_t thread_number = 0; thread_number < shared_data->thread_count
      ; thread_number++) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    for (int64_t thread_number = 0; thread_number < shared_data->thread_count
      ; thread_number++) {
      fprintf(stdout, "%s\n", shared_data->greets[thread_number]);
      free(shared_data->greets[thread_number]);
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

  /// se usa scan en vez de file para que guarde el resultado en un buffer
  sprintf(shared_data->greets[private_data->thread_number]
    , "Hello from secondary thread %" PRIu64 " of %" PRIu64
    , (*private_data).thread_number, shared_data->thread_count);

  return NULL;
}
