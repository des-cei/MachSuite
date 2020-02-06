#include "viterbi.h"
#include <string.h>
#include <sys/time.h> // struct timeval, gettimeofday()
#include "artico3.h"

int INPUT_SIZE = sizeof(struct bench_args_t);

#define ITERATIONS (1024)
#define ACCELERATORS (1)

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ viterbi( args->obs, args->init, args->transition, args->emission, args->path );

  struct timeval t0, tf;
  float t;

  artico3_init();

  artico3_kernel_create("viterbi", 49152, 3, 0);

  gettimeofday(&t0, NULL);
  for (int i = 0; i < ACCELERATORS; i++) {
      artico3_load("viterbi", i, 0, 0, 0);
  }
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel loading : %.3f ms\n", t);

  a3data_t *transition = artico3_alloc(ITERATIONS * N_STATES * N_STATES * sizeof *transition, "viterbi", "transition", A3_P_I);
  a3data_t *emission = artico3_alloc(ITERATIONS * N_STATES * N_TOKENS * sizeof *emission, "viterbi", "emission", A3_P_I);
  a3data_t *pack = artico3_alloc(ITERATIONS * (2*N_OBS + N_STATES) * sizeof *pack, "viterbi", "pack", A3_P_IO);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < (N_STATES * N_STATES); j++) {
      transition[i*N_STATES*N_STATES + j] = ftoa3(args->transition[j]);
    }
    for (int j = 0; j < (N_STATES * N_TOKENS); j++) {
      emission[i*N_STATES*N_TOKENS + j] = ftoa3(args->emission[j]);
    }
    for (int j = 0; j < N_OBS; j++) {
      pack[i*(2*N_OBS + N_STATES) + j] = args->obs[j];
    }
    for (int j = 0; j < N_STATES; j++) {
      pack[i*(2*N_OBS + N_STATES) + N_OBS + j] = ftoa3(args->init[j]);
    }
  }

  gettimeofday(&t0, NULL);
  artico3_kernel_execute("viterbi", ITERATIONS, 1);
  artico3_kernel_wait("viterbi");
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel execution : %.3f ms (average : %.3f ms)\n", t, t/ITERATIONS);

  for (int j = 0; j < N_OBS; j++) {
    args->path[j] = pack[(N_OBS + N_STATES) + j];
  }

  artico3_free("viterbi", "transition");
  artico3_free("viterbi", "emission");
  artico3_free("viterbi", "pack");

  artico3_kernel_release("viterbi");

  artico3_exit();
}

/* Input format:
%% Section 1
tok_t[N_OBS]: observation vector
%% Section 2
prob_t[N_STATES]: initial state probabilities
%% Section 3
prob_t[N_STATES*N_STATES]: transition matrix
%% Section 4
prob_t[N_STATES*N_TOKENS]: emission matrix
*/

void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  parse_uint8_t_array(s, data->obs, N_OBS);

  s = find_section_start(p,2);
  STAC(parse_,TYPE,_array)(s, data->init, N_STATES);

  s = find_section_start(p,3);
  STAC(parse_,TYPE,_array)(s, data->transition, N_STATES*N_STATES);

  s = find_section_start(p,4);
  STAC(parse_,TYPE,_array)(s, data->emission, N_STATES*N_TOKENS);
  free(p);
}

void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  write_uint8_t_array(fd, data->obs, N_OBS);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->init, N_STATES);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->transition, N_STATES*N_STATES);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->emission, N_STATES*N_TOKENS);
}

/* Output format:
%% Section 1
uint8_t[N_OBS]: most likely state chain
*/

void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  parse_uint8_t_array(s, data->path, N_OBS);
  free(p);
}

void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  write_uint8_t_array(fd, data->path, N_OBS);
}

int check_data( void *vdata, void *vref ) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  struct bench_args_t *ref = (struct bench_args_t *)vref;
  int has_errors = 0;
  int i;

  for(i=0; i<N_OBS; i++) {
    has_errors |= (data->path[i]!=ref->path[i]);
  }

  // Return true if it's correct.
  return !has_errors;
}
