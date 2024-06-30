/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "omp.h"

#include "program_controller.h"
#include "input_handler.h"
#include "thread_handler.h"
#include "goldbach_handler.h"

/// controla el flujo del programa
void run(int argc, char* argv[]) {
  /// recibir input
  input_t* input = read_input();
  if (!input) {
    return;
  }

  /// recibir cantidad de hilos
  int64_t thread_count = analyze_arguments(argc, argv);

  /// crear array de goldbach_number_t* del tamano del input
  goldbach_number_t** goldbach_numbers = (goldbach_number_t**)
    calloc(input->size, sizeof(goldbach_number_t));
  if (!goldbach_numbers) {
    fprintf(stderr, "Error: Could not allocate goldbach_numbers in memory."
      " errno: %i\n", errno);
    return;
  }
  input->goldbach_numbers = goldbach_numbers;

  /// crear el equipo de trabajo con los argumentos necesarios (OMP)
  create_threads(input, thread_count, argc, argv);

  /// imprimir el array de goldbach_number_t
  print_goldach_numbers(input);

  /// liberar memoria que se necesito hasta el final
  for (int64_t i = 0; i < input->size; i++) {
    free(input->goldbach_numbers[i]->sums);
    free(input->goldbach_numbers[i]);
  }
  free(input->goldbach_numbers);
  free(input->input_arr);
  free(input);
}

