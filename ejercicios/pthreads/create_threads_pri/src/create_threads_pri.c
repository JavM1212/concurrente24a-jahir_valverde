// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// Simulates a producer and a consumer that share a bounded buffer

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

/// errores
enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_BUFFER_CAPACITY,
  ERR_ROUND_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};

/// parametros anadidos a memoria compartida
typedef struct {
  size_t thread_count;
  size_t buffer_capacity;
  double* buffer;
  size_t rounds;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;

  sem_t can_produce;
  sem_t can_consume;
} shared_data_t;

typedef struct  {
  size_t thread_number;
  shared_data_t* shared_data;
  pthread_t thread_uid;
} private_data_t;

int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
private_data_t** create_threads(shared_data_t* shared_data, size_t thread_count, void *(*start_routine)(void *));
int join_threads(size_t count, private_data_t** private_data_arr);

void* produce(void* data);
void* consume(void* data);
useconds_t random_between(useconds_t min, useconds_t max);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    /// se analiza si los argumentos son validos
    error = analyze_arguments(argc, argv, shared_data);
    if (error == EXIT_SUCCESS) {
      shared_data->buffer = (double*)
        calloc(shared_data->buffer_capacity, sizeof(double));
      if (shared_data->buffer) {
        /// inicializamos semaforos
        sem_init(&shared_data->can_produce, /*int pshared*/ 0
          , shared_data->buffer_capacity);
        sem_init(&shared_data->can_consume, /*int pshared*/ 0
        , /*int value*/ 0);

        unsigned int seed = 0u;
        getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
        srandom(seed);

        struct timespec start_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

        private_data_t** producer_private_arr = create_threads(shared_data, shared_data->buffer_capacity, produce);
        // private_data_t** consumer_private_arr = create_threads(shared_data, shared_data->rounds, consume);
          
        struct timespec finish_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

        double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed);

        join_threads(shared_data->buffer_capacity, producer_private_arr);
        // join_threads(shared_data->rounds, consumer_private_arr);

        sem_destroy(&shared_data->can_consume);
        sem_destroy(&shared_data->can_produce);
        free(shared_data->buffer);
      } else {
        fprintf(stderr, "error: could not create buffer\n");
        error = ERR_NOMEM_BUFFER;
      }
    }

    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = ERR_NOMEM_SHARED;
  }

  return error;
}

int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  if (argc == 7) {
    if (sscanf(argv[1], "%zu", &shared_data->buffer_capacity) != 1
      || shared_data->buffer_capacity == 0) {
        fprintf(stderr, "error: invalid buffer capacity\n");
        error = ERR_BUFFER_CAPACITY;
    } else if (sscanf(argv[2], "%zu", &shared_data->rounds) != 1
      || shared_data->rounds == 0) {
        fprintf(stderr, "error: invalid round count\n");
        error = ERR_ROUND_COUNT;
    } else if (sscanf(argv[3], "%u", &shared_data->producer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min producer delay\n");
        error = ERR_MIN_PROD_DELAY;
    } else if (sscanf(argv[4], "%u", &shared_data->producer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max producer delay\n");
        error = ERR_MAX_PROD_DELAY;
    } else if (sscanf(argv[5], "%u", &shared_data->consumer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min consumer delay\n");
        error = ERR_MIN_CONS_DELAY;
    } else if (sscanf(argv[6], "%u", &shared_data->consumer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max consumer delay\n");
        error = ERR_MAX_CONS_DELAY;
    }
  } else {
    fprintf(stderr, "usage: prod_cons_bound buffer_capacity rounds"
      " producer_min_delay producer_max_delay"
      " consumer_min_delay consumer_max_delay\n");
      error = ERR_NO_ARGS;
  }
  return error;
}

private_data_t** create_threads(shared_data_t* shared_data, size_t thread_count, void *(*start_routine)(void *)) {
  /// creamos el arreglo de punteros a private data
  assert(shared_data);
  private_data_t** private_data_arr = (private_data_t**) calloc(thread_count, sizeof(private_data_t*));
  for (size_t i = 0; i < thread_count; i++) {
    private_data_arr[i] = (private_data_t*) calloc(thread_count, sizeof(private_data_t));
    if (!private_data_arr[i]) {
      fprintf(stderr, "error: could not create private_data_arr[%ld]\n", i);
      return NULL;
    }
  }
  if (!private_data_arr) {
    fprintf(stderr, "error: could not create private_data_arr\n");
    return NULL;
  }

  /// asignamos los valores a private date
  int error = EXIT_SUCCESS;
  for (size_t i = 0; i < thread_count; i++) {
    private_data_arr[i]->thread_number = i;
    private_data_arr[i]->shared_data = shared_data;
    /// le decimos que ejecute la funcion pasada por parametro
    /// y se le envia la dirección del registro privado a cada hilo al crearlo 
    error = pthread_create(&private_data_arr[i]->thread_uid, /*attr*/ NULL
      , start_routine, private_data_arr[i]);
    if (!error) {
      /// descartamos los que ya habiamos logrado crear
      for (size_t j = 0; j < i; j++) { 
        pthread_join(private_data_arr[i]->thread_uid, /*value_ptr*/ NULL);
      }
      /// damos mensaje de error y retornamos
      fprintf(stderr, "error: could not create pthread[%ld]\n", i);
      return NULL;
    }
  }

  return private_data_arr;
}

int join_threads(size_t count, private_data_t** private_data_arr) {
  int error = EXIT_SUCCESS;
  for (size_t index = 0; index < count; ++index) {
    // todo: sum could not be right
    error += pthread_join(private_data_arr[index]->thread_uid, /*value_ptr*/ NULL);
  }
  if (!error) {
    fprintf(stderr, "error: could not join threads\n");
  }
  for (size_t i = 0; i < count; i++) {
    free(private_data_arr[i]);
  }
  free(private_data_arr);
  return error;
}

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data;
  size_t count = 0;
  for (size_t round = 0; round < shared_data->rounds; ++round) {
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) {
      /// hacemos el wait a produce
      sem_wait(&shared_data->can_produce);

      /// milisegundos
      usleep(1000 * random_between(shared_data->producer_min_delay
        , shared_data->producer_max_delay));
      shared_data->buffer[index] = ++count;
      printf("Produced %lg\n", shared_data->buffer[index]);

      /// y el signal a consume
      sem_post(&shared_data->can_consume);
    }
  }

  return NULL;
}

void* consume(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data;
  for (size_t round = 0; round < shared_data->rounds; ++round) {
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) {
      sem_wait(&shared_data->can_consume);

      double value = shared_data->buffer[index];
      usleep(1000 * random_between(shared_data->consumer_min_delay
        , shared_data->consumer_max_delay));
      printf("\tConsumed %lg\n", value);

      sem_post(&shared_data->can_produce);
    }
  }

  return NULL;
}

useconds_t random_between(useconds_t min, useconds_t max) {
  return min + (max > min ? (random() % (max - min)) : 0);
}
