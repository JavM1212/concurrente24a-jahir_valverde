/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "thread_handler.h"
#include "goldbach_handler.h"

/// crear equipo de hilos
void create_threads(input_t* input, int64_t thread_count) {
  // for (int thread = 0; thread < thread_count; thread++) {
    /// crear el hilo pasando por parametro el my_work correspondiente
    // errno = pthread_create(&input->threads[thread], /*attr*/ NULL, consume
    //   , /*arg*/ input);
  // }

  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(input) {
    // mapeo dinamico
    #pragma omp for schedule(dynamic)
    for (int iteration = 0; iteration < input->size; ++iteration) {
      goldbach_number_t* my_gold_number
        = calc_goldbach_number(input->input_arr[iteration]);

      input->goldbach_numbers[iteration] = my_gold_number;
      input->goldbach_numbers_index++;
    }
  }
}
