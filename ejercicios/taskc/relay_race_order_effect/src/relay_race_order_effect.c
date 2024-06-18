// ¿Afecta el orden de creación de los equipos el resultado de la carrera de relevos? 
// Respuesta: si, la impresion se ve alterada al hacer la ejecucion normal y la inversa

#define _DEFAULT_SOURCE

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct shared_data {
    size_t team_count;
    useconds_t stage1_duration;
    useconds_t stage2_duration;
    size_t position;
    pthread_barrier_t start_barrier;
    sem_t* batons;
    pthread_mutex_t finish_mutex;
} shared_data_t;

typedef struct private_data {
    size_t thread_number;  // rank
    shared_data_t* shared_data;
} private_data_t;

int create_threads(shared_data_t* shared_data);
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
void* start_race(void* data);
void* finish_race(void* data);

int main(int argc, char* argv[]) {
    int error = EXIT_SUCCESS;

    shared_data_t* shared_data = (shared_data_t*)
        calloc(1, sizeof(shared_data_t));

    if (shared_data) {
        error = analyze_arguments(argc, argv, shared_data);
        if (error == EXIT_SUCCESS) {
            shared_data->position = 0;
            error = pthread_barrier_init(&shared_data->start_barrier,
                /*attr*/ NULL, /*count*/ shared_data->team_count);
            shared_data->batons = (sem_t*) calloc(shared_data->team_count
                , sizeof(sem_t));
            error += pthread_mutex_init(&shared_data->finish_mutex, /*attr*/ NULL);

            if (error == EXIT_SUCCESS && shared_data->batons) {
                for (size_t index = 0; index < shared_data->team_count; ++index) {
                    sem_init(&shared_data->batons[index], /*pshared*/ 0, /*value*/ 0);
                }

                struct timespec start_time, finish_time;
                clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

                error = create_threads(shared_data);

                clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);
                double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
                    (finish_time.tv_nsec - start_time.tv_nsec) / 1e9;
                printf("Elapsed time: %.9lfs\n", elapsed_time);
            }

            for (size_t index = 0; index < shared_data->team_count; ++index) {
                sem_destroy(&shared_data->batons[index]);
            }
            free(shared_data->batons);
            pthread_barrier_destroy(&shared_data->start_barrier);
            pthread_mutex_destroy(&shared_data->finish_mutex);
        }
        free(shared_data);
    }

    return error;
}

int create_threads(shared_data_t* shared_data) {
    int error = EXIT_SUCCESS;

    pthread_t* threads = (pthread_t*)
        calloc(shared_data->team_count * 2, sizeof(pthread_t));
    private_data_t* private_data = (private_data_t*)
        calloc(shared_data->team_count * 2, sizeof(private_data_t));

    if (threads && private_data) {
        for (size_t index = 0; index < shared_data->team_count * 2 && error == EXIT_SUCCESS; ++index) {
            private_data[index].thread_number = index;
            private_data[index].shared_data = shared_data;
            if (index < shared_data->team_count) {
                error = pthread_create(&threads[index], /*attr*/ NULL, start_race, &private_data[index]);
            } else {
                error = pthread_create(&threads[index], /*attr*/ NULL, finish_race, &private_data[index]);
            }
        }

        for (size_t index = 0; index < shared_data->team_count * 2; ++index) {
            pthread_join(threads[index], /*value_ptr*/ NULL);
        }
    } else {
        error = EXIT_FAILURE;
    }

    free(threads);
    free(private_data);

    return error;
}

void* start_race(void* data) {
    private_data_t* private_data = (private_data_t*)data;
    shared_data_t* shared_data = private_data->shared_data;

    const size_t team_number = private_data->thread_number;
    assert(team_number < shared_data->team_count);

    pthread_barrier_wait(&shared_data->start_barrier);
    usleep(1000 * shared_data->stage1_duration);
    sem_post(&shared_data->batons[team_number]);

    return NULL;
}

void* finish_race(void* data) {
    private_data_t* private_data = (private_data_t*)data;
    shared_data_t* shared_data = private_data->shared_data;

    const size_t rank = private_data->thread_number;
    const size_t team_number = rank - shared_data->team_count;
    assert(team_number < shared_data->team_count);

    sem_wait(&shared_data->batons[team_number]);
    usleep(1000 * shared_data->stage2_duration);

    pthread_mutex_lock(&shared_data->finish_mutex);
    const size_t our_position = ++shared_data->position;
    if (our_position <= 3) {
        printf("Place %zu: team %zu\n", our_position, team_number);
    }
    pthread_mutex_unlock(&shared_data->finish_mutex);

    return NULL;
}

int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data) {
    int error = EXIT_SUCCESS;

    if (argc == 4) {
        shared_data->team_count = strtoull(argv[1], /*endptr*/ NULL, /*base*/ 10);
        shared_data->stage1_duration = strtoull(argv[2], /*endptr*/ NULL, /*base*/ 10);
        shared_data->stage2_duration = strtoull(argv[3], /*endptr*/ NULL, /*base*/ 10);
    } else {
        fprintf(stderr, "usage: %s team_count stage1_duration stage2_duration\n", argv[0]);
        error = EXIT_FAILURE;
    }

    return error;
}
