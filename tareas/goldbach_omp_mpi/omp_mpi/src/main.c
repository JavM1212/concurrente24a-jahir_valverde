// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include "program_controller.h"

/// llama a la funcion run (la cual tiene el flujo del programa) y calcula
/// el tiempo que dura en ejecutarla
int main(int argc, char* argv[]) {
  run(argc, argv);
  return 0;
}

