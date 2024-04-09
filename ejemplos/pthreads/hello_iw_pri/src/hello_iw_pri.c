/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>

/// Aqui se va a guardar la informacion privada de cada threat
/**
 * @brief Los structs son registros
 *        Hay 3 tipos de registros
 *        1. Estructura: Solo aloja variables
 *        2. Union: Solo aloja una a la vez
 *        3. Clase: Azucar sintactico que funciona la estructura y la subrutina
 * 
 */
typedef struct private_data_t {
  int64_t thread_number;  /// rank
  int64_t thread_count;
} private_data_t;

void* race(void* data);
int64_t create_threads(int64_t thread_count);

int main(int argc, char* argv[]) {
  int64_t error = EXIT_SUCCESS;

  int64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if  (argc == 2) {  /// Error orientado a usuario
    if (!((sscanf(argv[1], "%" SCNu64, &thread_count)) == 1)) {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  fprintf(stdout, "thread count: %" PRIu64 "\n", thread_count);

  error = create_threads(thread_count);

  return error;
}

int64_t create_threads(int64_t thread_count) {
  int64_t error = EXIT_SUCCESS;

  pthread_t* threads = (pthread_t*) calloc(thread_count, sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*) calloc(thread_count
    , sizeof(private_data_t));

  if (threads && private_data) {
    for (int64_t thread_number = 0; thread_number < thread_count
      ; thread_number++) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].thread_count = thread_count;

      /// la aritmetica de punteros tambien es valida
      error = pthread_create(threads + thread_number, /*attr*/ NULL, race
        , /*arg*/ private_data + thread_number);
      if (error != EXIT_SUCCESS) {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    fprintf(stdout, "Hello from main thread\n");

    for (int64_t thread_number = 0; thread_number < thread_count
      ; thread_number++) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , thread_count);
    error = 22;
  }

  return error;
}

void* race(void* data) {
  private_data_t* private_data = (private_data_t*) data;

  /**
   * @brief dos formas diferentes de acceder a la info de private data. 
   *        la forma "manual" y un azucar sintactico
   * 
   */
  fprintf(stdout, "Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , (*private_data).thread_number, private_data->thread_count);
  return NULL;
}
