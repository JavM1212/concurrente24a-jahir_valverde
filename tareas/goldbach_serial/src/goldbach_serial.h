// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#ifndef GOLDBACH_SERIAL_H
#define GOLDBACH_SERIAL_H

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct input {
  int64_t* input_arr;
  int64_t size;
} input_t;

typedef struct goldbach_number {
  int64_t number;
  int64_t amount_of_sums;
  int64_t* sums;
} goldbach_number_t;

typedef struct prime_numbers {
  int64_t* prime_numbers;
  int64_t size;
} prime_numbers_t;

input_t* read_input();

goldbach_number_t calc_goldbach_number(int64_t number);

prime_numbers_t* calc_prime_numbers(int64_t number);

void print_goldach_numbers(goldbach_number_t* goldbach_numbers, int64_t amount);

#endif  // GOLDBACH_SERIAL_H
