/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "libraries_and_structs.h"

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
