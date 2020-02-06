#include "spmv.h"
#include <string.h>
#include <ff.h>      // FIL, f_xxx()
#include <sds_lib.h> // sds_clock_counter()

int INPUT_SIZE = sizeof(struct bench_args_t);

#define EPSILON 1.0e-3 //1.0e-6
float t;

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ spmv( args->val, args->cols, args->rowDelimiters, args->vec, args->out );

  TYPE val[NNZ];
  int32_t cols[NNZ];
  int32_t rowDelimiters[NNZ];
  TYPE vec[NNZ];
  TYPE out[NNZ];
  unsigned long long t0, tf;

  memcpy(val, args->val, sizeof args->val);
  memcpy(cols, args->cols, sizeof args->cols);
  memcpy(rowDelimiters, args->rowDelimiters, sizeof args->rowDelimiters);
  memcpy(vec, args->vec, sizeof args->vec);

  t0 = 2 * sds_clock_counter(); // AR# 67763
  spmv( val, cols, rowDelimiters, vec, out );
  tf = 2 * sds_clock_counter(); // AR# 67763
  t += (float)(tf-t0)/(sds_clock_frequency() / 1000);

  memcpy(args->out, out, sizeof args->out);
}

/* Input format:
%% Section 1
TYPE[NNZ]: the nonzeros of the matrix
%% Section 2
int32_t[NNZ]: the column index of the nonzeros
%% Section 3
int32_t[N+1]: the start of each row of nonzeros
%% Section 4
TYPE[N]: the dense vector
*/

void input_to_data(FIL fd, void *vdata) { // void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->val, NNZ);

  s = find_section_start(p,2);
  parse_int32_t_array(s, data->cols, NNZ);

  s = find_section_start(p,3);
  parse_int32_t_array(s, data->rowDelimiters, N+1);

  s = find_section_start(p,4);
  STAC(parse_,TYPE,_array)(s, data->vec, N);
  free(p);
}

void data_to_input(FIL fd, void *vdata) { // void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->val, NNZ);

  write_section_header(fd);
  write_int32_t_array(fd, data->cols, NNZ);

  write_section_header(fd);
  write_int32_t_array(fd, data->rowDelimiters, N+1);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->vec, N);
}

/* Output format:
%% Section 1
TYPE[N]: The output vector
*/

void output_to_data(FIL fd, void *vdata) { // void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->out, N);
  free(p);
}

void data_to_output(FIL fd, void *vdata) { // void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->out, N);
}

int check_data( void *vdata, void *vref ) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  struct bench_args_t *ref = (struct bench_args_t *)vref;
  int has_errors = 0;
  int i;
  TYPE diff;

  for(i=0; i<N; i++) {
    diff = data->out[i] - ref->out[i];
    has_errors |= (diff<-EPSILON) || (EPSILON<diff);
  }

  // Return true if it's correct.
  return !has_errors;
}
