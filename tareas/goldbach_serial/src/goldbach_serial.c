// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "goldbach_serial.h"

/**
 * @mainpage Por tomar en cuenta:
 *           - Todas las funciones imprimen los errores que generan
 *           - errno solo es modificado manualmente en errores de flujo,
 *             no de alojamiento en memoria, etc
 *           - Si una funcion que devuelve un puntero da error, va a devolver 0
 * 
 */

int main() {
  /// recibir el input y manejar si no fue exitoso
  input_t* input = read_input();
  if (!input) {
    return errno;
  }

  /// crear array de goldbach_number_t* del tamano del input
  goldbach_number_t** goldbach_numbers = (goldbach_number_t**) 
    calloc(input->size, sizeof(goldbach_number_t));
  if (!goldbach_numbers) {
    fprintf(stderr, "Error: Could not allocate goldbach_numbers in memory."
      " errno: %i\n", errno);
    return errno;
  }

  /// por cada celda del array, calcular las sumas de Goldbach
  for (int64_t i = 0; i < input->size; i++) {
    goldbach_numbers[i] = calc_goldbach_number(input->input_arr[i]);
    /// si calcular las sumas dio error, devuelve el codigo de error
    if (!goldbach_numbers[i]) {
      return errno;
    }
  }

  /// imprimir el array de goldbach_number_t
  print_goldach_numbers(goldbach_numbers, input->size);

  /// liberar memoria que se necesito hasta el final
  for (int64_t i = 0; i < input->size; i++) {
    free(goldbach_numbers[i]->sums);
    free(goldbach_numbers[i]);
  }
  free(goldbach_numbers);
  free(input->input_arr);
  free(input);
  return errno;
}

input_t* read_input() {
  input_t* input = (input_t*) calloc(1, sizeof(input_t));
  int64_t* input_arr = (int64_t*) calloc(1, sizeof(int64_t));
  if (!(input && input_arr)) {  /// validar memoria
    fprintf(stderr, "Error: Could not allocate input/input_arr"
      " in memory. errno: %i\n", errno);
    return NULL;
  }

  int64_t count = 0;

  FILE* f_input = stdin;
  int64_t value = 0;
  while (1) {
    if ((fscanf(f_input, "%" SCNd64, &value)) == 1) {
      input_arr[count] = value;
      count++;
      input_arr = (int64_t*) realloc(input_arr, sizeof(int64_t) * (count + 1));
    } else {  /// si no logro leer mas
      if (fgetc(f_input) != EOF) {  /// preguntamos si es por el eof
        /// sino es por el eof, es porque quiso leer algun tipo de dato como
        /// int64t cuando no lo era, por lo que liberamos memoria, setteamos
        /// codigo de error e imprimimos el error
        free(input_arr);
        free(input);
        errno = 21;
        fprintf(stderr, "Error: invalid input. errno: %i\n", errno);
        return NULL;
      }
      /// si si fue por el EOF, no hay problema entonces solo salga del while
      /// infinito
      break;
    }
  }

  /// setteamos los valores que obtenimos
  input->input_arr = input_arr;
  input->size = count;

  return input;
}

goldbach_number_t* calc_goldbach_number(int64_t number) {
  goldbach_number_t* goldbach_number = (goldbach_number_t*) 
    calloc(1, sizeof(goldbach_number_t));
  if (!goldbach_number) {
    fprintf(stderr, "Error: Could not allocate goldbach_number in memory."
    " errno: %i\n", errno);
    return NULL;
  }

  goldbach_number->number = number;

  /// si el valor absoluto del numero es menor que 5, no aplica para la
  /// conjetura de Goldbach, por lo que devolvemos con los valores inicales
  /// y se entiende que es NA
  if (!(number > 5 || number < -5)) {
    return goldbach_number;
  }

  /// setteamos una variable auxiliar para los negativos
  int64_t is_negative = 1;
  if (number < 0) {
    is_negative = -1;
  }

  /// obtenemos los numeros primos
  prime_numbers_t* prime_numbers = calc_prime_numbers(number * is_negative);
  if (!prime_numbers) {  /// y validamos que se hayan obtenido correctamente
    fprintf(stderr, "Error: Could not calculate prime_numbers for nummber:"
      " %" PRId64 ". errno: %i\n", number, errno);
    return NULL;
  }

  /// si el numero es par vamos a ocupar 2 for
  if (number % 2 == 0) {
    for (int64_t i = 0; i < prime_numbers->size; i++) {
      for (int64_t j = i; j < prime_numbers->size; j++) {
        /// se multiplica por el auxiliar is_negative por orden
        if ((prime_numbers->prime_numbers[i] + prime_numbers->prime_numbers[j])
          == (number * is_negative)) {
          /// se incrementa la cantidad de sumas encontradas
          goldbach_number->amount_of_sums++;
          /// si es negativo se debe hacer el "desglose"
          if (is_negative == -1) {
            /// entonces reservamos mas memoria
            goldbach_number->sums = (int64_t*) realloc(goldbach_number->sums
              , sizeof(int64_t) * 2 * goldbach_number->amount_of_sums);
            if (goldbach_number->sums) {  /// y si la reserva fue exitosa
              /// asignamos los valores
              goldbach_number->sums[(goldbach_number->amount_of_sums - 1)
                * 2 + 0] = prime_numbers->prime_numbers[i];
              goldbach_number->sums[(goldbach_number->amount_of_sums - 1) 
                * 2 + 1] = prime_numbers->prime_numbers[j];
            } else {  /// si no fue exitosa
              fprintf(stderr, "Error: Could not allocate goldach_numbers[i]."
                " sums in memory. errno: %i\n", errno);
              return NULL;
            }
          }
        }
      }
    }
  } else {  /// si es impar hacemos practicamente la misma logica pero con 3 for
    for (int64_t i = 0; i < prime_numbers->size; i++) {
      for (int64_t j = i; j < prime_numbers->size; j++) {
        for (int64_t k = j; k < prime_numbers->size; k++) {
          if ((prime_numbers->prime_numbers[i] + prime_numbers->prime_numbers[j]
           + prime_numbers->prime_numbers[k]) == (number * is_negative)) {
            goldbach_number->amount_of_sums++;
            if (is_negative == -1) {
              goldbach_number->sums = (int64_t*) realloc(goldbach_number->sums
              , sizeof(int64_t) * 3 * goldbach_number->amount_of_sums);
              if (goldbach_number->sums) {
                goldbach_number->sums[(goldbach_number->amount_of_sums - 1)
                  * 3 + 0] = prime_numbers->prime_numbers[i];
                goldbach_number->sums[(goldbach_number->amount_of_sums - 1)
                  * 3 + 1] = prime_numbers->prime_numbers[j];
                goldbach_number->sums[(goldbach_number->amount_of_sums - 1)
                  * 3 + 2] = prime_numbers->prime_numbers[k];
              } else {
                fprintf(stderr, "Error: Could not allocate goldach_numbers[i]."
                  " sums in memory. errno: %i\n", errno);
                return NULL;
              }
            }
          }
        }
      }
    }
  }

  /// liberamos memoria que no vamos a ocupar mas
  free(prime_numbers->prime_numbers);
  free(prime_numbers);
  return goldbach_number;
}

prime_numbers_t* calc_prime_numbers(int64_t number) {
  prime_numbers_t* prime_numbers = (prime_numbers_t*) calloc(1
  , sizeof(prime_numbers_t));
  int64_t* prime_numbers_aux = (int64_t*) calloc(number + 1, sizeof(int64_t));
  bool* sieve = (bool*) calloc(number + 1, sizeof(bool));
  if (!(prime_numbers && prime_numbers_aux && sieve)) {  /// validamos
    fprintf(stderr, "Error: Could not allocate prime_numbers/prime_numbers_aux"
      " /sieve in memory. errno: %i\n", errno);
    return NULL;
  }

  int64_t count = 0;

  for (int64_t i = 2; i <= number; i++) {  /// settear criba en true
    sieve[i] = true;
  }

  for (int64_t i = 2; i <= number; i++) {
    if (sieve[i] == true) {  /// desde cada true marcar multiplos como no primos
      for (int64_t j = i + i; j <= number; j += i) {  /// se suma en i
        sieve[j] = false;
      }
    }
  }

  for (int64_t i = 2; i <= number; i++) {
    if (sieve[i] == true) {  /// si la posicion es true es que es primo
      prime_numbers_aux[count] = i;  /// y el numero igual a la posicion
      count++;
    }
  }

  /// seteamos los valores en la estructura creada
  prime_numbers->prime_numbers = prime_numbers_aux;
  prime_numbers->size = count;

  /// liberamos memoria que no se va a usar mas
  free(sieve);
  return prime_numbers;
}


void print_goldach_numbers(goldbach_number_t** goldbach_numbers, int64_t size) {
  int64_t total_amount_of_sums = 0;
  for (int64_t i = 0; i < size; i++) {  /// obtener el numero total de sumas
    total_amount_of_sums += goldbach_numbers[i]->amount_of_sums;
  }

  /// Imprimir el encabezado
  fprintf(stdout, "Total: %" PRId64 " numbers %" PRId64 " sums\n\n", size
  , total_amount_of_sums);

  for (int64_t i = 0; i < size; i++) {
    /// imprimir numeros cuyo valor absoluto es menor que 5
    if (goldbach_numbers[i]->sums == 0 && goldbach_numbers[i]
      ->amount_of_sums == 0) {
      fprintf(stdout, "%" PRId64 ": NA\n", goldbach_numbers[i]->number);
      continue;
    }

    /// imprimir dependiendo de si es negativo o positivo
    if (goldbach_numbers[i]->number < 0) {
      fprintf(stdout, "%" PRId64 ": %" PRId64 " sums: ", goldbach_numbers[i]
        ->number, goldbach_numbers[i]->amount_of_sums);
      if (goldbach_numbers[i]->number % 2 == 0) {
        for (int64_t j = 0; j < goldbach_numbers[i]
          ->amount_of_sums * 2; j += 2) {
          if (j + 2 == goldbach_numbers[i]->amount_of_sums * 2) {
            fprintf(stdout, "%" PRId64 " + %" PRId64, goldbach_numbers[i]
              ->sums[j + 0], goldbach_numbers[i]->sums[j + 1]);
          } else {
            fprintf(stdout, "%" PRId64 " + %" PRId64 ", ", goldbach_numbers[i]
              ->sums[j + 0], goldbach_numbers[i]->sums[j + 1]);
          }
        }
        fprintf(stdout, "\n");
      } else {
        for (int64_t j = 0; j < goldbach_numbers[i]
          ->amount_of_sums * 3; j += 3) {
          if (j + 3 == goldbach_numbers[i]->amount_of_sums * 3) {
            fprintf(stdout, "%" PRId64 " + %" PRId64 " + %" PRId64
              , goldbach_numbers[i]->sums[j + 0]
              , goldbach_numbers[i]->sums[j + 1]
              , goldbach_numbers[i]->sums[j + 2]);
          } else {
            fprintf(stdout, "%" PRId64 " + %" PRId64 " + %" PRId64 ", "
              , goldbach_numbers[i]->sums[j + 0], goldbach_numbers[i]
              ->sums[j + 1], goldbach_numbers[i]->sums[j + 2]);
          }
        }
        fprintf(stdout, "\n");
      }
    } else {
      fprintf(stdout, "%" PRId64 ": %" PRId64 " sums\n", goldbach_numbers[i]
        ->number, goldbach_numbers[i]->amount_of_sums);
    }
  }
}
