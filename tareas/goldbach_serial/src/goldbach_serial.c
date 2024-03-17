// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "goldbach_serial.h"

/// procedure main()
int main() {
  int main_error = 0;

  /// input_arr = read_input()
  input_t* input = read_input();

  /// if sizeof input_arr != NULL
  if (input == NULL) {
    main_error = 1;
  } else {
    ///   goldbach_summary = allocate memory
    goldbach_summary_t* goldbach_summary = calc_goldbach(input);
    printf("goldbach_summary->goldbach_sums[0]: %d", goldbach_summary->goldbach_sums[11].number);

    //   calc_goldbach(input_arr, goldbach_summary)
    //   print_goldbach(goldbach_summary);
    // else
    //   print "only integer inputs are accepted"
    free(input->input_arr);
  }

  return main_error;
}  /// end procedure

/// procedure read_input()
input_t* read_input() {
  input_t* input = (input_t*) calloc(1, sizeof(input_t));  /// 

  int* input_arr = (int*) calloc(1, sizeof(int));
  int count = 0;

  FILE* f_input = stdin;  /// file's input
  int value = 0;
  while((fscanf(f_input, "%d", &value)) == 1) {
      input_arr[count] = value;
      count++;
      input_arr = (int*) realloc(input_arr, sizeof(int) * (count + 1));
  }

  input->input_arr = input_arr;
  input->size = count;

  // return stdin()
  return input;
} /// end procedure

/// procedure calc_goldbach(input_arr)
goldbach_summary_t* calc_goldbach(input_t* input) {
  ///  goldbach_summary = allocate memory
  goldbach_summary_t* goldbach_summary = (goldbach_summary_t*) calloc(1, sizeof(goldbach_summary_t));

  ///  goldbach_summary->numbers = length(input_arr)
  goldbach_summary->numbers = input->size;

  ///  goldbach_sum = allocate memory
  goldbach_summary->goldbach_sums = (goldbach_sum_t*) calloc(goldbach_summary->numbers, sizeof(goldbach_sum_t));
  goldbach_summary->goldbach_sums->prime_summands = (int**) calloc(goldbach_summary->numbers, sizeof(int*));
  goldbach_sum_t* goldbach_sums = goldbach_summary->goldbach_sums;
  // for el in input_arr
  for (int i = 0; i < goldbach_summary->numbers; i++) {
    ///   goldbach_sum->number = el
    goldbach_sums[i].number = input->input_arr[i];

    goldbach_sums[i].invalid = 0;   /// asumo valido al principio

    ///   if is_even(el)
    if (goldbach_sums[i].number % 2 == 0) {
      ///     goldbach_sum->even_or_odd = 1  /// Codigo de even
      goldbach_sums[i].even_or_odd = 1;
      ///     (goldbach_summary->goldbach_sum).add(even_goldbach(el, goldbach_sum))
      // goldbach_sums[i].prime_summands[i] = even_goldbach(goldbach_summary->goldbach_sums[i].number);
      // if (goldbach_sums[i].prime_summands[i][0] == 0 && goldbach_sums[i].prime_summands[i][1] == 0) {
        // goldbach_sums[i].invalid = 1;
      // }
      ///   else
    } else {
      ///     goldbach_sum->even_or_odd = 0  /// Codigo de odd
      goldbach_sums[i].even_or_odd = 0;
      ///     (goldbach_summary->goldbach_sum).add(odd_goldbach(el, goldbach_sum))
      // goldbach_sums[i].prime_summands[i] = odd_goldbach(goldbach_summary->goldbach_sums[i].number);
      // if (goldbach_sums[i].prime_summands[i][0] == 0 && goldbach_sums[i].prime_summands[i][1] == 0 && goldbach_sums[i].prime_summands[i][2] == 0) {
        // goldbach_sums[i].invalid = 1;
      // }
    }
  }
  return goldbach_summary;
}  /// end procedure

/// procedure even_goldbach(el)
int* even_goldbach(int number) {
///   prime_nums = calc_minor_primes(el)

///   for p_num in prime_nums
///     for p_num2 in prime_nums
///       if p_num + p_num2 = el
///         goldbach_sum->prime_summands.add([p_num, p_num2, 0])
        
///   for el in goldbach_sum->prime_summands[0]
///     if el[0] > el[1]

///   return goldbach_sum
  int* temp = (int*) calloc(1, sizeof(int));
  return temp;
}  /// end procedure

int* odd_goldbach(int number) {

  int* temp = (int*) calloc(1, sizeof(int));
  return temp;
} 
