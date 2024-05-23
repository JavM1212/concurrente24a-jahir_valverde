// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <iostream>

int main() {
  #pragma omp parallel
  // bloque paralelo, esto es manejado por omp y hace lo mismo que hemos hecho
  // en el curso pero de manera declarativa
  {
    #pragma omp critical
    std::cout << "Hello from secondary thread" << std::endl;
  }
}
