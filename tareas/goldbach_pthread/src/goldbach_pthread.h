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

/**
 * @brief funcion principal, corrida por el hilo principal, llama a run
 * 
 * @return mensaje de error
 */
int main();

/**
 * @brief maneja el flujo del programa
 * 
 */
void run(int argc, char* argv[]);


/**
 * @brief lee linea por linea hasta el EOF
 * 
 * @return on success: puntero a input_t con la data recopilada
 *         on failure: 0
 */
input_t* read_input();

/**
 * @brief analiza los argumentos dados por la linea de comandos
 * 
 * @param argc total de argumentos pasados por terminal
 * @param argv array de argumentos pasados por terminal
 * @return cantidad de hilos pasados por el segundo argumento de la terminal
 *         y si no se paso, o se paso uno invalido se asigna la cantidad de
 *         hilos del sistema
 */
int analyze_arguments(int argc, char* argv[]);

/**
 * @brief crea un equipo de hilos con trabajo mapeado equitativamente
 *        con la formula de mapeo por bloque se se proporciono en la pagina
 *        de jeisson
 * 
 * @param input numeros ingresados por el usuario
 * @param thread_count numero de hilos en el equipo
 * @param goldbach_numbers estructura donde se van a almacenar las sumas 
 *        de goldbach
 */
void create_threads(input_t* input, goldbach_number_t** goldbach_numbers
  , int64_t thread_count);

/**
 * @brief cada hilo creado va a correr esta funcion la cual llama 
 *        calc_goldbach_number
 * 
 * @param my_work registro que contiene
 *          direccion de memoria al inicio del subarreglo de input (shared 
 *            memory) que le corresponde a cada hilo
 *          direccion de memoria al inicio del subarreglo de goldbach_numbers
 *            (shared memory) que le corrseponde a cada hilo
 *          cantidad de elementos a analizar del subarreglo correpsondiente
 */
void* run_thread(void* data);

/**
 * @brief hace join de los hilos una despues que terminan su trabajo
 * 
 * @param input se pasa input para de ahi sacar el array de threads
 * @param thread_count numero de threads a hacerles join
 */
void join_threads(input_t* input, int64_t thread_count);

/**
 * @brief Devuelve el inicio del subarreglo a analizar en funcion del hilo
 * 
 * @param i numero de hilo
 * @param D cantidad de datos
 * @param w cantidad de hilos
 * @return inicio del subarreglo corespondiente
 */
int64_t get_static_block_start(int64_t i, int64_t D, int64_t w);

/**
 * @brief Devuelve el minimo de dos numeros
 * 
 * @param i numero de hilo
 * @param mod operacion modulo entre la cantidad de datos y la cantidad de hilos
 * @return el menor de los dos numeros
 */
int64_t get_static_block_min(int64_t i, int64_t mod);

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
void print_goldach_numbers(goldbach_number_t** goldbach_numbers, int64_t size);

#endif  // GOLDBACH_SERIAL_H
