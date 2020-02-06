#include "nw.h"
#include <string.h>
#include <ff.h>      // FIL, f_xxx()
#include <sds_lib.h> // sds_clock_counter()

int INPUT_SIZE = sizeof(struct bench_args_t);
float t;

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ needwun( args->seqA, args->seqB, args->alignedA, args->alignedB, args->M, args->ptr);

  char seqA[(ALEN+1)*(BLEN+1)];
  char seqB[(ALEN+1)*(BLEN+1)];
  char alignedA[(ALEN+1)*(BLEN+1)];
  char alignedB[(ALEN+1)*(BLEN+1)];
  int M[(ALEN+1)*(BLEN+1)];
  char ptr[(ALEN+1)*(BLEN+1)];
  unsigned long long t0, tf;

  memcpy(seqA, args->seqA, sizeof args->seqA);
  memcpy(seqB, args->seqB, sizeof args->seqB);
  memcpy(M, args->M, sizeof args->M);
  memcpy(ptr, args->ptr, sizeof args->ptr);

  t0 = 2 * sds_clock_counter(); // AR# 67763
  needwun( seqA, seqB, alignedA, alignedB, M, ptr);
  tf = 2 * sds_clock_counter(); // AR# 67763
  t += (float)(tf-t0)/(sds_clock_frequency() / 1000);

  memcpy(args->alignedA, alignedA, sizeof args->alignedA);
  memcpy(args->alignedB, alignedB, sizeof args->alignedB);
}

/* Input format:
%% Section 1
char[]: sequence A
%% Section 2
char[]: sequence B
*/

void input_to_data(FIL fd, void *vdata) { // void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  parse_string(s, data->seqA, ALEN);

  s = find_section_start(p,2);
  parse_string(s, data->seqB, BLEN);
  free(p);

}

void data_to_input(FIL fd, void *vdata) { // void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  write_string(fd, data->seqA, ALEN);

  write_section_header(fd);
  write_string(fd, data->seqB, BLEN);

  write_section_header(fd);
}

/* Output format:
%% Section 1
char[sum_size]: aligned sequence A
%% Section 2
char[sum_size]: aligned sequence B
*/

void output_to_data(FIL fd, void *vdata) { // void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  parse_string(s, data->alignedA, ALEN+BLEN);

  s = find_section_start(p,2);
  parse_string(s, data->alignedB, ALEN+BLEN);
  free(p);
}

void data_to_output(FIL fd, void *vdata) { // void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  write_string(fd, data->alignedA, ALEN+BLEN);

  write_section_header(fd);
  write_string(fd, data->alignedB, ALEN+BLEN);

  write_section_header(fd);
}

int check_data( void *vdata, void *vref ) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  struct bench_args_t *ref = (struct bench_args_t *)vref;
  int has_errors = 0;

  has_errors |= memcmp(data->alignedA, ref->alignedA, ALEN+BLEN);
  has_errors |= memcmp(data->alignedB, ref->alignedB, ALEN+BLEN);

  // Return true if it's correct.
  return !has_errors;
}
