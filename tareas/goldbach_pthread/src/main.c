// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include "program_controller.h"

/// llama a la funcion run (la cual tiene el flujo del programa) y calcula
/// el tiempo que dura en ejecutarla
int main(int argc, char* argv[]) {
  /// obtener la hora antes de ejecutar run
  // struct timespec start_time;
  // clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

  run(argc, argv);

  /// obtener la hora despues de ejecutar run
  // struct timespec finish_time;
  // clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

  /// tiempo transcurrido = hora final - hora inicial
  // double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
  //         (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        // printf("execution time: %.9lfs\n", elapsed);

  return errno;
}

