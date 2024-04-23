// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#ifndef LIBRARIES
#define LIBRARIES

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief input_arr es el array de numeros introducidos por el usuario
 *        size es el tamano del array
 *        threads son los hilos que van a dividirse el trabajo, se guarda en 
 *        input porque desde el input se decide cuantos threads van a haber
 * 
 *        va a ser utilizado como memoria compartida
 * 
 */
typedef struct input {
  int64_t* input_arr;
  int64_t size;
  pthread_t* threads;
} input_t;

/**
 * @brief number es el numero al que se le dean calcular las sumas
 *        amount_of_sums es la cantidad de sumas encontradas
 *        sums es el array a las sumas
 * 
 *        va a ser utilizado como memoria compartida
 */
typedef struct goldbach_number {
  int64_t number;
  int64_t amount_of_sums;
  int64_t* sums;
} goldbach_number_t;

/**
 * @brief prime_numbers es el array de numeros primos
 *        size es el tamano del array
 * 
 */
typedef struct prime_numbers {
  int64_t* prime_numbers;
  int64_t size;
} prime_numbers_t;

/**
 * @brief my_input es el registro compartido de input_t pero desde un
 *        determinado indice calculado anteriormente
 *        my_goldbach_numbers es el registro compartido de goldbach_number_t 
 *        pero desde un determinado indice calculado anteriormente
 *        assigned_work_units es analogo a un size, y le dice a cada hilo
 *        cuantos numeros debe procesar de my_input y sus resultados guardarlos
 *        en my_goldbach_numbers
 * 
 *        las 3 variables en conjunto permiten manipular la memoria compartida
 *        de una manera thread safe, debido a que cada hilo tiene asignado 
 *        modificar solo un subarreglo de la memoria compartida
 * 
 *        va a ser utilizado como memoria privada
 * 
 */
typedef struct my_work {
  int64_t* my_input;
  goldbach_number_t** my_goldbach_numbers;
  int64_t assigned_work_units;
} my_work_t;

#endif  // LIBRARIES
