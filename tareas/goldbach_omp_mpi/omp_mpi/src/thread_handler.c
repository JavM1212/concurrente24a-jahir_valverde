/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "thread_handler.h"
#include "goldbach_handler.h"

void create_threads(input_t* input, int64_t thread_count, int process_start
  , int process_finish) {
    input->goldbach_numbers_index = 0;

    #pragma omp parallel num_threads(thread_count) \
        default(none) shared(input, process_start \
          , process_finish, stderr) 
          {
        #pragma omp for schedule(dynamic)
        for (int iteration = process_start; iteration < process_finish
          ; ++iteration) {
            goldbach_number_t* my_gold_number = calc_goldbach_number(
                input->input_arr[iteration]);

            #pragma omp critical
            {
                input->goldbach_numbers[iteration - process_start]
                  = my_gold_number;
                input->goldbach_numbers_index++;
            }
          }
    }
}
