#include "sort.h"
#include <string.h>
#include <sys/time.h> // struct timeval, gettimeofday()
#include "artico3.h"

int INPUT_SIZE = sizeof(struct bench_args_t);

#define ITERATIONS (1024)
#define ACCELERATORS (1)

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ ms_mergesort( args->a );

  struct timeval t0, tf;
  float t;

  artico3_init();

  artico3_kernel_create("sort", 8192, 1, 0);

  gettimeofday(&t0, NULL);
  for (int i = 0; i < ACCELERATORS; i++) {
      artico3_load("sort", i, 0, 0, 0);
  }
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel loading : %.3f ms\n", t);

  a3data_t *a = artico3_alloc(ITERATIONS * SIZE * sizeof *a, "sort", "a", A3_P_IO);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < SIZE; j++) {
      a[i*SIZE + j] = args->a[j];
    }
  }

  gettimeofday(&t0, NULL);
  artico3_kernel_execute("sort", ITERATIONS, 1);
  artico3_kernel_wait("sort");
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel execution : %.3f ms (average : %.3f ms)\n", t, t/ITERATIONS);

  for (int j = 0; j < SIZE; j++) {
      args->a[j] = a[j];
  }

  artico3_free("sort", "a");

  artico3_kernel_release("sort");

  artico3_exit();
}

/* Input format:
%% Section 1
TYPE[SIZE]: the array
*/

void input_to_data(int fd, void *vdata) {
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

void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->a, SIZE);
}

/* Output format:
%% Section 1
TYPE[SIZE]: the array
*/

void output_to_data(int fd, void *vdata) {
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

void data_to_output(int fd, void *vdata) {
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
