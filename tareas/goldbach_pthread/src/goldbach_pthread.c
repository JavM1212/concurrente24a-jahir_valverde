// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include "goldbach_pthread.h"

/// llama a la funcion run (la cual tiene el flujo del programa) y calcula
/// el tiempo que dura en ejecutarla
int main(int argc, char* argv[]) {
  /// obtener la hora antes de ejecutar run
  struct timespec start_time;
  clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

  run(argc, argv);

  /// obtener la hora despues de ejecutar run
  struct timespec finish_time;
  clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

  /// tiempo transcurrido = hora final - hora inicial
  double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed);

  return errno;
}

/// controla el flujo del programa
void run(int argc, char* argv[]) {
  /// recibir input
  input_t* input = read_input();
  if (!input) {
    return;
  }

  /// recibir cantidad de hilos
  int64_t thread_count = analyze_arguments(argc, argv);

  /// inicializar los hilos que seran usados posteriormente
  input->threads = (pthread_t*) calloc(thread_count, sizeof(pthread_t));
  if (!input->threads) {
    fprintf(stderr, "Error: Could not allocate threads in memory."
      " errno: %i\n", errno);
    return;
  }

  /// crear array de goldbach_number_t* del tamano del input
  goldbach_number_t** goldbach_numbers = (goldbach_number_t**)
    calloc(input->size, sizeof(goldbach_number_t));
  if (!goldbach_numbers) {
    fprintf(stderr, "Error: Could not allocate goldbach_numbers in memory."
      " errno: %i\n", errno);
    return;
  }

  /// crear el equipo de trabajo con los argumentos necesarios
  create_threads(input, goldbach_numbers, thread_count);

  /// hacerle merge al equipo
  join_threads(input, thread_count);

  /// imprimir el array de goldbach_number_t
  print_goldach_numbers(goldbach_numbers, input->size);

  /// liberar memoria que se necesito hasta el final
  free(input->threads);
  for (int64_t i = 0; i < input->size; i++) {
    free(goldbach_numbers[i]->sums);
    free(goldbach_numbers[i]);
  }
  free(goldbach_numbers);
  free(input->input_arr);
  free(input);
}

/// lee de la entrada estandar los numeros ingresados y valida que sean ints
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

/// obtener argumentos de la linea de comandos
int analyze_arguments(int argc, char* argv[]) {
  int thread_count = sysconf(_SC_NPROCESSORS_ONLN);

  /// si son 2 argumentos settea el segundo
  if (argc == 2) {
    int temp_thread_count = atoi(argv[1]);

    /// si la entrada fue valida la asignamos
    if (temp_thread_count > 0) {
      thread_count = temp_thread_count;
    }
  }

  return thread_count;
}

/// crear equipo de hilos
void create_threads(input_t* input, goldbach_number_t** goldbach_numbers
  , int64_t thread_count) {
  int64_t start = 0;
  int64_t finish = 0;

  /// inicializar un my_work por cada hilo
  my_work_t* my_works = (my_work_t*) calloc(thread_count, sizeof(my_work_t));
  if (!my_works) {
    fprintf(stderr, "Error: Could not allocate my_works in memory."
      " errno: %i\n", errno);
    return;
  }

  for (int thread = 0; thread < thread_count; thread++) {
    /// calcular el indice del inicio del subarreglo de memoria compartida 
    /// correspondiente
    start = get_static_block_start(thread, input->size, thread_count);
    /// calcular el indice del final del subarreglo de memoria compartida
    /// correspondiente
    finish = get_static_block_start(thread + 1, input->size, thread_count);

    /// asignar a cada my_work su subarreglo de memoria compartida
    my_works[thread].my_input = input->input_arr + start;
    my_works[thread].my_goldbach_numbers = goldbach_numbers + start;
    /// asignar la cantidad de trabajo en cada subarreglo de memoria compartida
    /// tambien se puede ver como asignar el tamano de cada subarreglo de
    /// memoria compartida
    my_works[thread].assigned_work_units = finish - start;

    /// crear el hilo pasando por parametro el my_work correspondiente
    errno = pthread_create(&input->threads[thread], /*attr*/ NULL, run_thread
      , /*arg*/ &my_works[thread]);
  }
}

/// funcion corrida por hilos, calcula los goldbach_number para cada unidad
/// del subarreglo memoria compartida
void* run_thread(void* data) {
  my_work_t* my_work = (my_work_t*) data;

  /// como el subarreglo de memoria compartida ya le fue calculado su offset
  /// solo hace falta recorrer hasta el assigned_work_units calculado 
  /// anteriormente
  for (int i = 0; i < my_work->assigned_work_units; i++) {
    my_work->my_goldbach_numbers[i] =
      calc_goldbach_number(my_work->my_input[i]);
  }

  return NULL;
}

/// hacerle merge al equipo de trabajo
void join_threads(input_t* input, int64_t thread_count) {
  for (int thread = 0; thread < thread_count; thread++) {
    pthread_join(input->threads[thread], /*value_ptr*/ NULL);
  }
}

/// obtener el indice de inicio que le corresponde a cada hilo
int64_t get_static_block_start(int64_t i, int64_t D, int64_t w) {
  int64_t min = get_static_block_min(i, (D % w));
  return i * (D / w) + min;
}

/// obtener el menor de dos numeros
int64_t get_static_block_min(int64_t i, int64_t mod) {
  if (i < mod) {
    return i;
  }
  if (i > mod) {
    return mod;
  }
  // i == mod
  return i;
}

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
void print_goldach_numbers(goldbach_number_t** goldbach_numbers, int64_t size) {
  // int64_t total_amount_of_sums = 0;
  // for (int64_t i = 0; i < size; i++) {  /// obtener el numero total de sumas
  //   total_amount_of_sums += goldbach_numbers[i]->amount_of_sums;
  // }

  /// Imprimir el encabezado
  /*fprintf(stdout, "Total: %" PRId64 " numbers %" PRId64 " sums\n\n", size
  , total_amount_of_sums);*/

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
