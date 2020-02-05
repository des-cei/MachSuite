#include "stencil.h"
#include <string.h>
#include <sys/time.h> // struct timeval, gettimeofday()
#include "artico3.h"

int INPUT_SIZE = sizeof(struct bench_args_t);

#define EPSILON (1.0e-6)

#define ITERATIONS (1024)
#define ACCELERATORS (1)

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ stencil3d( args->C, args->orig, args->sol );

  struct timeval t0, tf;
  float t;

  artico3_init();

  artico3_kernel_create("stencil", 49152, 3, 0);

  gettimeofday(&t0, NULL);
  for (int i = 0; i < ACCELERATORS; i++) {
      artico3_load("stencil", i, 0, 0, 0);
  }
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel loading : %.3f ms\n", t);

  a3data_t *orig = artico3_alloc(ITERATIONS * SIZE * sizeof *orig, "stencil", "orig", A3_P_I);
  a3data_t *sol = artico3_alloc(ITERATIONS * SIZE * sizeof *sol, "stencil", "sol", A3_P_O);
  a3data_t *C = artico3_alloc(ITERATIONS * 2 * sizeof *C, "stencil", "C", A3_P_I);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < SIZE; j++) {
      orig[i*SIZE + j] = args->orig[j];
    }
    for (int j = 0; j < 2; j++) {
      C[i*2 + j] = args->C[j];
    }
  }

  gettimeofday(&t0, NULL);
  artico3_kernel_execute("stencil", ITERATIONS, 1);
  artico3_kernel_wait("stencil");
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel execution : %.3f ms (average : %.3f ms)\n", t, t/ITERATIONS);

  for (int j = 0; j < SIZE; j++) {
    args->sol[j] = sol[j];
  }

  artico3_free("stencil", "orig");
  artico3_free("stencil", "sol");
  artico3_free("stencil", "C");

  artico3_kernel_release("stencil");

  artico3_exit();
}

/* Input format:
%% Section 1
TYPE[2]: stencil coefficients (inner/outer)
%% Section 2
TYPE[SIZE]: input matrix
*/

void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->C, 2);

  s = find_section_start(p,2);
  STAC(parse_,TYPE,_array)(s, data->orig, SIZE);
  free(p);
}

void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->C, 2);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->orig, SIZE);
}

/* Output format:
%% Section 1
TYPE[SIZE]: solution matrix
*/

void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->sol, SIZE);
  free(p);
}

void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->sol, SIZE);
}

int check_data( void *vdata, void *vref ) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  struct bench_args_t *ref = (struct bench_args_t *)vref;
  int has_errors = 0;
  int i;
  TYPE diff;

  for(i=0; i<SIZE; i++) {
    diff = data->sol[i] - ref->sol[i];
    has_errors |= (diff<-EPSILON) || (EPSILON<diff);
  }

  // Return true if it's correct.
  return !has_errors;
}
