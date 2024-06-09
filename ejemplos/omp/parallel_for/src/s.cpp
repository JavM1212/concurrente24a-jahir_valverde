// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>

// El mapeo por defecto de OMP es por bloque

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  // va a hacer una reparticion por bloque a los hilos
  #pragma omp parallel for num_threads(thread_count) \
    default(none) shared(iteration_count, std::cout)
  // el for es obligatorio y debe ser un ciclo por contador, es decir, el
  // compilador va a saber cuantas iteraciones se van a realizar
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    #pragma omp critical(stdout)
    std::cout << omp_get_thread_num() << '/' << omp_get_num_threads()
      << ": iteration " << iteration << '/' << iteration_count << std::endl;
  }
}
