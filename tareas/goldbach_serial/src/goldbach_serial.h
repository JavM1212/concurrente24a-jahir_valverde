// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#ifndef GOLDBACH_SERIAL_H
#define GOLDBACH_SERIAL_H

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
 * 
 */
typedef struct input {
  int64_t* input_arr;
  int64_t size;
} input_t;

/**
 * @brief number es el numero al que se le dean calcular las sumas
 *        amount_of_sums es la cantidad de sumas encontradas
 *        sums es el array a las sumas
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
 * @brief Maneja el flujo del programa
 * 
 * @return on success: 0
 *         on failure: numero distinto de 0
 */
int main();

/**
 * @brief Lee linea por linea hasta el EOF
 * 
 * @return on success: puntero a input_t con la data recopilada
 *         on failure: 0
 */
input_t* read_input();

/**
 * @brief Calcula las sumas de Goldbach para un numero 
 * 
 * @param number Int de 64 bits al que se le desea calcular las sumas de
 *        goldbach
 * @return on success: registro de tipo goldbach_number_t con los valores 
 *         encontrados 
 *         on failure: 0
 */
goldbach_number_t* calc_goldbach_number(int64_t number);

/**
 * @brief Calcula los numeros primos menores a si mismo
 * 
 * @param number Int de 64 bits al que se le quieren calcular los numeros 
 *        primos menores a si mismo
 * @return on success: puntero a prime_numbers_t con los numeros calculados
 *         on failure: 0
 */
prime_numbers_t* calc_prime_numbers(int64_t number);

/**
 * @brief imprime sistematicamente el contenido del array de goldbach_number_t 
 *        obtenido anteriormente
 * 
 * @param goldbach_numbers** array donde guardamos los datos de las sumas de 
 *        Goldbach
 * @param size tamano del array goldbach_numbers*
 */
void print_goldach_numbers(goldbach_number_t** goldbach_numbers, int64_t size);

#endif  // GOLDBACH_SERIAL_H
