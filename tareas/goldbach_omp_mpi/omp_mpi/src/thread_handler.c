#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "thread_handler.h"
#include "goldbach_handler.h"

int64_t get_static_block_start(int64_t i, int64_t D, int64_t w);
int64_t get_static_block_min(int64_t i, int64_t mod);

void create_threads(input_t* input, int64_t thread_count, int argc, char* argv[]) {
    if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
        int process_number = -1;  // rank
        MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

        int process_count = -1;
        MPI_Comm_size(MPI_COMM_WORLD, &process_count);

        char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
        int hostname_length = -1;
        MPI_Get_processor_name(process_hostname, &hostname_length);

        MPI_Bcast(&input->size, 1, MPI_INT64_T, 0, MPI_COMM_WORLD);
        MPI_Bcast(&input->goldbach_numbers_index, 1, MPI_INT64_T, 0, MPI_COMM_WORLD);

        if (process_number != 0) {
            input->input_arr = (int64_t*) malloc(input->size * sizeof(int64_t));
            input->goldbach_numbers = (goldbach_number_t**) malloc(input->size * sizeof(goldbach_number_t*));

            if (input->input_arr == NULL || input->goldbach_numbers == NULL) {
                fprintf(stderr, "Error allocating memory on process %d\n", process_number);
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            } else {
                // Initialize memory to avoid garbage values
                memset(input->input_arr, 0, input->size * sizeof(int64_t));
                for (int i = 0; i < input->size; ++i) {
                    input->goldbach_numbers[i] = NULL;
                }
            }
        }

        MPI_Barrier(MPI_COMM_WORLD); // Synchronize before broadcasting array
        MPI_Bcast(input->input_arr, input->size, MPI_INT64_T, 0, MPI_COMM_WORLD);

        const int process_start = get_static_block_start(process_number, input->size, process_count);
        const int process_finish = get_static_block_start(process_number + 1, input->size, process_count);

        #pragma omp parallel num_threads(thread_count) \
            default(none) shared(input, process_number, process_start, process_finish, stderr)
        {
            #pragma omp for schedule(dynamic)
            for (int iteration = process_start; iteration < process_finish; ++iteration) {
                goldbach_number_t* my_gold_number = calc_goldbach_number(input->input_arr[iteration]);

                if (my_gold_number == NULL) {
                    fprintf(stderr, "Error calculating Goldbach number on process %d, iteration %d\n", process_number, iteration);
                    continue;
                }

                #pragma omp critical
                {
                    input->goldbach_numbers[iteration] = my_gold_number;
                    input->goldbach_numbers_index++;
                }
            }
        }

        MPI_Barrier(MPI_COMM_WORLD); // Synchronize before freeing memory

        if (process_number != 0) {
            free(input->input_arr);
            for (int i = 0; i < input->size; ++i) {
                if (input->goldbach_numbers[i] != NULL) {
                    free(input->goldbach_numbers[i]);
                }
            }
            free(input->goldbach_numbers);
        }

        MPI_Finalize();
    } else {
        printf("Error initializing MPI");
    }
}

int64_t get_static_block_start(int64_t i, int64_t D, int64_t w) {
    int64_t min = get_static_block_min(i, (D % w));
    return i * (D / w) + min;
}

int64_t get_static_block_min(int64_t i, int64_t mod) {
    if (i < mod) {
        return i;
    }
    if (i > mod) {
        return mod;
    }
    return i;
}
