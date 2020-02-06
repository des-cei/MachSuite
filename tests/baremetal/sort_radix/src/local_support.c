#include "sort.h"
#include <string.h>
#include <ff.h>      // FIL, f_xxx()
#include <sds_lib.h> // sds_clock_counter()

int INPUT_SIZE = sizeof(struct bench_args_t);
float t;

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ ss_sort( args->a, args->b, args->bucket, args->sum );

  int a[SIZE], a_aux[SIZE];
  int b[SIZE], b_aux[SIZE];
  int bucket[BUCKETSIZE], bucket_aux[BUCKETSIZE];
  int sum[SIZE], sum_aux[SIZE];
  static char flag = 0;
  unsigned long long t0, tf;

  if (flag == 0) {
	memcpy(a_aux, args->a, sizeof args->a);
	memcpy(b_aux, args->b, sizeof args->b);
 	memcpy(bucket_aux, args->bucket, sizeof args->bucket);
	memcpy(sum_aux, args->sum, sizeof args->sum);
  }
  flag = 1;
  memcpy(a, a_aux, sizeof a);
  memcpy(b, b_aux, sizeof b);
  memcpy(bucket, bucket_aux, sizeof bucket);
  memcpy(sum, sum_aux, sizeof sum);

  t0 = 2 * sds_clock_counter(); // AR# 67763
  ss_sort( a, b, bucket, sum );
  tf = 2 * sds_clock_counter(); // AR# 67763
  t += (float)(tf-t0)/(sds_clock_frequency() / 1000);

  memcpy(args->a, a, sizeof a);
  memcpy(args->b, b, sizeof b);
}

/* Input format:
%% Section 1
TYPE[SIZE]: unsorted array
*/

void input_to_data(FIL fd, void *vdata) { // void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->a, SIZE);
  free(p);
}

void data_to_input(FIL fd, void *vdata) { // void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->a, SIZE);
}

/* Output format:
%% Section 1
TYPE[SIZE]: sorted array
*/

void output_to_data(FIL fd, void *vdata) { // void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->a, SIZE);
  free(p);
}

void data_to_output(FIL fd, void *vdata) { // void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->a, SIZE);
}

int check_data( void *vdata, void *vref ) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  struct bench_args_t *ref = (struct bench_args_t *)vref;
  int has_errors = 0;
  int i;
  TYPE data_sum, ref_sum;

  // Check sortedness and sum
  data_sum = data->a[0];
  ref_sum = ref->a[0];
  for( i=1; i<SIZE; i++ ) {
    has_errors |= data->a[i-1] > data->a[i];
    data_sum += data->a[i];
    ref_sum += ref->a[i];
  }
  has_errors |= (data_sum!=ref_sum);

  // Return true if it's correct.
  return !has_errors;
}
