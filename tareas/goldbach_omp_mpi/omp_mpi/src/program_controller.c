/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <mpi.h>

#include "program_controller.h"
#include "input_handler.h"
#include "thread_handler.h"
#include "goldbach_handler.h"

void print_goldbach_numbers_MPI(int process_number, int process_count
  , int process_start, int process_finish, input_t *input);
int64_t get_static_block_start(int64_t i, int64_t D, int64_t w);
int64_t get_static_block_min(int64_t i, int64_t mod);

/// controla el flujo del programa
void run(int argc, char *argv[]) {
  /// recibir input
  input_t *input = read_input();
  if (!input) {
    return;
  }

  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;  // rank
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME] = {'\0'};
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);

    MPI_Bcast(&input->size, 1, MPI_INT64_T, 0, MPI_COMM_WORLD);
    // Broadcast the input array
    MPI_Bcast(input->input_arr, input->size, MPI_INT64_T, 0, MPI_COMM_WORLD);
    MPI_Bcast(&input->goldbach_numbers_index, 1, MPI_INT64_T, 0
    , MPI_COMM_WORLD);

    // greet(process_number, process_count, process_hostname);

    /// recibir cantidad de hilos
    int64_t thread_count = analyze_arguments(argc, argv);

    MPI_Bcast(&thread_count, 1, MPI_INT64_T, 0, MPI_COMM_WORLD);

    const int process_start = get_static_block_start(process_number
    , input->size, process_count);
    const int process_finish = get_static_block_start(process_number + 1
    , input->size, process_count);
    const int process_size = process_finish - process_start;

    // crear array de goldbach_number_t* del tamano del input
    goldbach_number_t **goldbach_numbers = (goldbach_number_t **)
        calloc(process_size, sizeof(goldbach_number_t));
    if (!goldbach_numbers) {
      fprintf(stderr, "Error: Could not allocate goldbach_numbers in memory."
                      " errno: %i\n",
              errno);
      return;
    }
    input->goldbach_numbers = goldbach_numbers;

    /// crear el equipo de trabajo con los argumentos necesarios (OMP)
    create_threads(input, thread_count, process_start, process_finish);

    /// imprimir el array de goldbach_number_t
    // print_goldach_numbers(input);
    print_goldbach_numbers_MPI(process_number, process_count, process_start
    , process_finish, input);

    MPI_Finalize();
  } else {
    printf("Error initializing MPI");
  }
}

void print_goldbach_numbers_MPI(int process_number, int process_count
, int process_start, int process_finish, input_t *input) {
  const int previous_process
    = (process_count + process_number - 1) % process_count;
  const int next_process = (process_number + 1) % process_count;
  bool can_print = true;

  if (process_number > 0) {
    // receive(&can_print, 1, previous_process)
    if (MPI_Recv(&can_print, /*count*/ 1, MPI_C_BOOL, previous_process
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      printf("could not receive message");
    }
  }

  print_goldach_numbers(input, process_start, process_finish);

  // send(can_print, 1, next_process);
  if (MPI_Send(&can_print, /*count*/ 1, MPI_C_BOOL, next_process, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
    printf("could not send message");
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
