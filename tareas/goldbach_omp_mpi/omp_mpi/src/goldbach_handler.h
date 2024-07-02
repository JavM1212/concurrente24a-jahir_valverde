/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "libraries_and_structs.h"

/**
 * @brief calcula las sumas de Goldbach para un numero 
 * 
 * @param number int al que se le desea calcular las sumas de goldbach
 * @return on success: puntero a goldbach_number_t con los valores 
 *         encontrados 
 *         on failure: 0
 */
goldbach_number_t* calc_goldbach_number(int64_t number);

/**
 * @brief calcula los numeros primos menores a si mismo
 * 
 * @param number int de 64 bits al que se le quieren calcular los numeros 
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
void print_goldach_numbers(input_t* input, int process_start
  , int process_finish);

bool is_prime(int64_t num);
