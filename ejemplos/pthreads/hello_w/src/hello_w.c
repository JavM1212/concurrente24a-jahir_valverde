/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>

void* greet(void* data);

/**
 * @brief APUNTES
 *  
 *        
 * 
 */


int main(int argc, char* argv[]) {
  /// forma alternativa de comentar utiliando el preprocesador
  #if 0
    for (int i = 0; i < argc; i++) {
      fprintf(stdout, "argv[%d] =  '%s'", i, argv[i]);
    }
    return 0;
  #endif

  int error = EXIT_SUCCESS;

  int64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  /// forma conveniente de pasar char* a long
  // assert(argc == 2); /// Error orientado a desarrollador
  if  (argc == 2) {  /// Error orientado a usuario
    if (!((sscanf(argv[1], "%" SCNu64, &thread_count)) == 1)) {
      fprintf(stderr, "Error: invalid thread count\n");
      return EXIT_FAILURE;
    }
  }

  /**
   * @brief este arreglo de hilos se esta creando en la pila. Por esto
   *        para numeros muy grandes se provoca un desbordamiento. El
   *        maximo de memoria para pila en sistemas unix es 8MB
   * 
   *        Para evitar esto es mejor reservar memoria dinamica 
   * 
   */
  // pthread_t threads[thread_count];
  pthread_t* threads = (pthread_t*) calloc(thread_count, sizeof(pthread_t));

  if (threads) {
    /**
     * @brief Como este programa es concurrente, presenta indeterminismo
     *        ya que no se puede determinar con certeza cual hilo va 
     *        primero que 
     * 
     */

    /// for para crear los threads
    for (int64_t thread_number = 0; thread_number < thread_count
      ; thread_number++) {
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
        , /*arg*/ (void*) thread_number);
    }

    fprintf(stdout, "Hello from main thread\n");

    /// for para joinearlos al ciclo principal
    for (int64_t thread_number = 0; thread_number < thread_count
      ; thread_number++) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(threads);
  
  } else {
    fprintf(stderr, "Usage: ./bin/hellow_w.c thread_count\n");
  }


  return error;
}

void* greet(void* data) {
  /// Se usa rank por convencion
  const int64_t rank = (int64_t) data;

  fprintf(stdout, "Hello from secondary thread %ld\n", rank);
  return NULL;
}
