/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct shared_data_t {
  int64_t next_thread;  /// settear next_thread en la memoria compartida
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
  if  (argc == 2) {  /// Error orientado a usuario
    if (!((sscanf(argv[1], "%" SCNu64, &thread_count)) == 1)) {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = (shared_data_t*) calloc(1
    , sizeof(shared_data_t));
  if (shared_data) {
    /// a pesar de que calloc ya settea en 0, se inicializa por legibilidad
    (*shared_data).next_thread = 0;
    (*shared_data).thread_count = thread_count;

    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    error = create_threads(shared_data);

    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec)
                        + ((finish_time.tv_nsec - start_time.tv_nsec)
                          /(double)1000000000);

    // double t := s2 - s1 + (n2 - n1) / 1000000000

    fprintf(stdout, "elapsed_time: %.9fs\n", elapsed_time);

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
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      error = pthread_create(threads + thread_number, /*attr*/ NULL, greet
        , /*arg*/ private_data + thread_number);
      if (error != EXIT_SUCCESS) {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    fprintf(stdout, "Hello from main thread\n");

    for (int64_t thread_number = 0; thread_number < shared_data->thread_count
      ; thread_number++) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
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

  /// desde cierto numero de hilos la espera activa puede ser peligrosa
  /// porque a veces un evento de mayor prioridad interrumpe un hilo
  /// y pasa su subrutina a la cola de espera. Esto inpide que se pueda
  /// incrementar el next_thread y genera que se gaste el 100% de los 
  /// recursos de los otros cpu preguntando si el next_thread es menor
  /// que el thread_number y siempre va a dar true porque no se pudo incrementar
  while (shared_data->next_thread < private_data->thread_number) {
    /// busy wait
  }

  fprintf(stdout, "Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , (*private_data).thread_number, shared_data->thread_count);

  /// se le avisa al proximo hilo que ya puede continuar
  shared_data->next_thread++;

  return NULL;
}
