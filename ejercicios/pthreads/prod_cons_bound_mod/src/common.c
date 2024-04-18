// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include <stdlib.h>

#include "common.h"
#include "sys/time.h"

useconds_t random_between(useconds_t min, useconds_t max) {
  unsigned int seed = 420;
  return min + (max > min ? (rand_r(&seed) % (max - min)) : 0);
}
