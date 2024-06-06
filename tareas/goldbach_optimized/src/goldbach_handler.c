/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "goldbach_handler.h"

/// calcular las sumas de goldbach para un numero
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


  // OPTIMIZACION 1: 1 for para numeros pares, 2 fors para numeros impares
  // Si el número es par
  if (number % 2 == 0) {
    for (int64_t i = 0; i < prime_numbers->size; i++) {
      int64_t prime1 = prime_numbers->prime_numbers[i];
      int64_t prime2 = (number * is_negative) - prime1;
      if (is_prime(prime2) && prime2 >= prime1) {
        goldbach_number->amount_of_sums++;
        if (is_negative == -1) {
          goldbach_number->sums = (int64_t*) realloc(goldbach_number->sums
            , sizeof(int64_t) * 2 * goldbach_number->amount_of_sums);
          if (goldbach_number->sums) {
            goldbach_number
              ->sums[(goldbach_number->amount_of_sums - 1) * 2 + 0] = prime1;
            goldbach_number
              ->sums[(goldbach_number->amount_of_sums - 1) * 2 + 1] = prime2;
          } else {
            fprintf(stderr, "Error: Could not allocate goldbach_number->sums"
              " in memory. errno: %i\n", errno);
            free(goldbach_number);
            return NULL;
          }
        }
      }
    }
  } else {  // Si el número es impar
    for (int64_t i = 0; i < prime_numbers->size; i++) {
      for (int64_t j = i; j < prime_numbers->size; j++) {
        int64_t prime1 = prime_numbers->prime_numbers[i];
        int64_t prime2 = prime_numbers->prime_numbers[j];
        int64_t prime3 = (number * is_negative) - prime1 - prime2;
        if (is_prime(prime3) && prime3 >= prime2) {
          goldbach_number->amount_of_sums++;
          if (is_negative == -1) {
            goldbach_number->sums = (int64_t*)realloc(goldbach_number->sums
              , sizeof(int64_t) * 3 * goldbach_number->amount_of_sums);
            if (goldbach_number->sums) {
              goldbach_number
                ->sums[(goldbach_number->amount_of_sums - 1) * 3 + 0] = prime1;
              goldbach_number
                ->sums[(goldbach_number->amount_of_sums - 1) * 3 + 1] = prime2;
              goldbach_number
                ->sums[(goldbach_number->amount_of_sums - 1) * 3 + 2] = prime3;
            } else {
              fprintf(stderr, "Error: Could not allocate goldbach_number->sums"
                " in memory. errno: %i\n", errno);
              free(goldbach_number);
              return NULL;
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

/// verificar si un número es primo o no
bool is_prime(int64_t num) {
  if (num <= 1) return false;
  if (num <= 3) return true;
  if (num % 2 == 0 || num % 3 == 0) return false;
  for (int64_t i = 5; i * i <= num; i += 6) {
    if (num % i == 0 || num % (i + 2) == 0) return false;
  }
  return true;
}

/// calcular los numeros primos menos a si mismo
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

/// imprimir los resultados
void print_goldach_numbers(input_t* input, int64_t size) {
  // int64_t total_amount_of_sums = 0;
  // for (int64_t i = 0; i < size; i++) {  /// obtener el numero total de sumas
  //   total_amount_of_sums += goldbach_numbers[i]->amount_of_sums;
  // }

  /// Imprimir el encabezado
  /*fprintf(stdout, "Total: %" PRId64 " numbers %" PRId64 " sums\n\n", size
  , total_amount_of_sums);*/

  goldbach_number_t** goldbach_numbers = input->goldbach_numbers;
  goldbach_number_t* gold_to_print = 0;

  for (int64_t j = 0; j < input->size; j++) {
    for (int64_t k = 0; k < input->goldbach_numbers_index; k++) {
      if (input->input_arr[j] == input->goldbach_numbers[k]->number) {
        gold_to_print = input->goldbach_numbers[k];
      }
    }

    if (!gold_to_print) {
      continue;
    }

      /// imprimir numeros cuyo valor absoluto es menor que 5
      if (gold_to_print->sums == 0 && gold_to_print
        ->amount_of_sums == 0) {
        fprintf(stdout, "%" PRId64 ": NA\n", gold_to_print->number);
        continue;
      }


    /// imprimir dependiendo de si es negativo o positivo
    if (gold_to_print->number < 0) {
      fprintf(stdout, "%" PRId64 ": %" PRId64 " sums: ", gold_to_print
        ->number, gold_to_print->amount_of_sums);
      if (gold_to_print->number % 2 == 0) {
        for (int64_t j = 0; j < gold_to_print
          ->amount_of_sums * 2; j += 2) {
          if (j + 2 == gold_to_print->amount_of_sums * 2) {
            fprintf(stdout, "%" PRId64 " + %" PRId64, gold_to_print
              ->sums[j + 0], gold_to_print->sums[j + 1]);
          } else {
            fprintf(stdout, "%" PRId64 " + %" PRId64 ", ", gold_to_print
              ->sums[j + 0], gold_to_print->sums[j + 1]);
          }
        }
        fprintf(stdout, "\n");
      } else {
        for (int64_t j = 0; j < gold_to_print
          ->amount_of_sums * 3; j += 3) {
          if (j + 3 == gold_to_print->amount_of_sums * 3) {
            fprintf(stdout, "%" PRId64 " + %" PRId64 " + %" PRId64
              , gold_to_print->sums[j + 0]
              , gold_to_print->sums[j + 1]
              , gold_to_print->sums[j + 2]);
          } else {
            fprintf(stdout, "%" PRId64 " + %" PRId64 " + %" PRId64 ", "
              , gold_to_print->sums[j + 0], gold_to_print
              ->sums[j + 1], gold_to_print->sums[j + 2]);
          }
        }
        fprintf(stdout, "\n");
      }
    } else {
      fprintf(stdout, "%" PRId64 ": %" PRId64 " sums\n", gold_to_print
        ->number, gold_to_print->amount_of_sums);
    }

  }
}
