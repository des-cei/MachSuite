#include "spmv.h"
#include <string.h>
#include <sys/time.h> // struct timeval, gettimeofday()
#include "artico3.h"

int INPUT_SIZE = sizeof(struct bench_args_t);

#define EPSILON 1.0e-3 //1.0e-6

#define ITERATIONS (1024)
#define ACCELERATORS (1)

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ ellpack( args->nzval, args->cols, args->vec, args->out );

  struct timeval t0, tf;
  float t;

  artico3_init();

  artico3_kernel_create("spmv", 59280, 1, 0);

  gettimeofday(&t0, NULL);
  for (int i = 0; i < ACCELERATORS; i++) {
      artico3_load("spmv", i, 0, 0, 0);
  }
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel loading : %.3f ms\n", t);

  a3data_t *pack = artico3_alloc(ITERATIONS * (2*N*L + 2*N) * sizeof *pack, "spmv", "pack", A3_P_IO);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < N*L; j++) {
      pack[i*(2*N*L + 2*N) + j] = ftoa3(args->nzval[j]);
      pack[i*(2*N*L + 2*N) + (N*L) + j] = args->cols[j];
    }
    for (int j = 0; j < N; j++) {
      pack[i*(2*N*L + 2*N) + (2*N*L) + j] = ftoa3(args->vec[j]);
      pack[i*(2*N*L + 2*N) + (2*N*L + N) + j] = ftoa3(args->out[j]);
    }
  }

  gettimeofday(&t0, NULL);
  artico3_kernel_execute("spmv", ITERATIONS, 1);
  artico3_kernel_wait("spmv");
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel execution : %.3f ms (average : %.3f ms)\n", t, t/ITERATIONS);

  for (int j = 0; j < N; j++) {
    args->out[j] = a3tof(pack[(2*N*L + N) + j]);
  }

  artico3_free("spmv", "pack");

  artico3_kernel_release("spmv");

  artico3_exit();
}

/* Input format:
%% Section 1
TYPE[N*L]: the nonzeros of the matrix
%% Section 2
int32_t[N*L]: the column index of the nonzeros
%% Section 3
TYPE[N]: the dense vector
*/

void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->nzval, N*L);

  s = find_section_start(p,2);
  parse_int32_t_array(s, data->cols, N*L);

  s = find_section_start(p,3);
  STAC(parse_,TYPE,_array)(s, data->vec, N);
  free(p);
}

void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->nzval, N*L);

  write_section_header(fd);
  write_int32_t_array(fd, data->cols, N*L);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->vec, N);
}

/* Output format:
%% Section 1
TYPE[N]: The output vector
*/

void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->out, N);
  free(p);
}

void data_to_output(int fd, void *vdata) {
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
