/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "libraries_and_structs.h"

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
void create_threads(input_t* input, int64_t thread_count, int argc, char* argv[]);

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
