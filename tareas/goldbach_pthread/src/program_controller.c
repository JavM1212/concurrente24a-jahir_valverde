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

  /// inicializar los hilos que seran usados posteriormente
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

  /// crear el equipo de trabajo con los argumentos necesarios
  create_threads(input, goldbach_numbers, thread_count);

  /// hacerle merge al equipo
  join_threads(input, thread_count);

  /// imprimir el array de goldbach_number_t
  print_goldach_numbers(goldbach_numbers, input->size);

  /// liberar memoria que se necesito hasta el final
  free(input->threads);
  for (int64_t i = 0; i < input->size; i++) {
    free(goldbach_numbers[i]->sums);
    free(goldbach_numbers[i]);
  }
  free(goldbach_numbers);
  free(input->input_arr);
  free(input);
}

