// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#ifndef GOLDBACH_SERIAL_H
#define GOLDBACH_SERIAL_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/// type goldbach_sum
typedef struct goldach_sum {
  /// number = 0;
  int number;
  /// prime_summands**;  /// ** matriz en memoria
  int** prime_summands;
  /// even_or_odd = 0;
  int even_or_odd;
  /// invalid = 0;  
  int invalid;
} goldbach_sum_t;  /// end type

/// type goldbach_summary 
typedef struct goldbach_summary {
  /// numbers = 0;
  int numbers;
  /// sums = 0;
  int sums;
  /// goldbach_sum*;
  goldbach_sum_t* goldbach_sums;
} goldbach_summary_t;  /// end type

typedef struct input {
  int* input_arr;
  int size;
  goldbach_sum_t* goldbach_sums;
} input_t;  /// end type

int main();

input_t* read_input();

goldbach_summary_t* calc_goldbach(input_t* input);

int* even_goldbach(int number);

int* odd_goldbach(int number);

#endif  // GOLDBACH_SERIAL_H
