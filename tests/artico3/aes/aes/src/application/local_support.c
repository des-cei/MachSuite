#include "aes.h"
#include <string.h>
#include <sys/time.h> // struct timeval, gettimeofday()
#include "artico3.h"

int INPUT_SIZE = sizeof(struct bench_args_t);

#define ITERATIONS (1024)
#define ACCELERATORS (1)

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ aes256_encrypt_ecb( &(args->ctx), args->k, args->buf );

  struct timeval t0, tf;
  float t;

  artico3_init();

  artico3_kernel_create("aes", 640, 5, 0);

  gettimeofday(&t0, NULL);
  for (int i = 0; i < ACCELERATORS; i++) {
      artico3_load("aes", i, 0, 0, 0);
  }
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel loading : %.3f ms\n", t);

  a3data_t *key = artico3_alloc(ITERATIONS * 32 * sizeof *key, "aes", "key", A3_P_I);
  a3data_t *enckey = artico3_alloc(ITERATIONS * 32 * sizeof *enckey, "aes", "enckey", A3_P_I);
  a3data_t *deckey = artico3_alloc(ITERATIONS * 32 * sizeof *deckey, "aes", "deckey", A3_P_I);
  a3data_t *k = artico3_alloc(ITERATIONS * 32 * sizeof *k, "aes", "k", A3_P_I);
  a3data_t *buf = artico3_alloc(ITERATIONS * 16 * sizeof *buf, "aes", "buf", A3_P_IO);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < 32; j++) {
      k[i*32 + j] = args->k[j];
    }
    for (int j = 0; j < 16; j++) {
      buf[i*16 + j] = args->buf[j];
    }
  }

  gettimeofday(&t0, NULL);
  artico3_kernel_execute("aes", ITERATIONS, 1);
  artico3_kernel_wait("aes");
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel execution : %.3f ms (average : %.3f ms)\n", t, t/ITERATIONS);

  for (int j = 0; j < 16; j++) {
      args->buf[j] = buf[j];
  }

  artico3_free("aes", "key");
  artico3_free("aes", "enckey");
  artico3_free("aes", "deckey");
  artico3_free("aes", "k");
  artico3_free("aes", "buf");

  artico3_kernel_release("aes");

  artico3_exit();
}

/* Input format:
%%: Section 1
uint8_t[32]: key
%%: Section 2
uint8_t[16]: input-text
*/

void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);
  // Section 1: key
  s = find_section_start(p,1);
  parse_uint8_t_array(s, data->k, 32);
  // Section 2: input-text
  s = find_section_start(p,2);
  parse_uint8_t_array(s, data->buf, 16);
  free(p);
}

void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  // Section 1
  write_section_header(fd);
  write_uint8_t_array(fd, data->k, 32);
  // Section 2
  write_section_header(fd);
  write_uint8_t_array(fd, data->buf, 16);
}

/* Output format:
%% Section 1
uint8_t[16]: output-text
*/

void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);
  // Section 1: output-text
  s = find_section_start(p,1);
  parse_uint8_t_array(s, data->buf, 16);
  free(p);
}

void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  // Section 1
  write_section_header(fd);
  write_uint8_t_array(fd, data->buf, 16);
}

int check_data( void *vdata, void *vref ) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  struct bench_args_t *ref = (struct bench_args_t *)vref;
  int has_errors = 0;

  // Exact compare encrypted output buffers
  has_errors |= memcmp(&data->buf, &ref->buf, 16*sizeof(uint8_t));

  // Return true if it's correct.
  return !has_errors;
}
