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
  sem_t can_hit;
  int64_t max_hits;  // cantidad maxima de golpes a la piñata
  int64_t hits_left;  // cantidad actual de golpes
  int64_t thread_count;
} shared_data_t;

typedef struct private_data_t {
  int64_t thread_number;
  int64_t my_hits;
  shared_data_t *shared_data;
} private_data_t;

int64_t create_threads(shared_data_t *shared_data);
void *hit_pinata(void *data);

int main(int argc, char *argv[]) {
  int64_t error = EXIT_SUCCESS;

  int64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  int64_t max_hits = 10;  // default
  if (argc == 3) {
    if (!((sscanf(argv[1], "%" SCNu64, &thread_count)) == 1)) {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
    if (!((sscanf(argv[2], "%" SCNu64, &max_hits)) == 1)) {
      fprintf(stderr, "Error: invalid max hits\n");
      return 11;
    }
  }

  shared_data_t *shared_data =
      (shared_data_t *)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    sem_init(&shared_data->can_hit, 0, 1);  // Inicializa el semáforo
    shared_data->max_hits = max_hits;
    shared_data->hits_left = max_hits;
    shared_data->thread_count = thread_count;

    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    error = create_threads(shared_data);

    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) +
                          ((finish_time.tv_nsec - start_time.tv_nsec) /
                           (double)1000000000);

    fprintf(stdout, "elapsed_time: %.9fs\n", elapsed_time);
    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }

  return error;
}

int64_t create_threads(shared_data_t *shared_data) {
  int64_t error = EXIT_SUCCESS;

  pthread_t *threads =
      (pthread_t *)calloc(shared_data->thread_count, sizeof(pthread_t));
  private_data_t *private_data =
      (private_data_t *)calloc(shared_data->thread_count, sizeof(private_data_t));

  if (threads && private_data) {
    for (int64_t thread_number = 0; thread_number < shared_data->thread_count;
         thread_number++) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].my_hits = 0;
      private_data[thread_number].shared_data = shared_data;
      error = pthread_create(threads + thread_number, NULL, hit_pinata,
                             private_data + thread_number);
      if (error != EXIT_SUCCESS) {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    for (int64_t thread_number = 0; thread_number < shared_data->thread_count;
         thread_number++) {
      pthread_join(threads[thread_number], NULL);
    }
    sem_destroy(&shared_data->can_hit);

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n",
            shared_data->thread_count);
    error = 22;
  }

  return error;
}

void *hit_pinata(void *data) {
  private_data_t *private_data = (private_data_t *)data;
  shared_data_t *shared_data = private_data->shared_data;

  while (1) {
    sem_wait(&shared_data->can_hit);  // Espera a que el semáforo esté disponible
    if (shared_data->hits_left > 0) {
      shared_data->hits_left--;
      private_data->my_hits++;
      sem_post(&shared_data->can_hit);  // Libera el semáforo antes de salir del bucle
      if (shared_data->hits_left == 0) {
        printf("Thread %ld/%ld: %ld hits, I broke the pinata\n",
               private_data->thread_number, shared_data->max_hits,
               private_data->my_hits);
        return NULL;
      }
    } else {
      sem_post(&shared_data->can_hit);  // Libera el semáforo antes de salir del bucle
      break;
    }
  }
  printf("Thread %ld/%ld: %ld hits\n", private_data->thread_number,
         shared_data->max_hits, private_data->my_hits);

  return NULL;
}