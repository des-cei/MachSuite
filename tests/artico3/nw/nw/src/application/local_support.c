#include "nw.h"
#include <string.h>
#include <sys/time.h> // struct timeval, gettimeofday()
#include "artico3.h"

int INPUT_SIZE = sizeof(struct bench_args_t);

#define ITERATIONS (1024)
#define ACCELERATORS (1)

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ needwun( args->seqA, args->seqB, args->alignedA, args->alignedB, args->M, args->ptr);

  struct timeval t0, tf;
  float t;

  artico3_init();

  artico3_kernel_create("nw", 49152, 3, 0);

  gettimeofday(&t0, NULL);
  for (int i = 0; i < ACCELERATORS; i++) {
      artico3_load("nw", i, 0, 0, 0);
  }
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel loading : %.3f ms\n", t);

  a3data_t *M = artico3_alloc(ITERATIONS * (ALEN+1) * (BLEN+1) * sizeof *M, "nw", "M", A3_P_I);
  a3data_t *ptr = artico3_alloc(ITERATIONS * (ALEN+1) * (BLEN+1) * sizeof *ptr, "nw", "ptr", A3_P_I);
  a3data_t *pack = artico3_alloc(ITERATIONS * (3*ALEN + 3*BLEN) * sizeof *pack, "nw", "pack", A3_P_IO);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < ((ALEN+1) * (BLEN+1)); j++) {
        M[i*(ALEN+1)*(BLEN+1) + j] = args->M[j];
        ptr[i*(ALEN+1)*(BLEN+1) + j] = args->ptr[j];
    }
    for (int j = 0; j < ALEN; j++) {
      pack[i*(3*ALEN + 3*BLEN) + j] = args->seqA[j];
    }
    for (int j = 0; j < BLEN; j++) {
      pack[i*(3*ALEN + 3*BLEN) + ALEN + j] = args->seqB[j];
    }
  }

  gettimeofday(&t0, NULL);
  artico3_kernel_execute("nw", ITERATIONS, 1);
  artico3_kernel_wait("nw");
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel execution : %.3f ms (average : %.3f ms)\n", t, t/ITERATIONS);

  for (int j = 0; j < (ALEN + BLEN); j++) {
      args->alignedA[j] = pack[(ALEN + BLEN) + j];
      args->alignedB[j] = pack[(2*ALEN + 2*BLEN) + j];
  }

  artico3_free("nw", "M");
  artico3_free("nw", "ptr");
  artico3_free("nw", "pack");

  artico3_kernel_release("nw");

  artico3_exit();
}

/* Input format:
%% Section 1
char[]: sequence A
%% Section 2
char[]: sequence B
*/

void input_to_data(int fd, void *vdata) {
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

void data_to_input(int fd, void *vdata) {
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

void output_to_data(int fd, void *vdata) {
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

void data_to_output(int fd, void *vdata) {
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
