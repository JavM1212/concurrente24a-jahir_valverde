// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "goldbach_serial.h"

int main() {
  int64_t main_error = EXIT_SUCCESS;

  input_t* input = read_input();
  if (!input) {
    fprintf(stderr, "Error: Your input is invalid");
    return EXIT_FAILURE;
  }

  goldbach_number_t* goldbach_numbers = (goldbach_number_t*) calloc(input->size, sizeof(goldbach_number_t));
  if (!goldbach_numbers) {   
    fprintf(stderr, "Error: Could not allocate goldbach_numbers in memory");
    return EXIT_FAILURE; 
  }

  for (int64_t i = 0; i < input->size; i++) {
    goldbach_numbers[i] = calc_goldbach_number(input->input_arr[i]);
  }

  print_goldach_numbers(goldbach_numbers, input->size);

  for (int64_t i = 0; i < input->size; i++) {
    free(goldbach_numbers[i].sums);
  }
  free(goldbach_numbers);
  free(input->input_arr);
  free(input);
  return main_error;
} 

input_t* read_input() {
  input_t* input = (input_t*) calloc(1, sizeof(input_t)); 
  int64_t* input_arr = (int64_t*) calloc(1, sizeof(int64_t));

  if (!(input && input_arr)) {
    fprintf(stderr, "Error: Could not allocate input/input_arr in memory");
    return input;
  }

  int64_t count = 0;

  FILE* f_input = stdin;
  int64_t value = 0;
  while((fscanf(f_input, "%" SCNd64, &value)) == 1) {
      input_arr[count] = value;
      count++;
      input_arr = (int64_t*) realloc(input_arr, sizeof(int64_t) * (count + 1));
  }

  input->input_arr = input_arr;
  input->size = count;

  return input;
}

goldbach_number_t calc_goldbach_number(int64_t number) {
  goldbach_number_t goldbach_number = {number, 0, 0};

  if (!(number > 5 || number < -5)) {
    return goldbach_number;
  }

  int64_t is_negative = 1;
  if (number < 0) {
    is_negative = -1;
  }

  prime_numbers_t* prime_numbers = calc_prime_numbers(number * is_negative);
  if (!prime_numbers) {
    fprintf(stderr, "Error: Could not calculate prime_numbers for nummber: %" PRId64, number);
    return goldbach_number;
  }

  if (number % 2 == 0) {
    for (int64_t i = 0; i < prime_numbers->size; i++) {
      for (int64_t j = i; j < prime_numbers->size; j++) {
        if ((prime_numbers->prime_numbers[i] + prime_numbers->prime_numbers[j]) == (number * is_negative)) {
          goldbach_number.amount_of_sums++;
          if (is_negative == -1) {
            goldbach_number.sums = (int64_t*) realloc(goldbach_number.sums, sizeof(int64_t) * 2 * goldbach_number.amount_of_sums);
            goldbach_number.sums[(goldbach_number.amount_of_sums - 1) * 2 + 0] = prime_numbers->prime_numbers[i];
            goldbach_number.sums[(goldbach_number.amount_of_sums - 1) * 2 + 1] = prime_numbers->prime_numbers[j];
          }
        }
      }
    }
  } else {
    for (int64_t i = 0; i < prime_numbers->size; i++) {
      for (int64_t j = i; j < prime_numbers->size; j++) {
        for (int64_t k = j; k < prime_numbers->size; k++) {
          if ((prime_numbers->prime_numbers[i] + prime_numbers->prime_numbers[j] + prime_numbers->prime_numbers[k]) == (number * is_negative)) {
            goldbach_number.amount_of_sums++;
            if (is_negative == -1) {
              goldbach_number.sums = (int64_t*) realloc(goldbach_number.sums, sizeof(int64_t) * 3 * goldbach_number.amount_of_sums);
              goldbach_number.sums[(goldbach_number.amount_of_sums - 1) * 3 + 0] = prime_numbers->prime_numbers[i];
              goldbach_number.sums[(goldbach_number.amount_of_sums - 1) * 3 + 1] = prime_numbers->prime_numbers[j];
              goldbach_number.sums[(goldbach_number.amount_of_sums - 1) * 3 + 2] = prime_numbers->prime_numbers[k];
            }
          }
        }
      }
    }
  }

  free(prime_numbers->prime_numbers);
  free(prime_numbers);
  return goldbach_number;
}

prime_numbers_t* calc_prime_numbers(int64_t number) {
  prime_numbers_t* prime_numbers = (prime_numbers_t*) calloc(1, sizeof(prime_numbers_t));
  int64_t* prime_numbers_aux = (int64_t*) calloc(number + 1, sizeof(int64_t));
  bool* sieve = (bool*) calloc(number + 1, sizeof(bool));

  if (!(prime_numbers && prime_numbers_aux && sieve)) {
    fprintf(stderr, "Error: Could not allocate prime_numbers/prime_numbers_aux/sieve in memory");
    return prime_numbers;
  }

  int64_t count = 0;

  for (int64_t i = 2; i <= number; i++) {
    sieve[i] = true;
  }

  for (int64_t i = 2; i <= number; i++) {
    if (sieve[i] == true) {
      for (int64_t j = i + i; j <= number; j += i) {
        sieve[j] = false;
      }
    }
  }

  for (int64_t i = 2; i <= number; i++) {
    if (sieve[i] == true) {
      prime_numbers_aux[count] = i;
      count++;
    }
  }

  prime_numbers->prime_numbers = prime_numbers_aux;
  prime_numbers->size = count;

  free(sieve);

  return prime_numbers;
}

void print_goldach_numbers(goldbach_number_t* goldbach_numbers, int64_t amount) {
  int64_t total_amount_of_sums = 0;
  for (int64_t i = 0; i < amount; i++) {
    total_amount_of_sums += goldbach_numbers[i].amount_of_sums;
  }

  fprintf(stdout, "Total: %" PRId64 " numbers %" PRId64 " sums\n\n", amount, total_amount_of_sums);

  for (int64_t i = 0; i < amount; i++) {
    if (goldbach_numbers[i].sums == 0 && goldbach_numbers[i].amount_of_sums == 0) {
      fprintf(stdout, "%" PRId64 ": NA\n", goldbach_numbers[i].number);
      continue;
    }
    if (goldbach_numbers[i].number < 0) {
      fprintf(stdout, "%" PRId64 ": %" PRId64 " sums: ", goldbach_numbers[i].number, goldbach_numbers[i].amount_of_sums);
      if (goldbach_numbers[i].number % 2 == 0) {
        for (int64_t j = 0; j < goldbach_numbers[i].amount_of_sums * 2; j += 2) {
          if (j + 2 == goldbach_numbers[i].amount_of_sums * 2) {
            fprintf(stdout, "%" PRId64 " + %" PRId64, goldbach_numbers[i].sums[j + 0], goldbach_numbers[i].sums[j + 1]);
          } else {
            fprintf(stdout, "%" PRId64 " + %" PRId64 ", ", goldbach_numbers[i].sums[j + 0], goldbach_numbers[i].sums[j + 1]);
          }
        }
        fprintf(stdout, "\n");
      } else {
        for (int64_t j = 0; j < goldbach_numbers[i].amount_of_sums * 3; j += 3) {
          if (j + 3 == goldbach_numbers[i].amount_of_sums * 3) {
            fprintf(stdout, "%" PRId64 " + %" PRId64 " + %" PRId64, goldbach_numbers[i].sums[j + 0], goldbach_numbers[i].sums[j + 1], goldbach_numbers[i].sums[j + 2]);
          } else {
            fprintf(stdout, "%" PRId64 " + %" PRId64 " + %" PRId64 ", ", goldbach_numbers[i].sums[j + 0], goldbach_numbers[i].sums[j + 1], goldbach_numbers[i].sums[j + 2]);
          }
        }
        fprintf(stdout, "\n");
      }
    } else {
      fprintf(stdout, "%" PRId64 ": %" PRId64 " sums\n", goldbach_numbers[i].number, goldbach_numbers[i].amount_of_sums);
    }
  }
}
