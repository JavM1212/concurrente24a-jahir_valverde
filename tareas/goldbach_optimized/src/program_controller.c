/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

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

  /// inicializar los hilos que seran usados posteriormente como consumidores
  input->threads = (pthread_t*) calloc(thread_count, sizeof(pthread_t));
  if (!input->threads) {
    fprintf(stderr, "Error: Could not allocate threads in memory."
      " errno: %i\n", errno);
    return;
  }

  /// crear array de goldbach_number_t* del tamano del input
  goldbach_number_t** goldbach_numbers = (goldbach_number_t**)
    calloc(input->size, sizeof(goldbach_number_t));
  if (!goldbach_numbers) {
    fprintf(stderr, "Error: Could not allocate goldbach_numbers in memory."
      " errno: %i\n", errno);
    return;
  }
  input->goldbach_numbers = goldbach_numbers;

  pthread_t* producer_thread = (pthread_t*) calloc(1
    , sizeof(pthread_t));
  errno = !pthread_create(producer_thread, /*attr*/ NULL, produce
    , /*arg*/ input);
  if (!errno) {
      fprintf(stderr, "Error: Could not allocate producer thread in memory."
      " errno: %i\n", errno);
      return;
  }

  /// crear el equipo de trabajo con los argumentos necesarios
  create_threads(input, thread_count);

  /// hacerle merge al equipo
  join_threads(input, thread_count);

  pthread_join(*producer_thread, /*value_ptr*/ NULL);
  free(producer_thread);

  /// imprimir el array de goldbach_number_t
  print_goldach_numbers(input, input->size);

  /// liberar memoria que se necesito hasta el final
  free(input->threads);
  for (int64_t i = 0; i < input->size; i++) {
    free(input->goldbach_numbers[i]->sums);
    free(input->goldbach_numbers[i]);
  }
  free(input->goldbach_numbers);
  free(input->input_arr);
  free(input);
}

