/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "thread_handler.h"
#include "goldbach_handler.h"

/// crear equipo de hilos
void create_threads(input_t* input, int64_t thread_count) {
  for (int thread = 0; thread < thread_count; thread++) {
    /// crear el hilo pasando por parametro el my_work correspondiente
    errno = pthread_create(&input->threads[thread], /*attr*/ NULL, consume
      , /*arg*/ input);
  }
}

/// funcion corrida por hilos, calcula los goldbach_number para cada unidad
/// del subarreglo memoria compartida
void* run_thread(void* data) {
  my_work_t* my_work = (my_work_t*) data;

  /// como el subarreglo de memoria compartida ya le fue calculado su offset
  /// solo hace falta recorrer hasta el assigned_work_units calculado
  /// anteriormente
  for (int i = 0; i < my_work->assigned_work_units; i++) {
    my_work->my_goldbach_numbers[i] =
      calc_goldbach_number(my_work->my_input[i]);
  }

  return NULL;
}

/// hacerle merge al equipo de trabajo
void join_threads(input_t* input, int64_t thread_count) {
  for (int thread = 0; thread < thread_count; thread++) {
    pthread_join(input->threads[thread], /*value_ptr*/ NULL);
  }
}

/// obtener el indice de inicio que le corresponde a cada hilo
int64_t get_static_block_start(int64_t i, int64_t D, int64_t w) {
  int64_t min = get_static_block_min(i, (D % w));
  return i * (D / w) + min;
}

/// obtener el menor de dos numeros
int64_t get_static_block_min(int64_t i, int64_t mod) {
  if (i < mod) {
    return i;
  }
  if (i > mod) {
    return mod;
  }
  // i == mod
  return i;
}
