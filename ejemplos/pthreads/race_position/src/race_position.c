/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/// se agrega la posicion a la data compartida
typedef struct shared_data_t {
  int64_t position;
  int64_t thread_count;
  pthread_mutex_t can_access_position;
} shared_data_t;

typedef struct private_data_t {
  int64_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

int64_t create_threads(shared_data_t* shared_data);
void* race(void* data);

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
    (*shared_data).position = 0;
    (*shared_data).thread_count = thread_count;
    /// se inicializa el mutex
    error = pthread_mutex_init(&(*shared_data).can_access_position
      , /**attr*/ NULL);

    if (error == EXIT_SUCCESS) {
      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = (finish_time.tv_sec - start_time.tv_sec)
                          + ((finish_time.tv_nsec - start_time.tv_nsec)
                            /(double)1000000000);

      fprintf(stdout, "\nExecution time: %.9fs\n", elapsed_time);

      pthread_mutex_destroy(&shared_data->can_access_position);
      free(shared_data);
    } else {
      fprintf(stderr, "Error: could not init mutex\n");
    }
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }

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
      error = pthread_create(threads + thread_number, /*attr*/ NULL, race
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

void* race(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  /// se incrementa primero la posicion para que comience desde 1
  /// como esta linea genera una condicion de carrera, se va a a matar
  /// la concurrencia. Esto se va a hacer con mutex, exclusion mutua
  /// Mutex asegura exclusion mutua, pero no orden
  pthread_mutex_lock(&shared_data->can_access_position); 
  uint64_t my_position = ++shared_data->position;  /// preincremento

  /// nuevo string a imprimir
  fprintf(stdout, "Thread %" PRIu64 "/%" PRIu64 ": I arrived at"
    " position %" PRIu64 "\n"
    , (*private_data).thread_number, shared_data->thread_count, my_position);

  pthread_mutex_unlock(&shared_data->can_access_position);
  /// el problema de esta implementacion es que practicamente esta serializado
  /// porque la mayor parte del cuerpo de la subrutina esta encerrado
  /// por mutex. 
  /// Mientras el trabajo pesado este fuera de un mutex no deberia ser 
  /// una mala practica

  /**
   * se ve tentador sacar el print y usar directamente la memoria compartida, 
   * el problema es que en la linea 123 se esta modificando la memoria 
   * compartida, y de manera concurrente se haria lectura a la misma memoria
   * compartida, lo que cumple la definicion de condicion de carrera 
   * porque no es necesariamente modificacion con modificacion sino
   * que puede ser modificacion con lectura como en este caso
   * Por eso es mas conveniente usar copias
   */
  // fprintf(stdout, "Thread %" PRIu64 "/%" PRIu64 ": I arrived at"
    // " position %" PRIu64 "\n"
    // , (*private_data).thread_number, shared_data->thread_count
    // , shared_data->position);

  /**
   * tambien se pueden usar varios mutex lock y unlock con el mismo 
   * o diferentes mutex. Esto asegura no condicion de carrera pero no
   * asegura orden.
   */

  /**
   * sino se utiliza unlock se va a quedar esperando infinitamente
   */

  return NULL;
}
