/*
Implementation based on http://www-igm.univ-mlv.fr/~lecroq/string/node8.html
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "support.h"

#define PATTERN_SIZE 4
#define STRING_SIZE (8192)

//int kmp(char pattern[PATTERN_SIZE], char input[STRING_SIZE], int32_t kmpNext[PATTERN_SIZE], int32_t n_matches[1]);
int kmp(char pattern[STRING_SIZE], char input[STRING_SIZE], int32_t kmpNext[STRING_SIZE], int32_t n_matches[STRING_SIZE]);
////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t {
  char pattern[PATTERN_SIZE];
  char input[STRING_SIZE];
  int32_t kmpNext[PATTERN_SIZE];
  int32_t n_matches[1];
};
