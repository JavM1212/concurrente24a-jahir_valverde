// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

/**
 * @brief Leer documentacion del header
 */

#include "grandma_lottery.h"


int main(void) {
  printf("Hola, soy la abuela!\n");

  // Inicializo hilos
  pthread_t nieto1;
  pthread_t nieto2;

  // Inicializo pedacitos
  int* pedacito1 = (int*) calloc(1, sizeof(int));
  int* pedacito2 = (int*) calloc(1, sizeof(int));

  /**
   * @brief En la funcion gen_num, se crea una seed que depende del tiempo.
   *        Como los 2 hilos corren concurrentemente se ejecutan al mismo
   *        tiempo. Por lo que se genera la misma seed. Por ende, estaba
   *        generando el mismo numero 2 veces.
   * 
   *        Para evitar esto paso de argumento los pedacitos con un dummy value
   *        y se los sumo a la seed. De esta manera obtengo una seed diferente.
   *        De paso aprovecho esas direcciones para devolver el valor generado.
   * 
   */
  *pedacito1 = 0;
  *pedacito2 = 1;

  // Creo hilos
  int error_nieto1 = pthread_create(&nieto1, /*attr*/ NULL
    , gen_num_v1, (void*) pedacito1);  /**@var1*/
  int error_nieto2 = pthread_create(&nieto2, /*attr*/ NULL
    , gen_num_v1, (void*) pedacito2);  /**@var1*/
  // int error_nieto1 = pthread_create(&nieto1, /*attr*/ NULL
  //   , gen_num_v2, (void*) pedacito1);  /**@var2*/
  // int error_nieto2 = pthread_create(&nieto2, /*attr*/ NULL
  //   , gen_num_v2, (void*) pedacito2);  /**@var2*/

  // Joineo hilos
  if (error_nieto1 == EXIT_SUCCESS) {
    pthread_join(nieto1, (void**) &pedacito1);
  }
  if (error_nieto2 == EXIT_SUCCESS) {
    pthread_join(nieto2, (void**) &pedacito2);
  }

  // Imprimo valores
  printf("El nieto 1 me dio el pedazo numero %d\n", *pedacito1);  /**@var1*/
  printf("El nieto 2 me dio el pedazo numero %d\n", *pedacito2);  /**@var1*/
  // printf("%d\n", pedacito1);  /**@var2*/
  // printf("%d\n", pedacito2);  /**@var2*/

  // Libero memoria
  free(pedacito1);
  free(pedacito2);

  // Success: 1 and 1. Failure: (1 and 0) or (0 and 1) or (0 and 0)
  return error_nieto1 & error_nieto2;
}

// variante 1
void* gen_num_v1(void* pedacito) {
  // create seed
  unsigned int seed = time(NULL);
  seed += *(int*) pedacito;
  *(int*) pedacito = 0;

  int random_number = rand_r(&seed) % 100;  // [0, 99]

  *(int*) pedacito = random_number;  // *memoria = numero
  return pedacito;
}

// variante 2
void* gen_num_v2(void* pedacito) {
  // create seeed
  unsigned int seed = time(NULL);
  seed += *(int*) pedacito;
  *(int*) pedacito = 0;

  int random_number = rand_r(&seed) % 100;

  pedacito = (void*)(size_t) random_number;  // memoria = numero
  return pedacito;
}
