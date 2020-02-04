#include "aes.h"
#include <string.h>
#include <ff.h>      // FIL, f_xxx()
#include <sds_lib.h> // sds_clock_counter()

int INPUT_SIZE = sizeof(struct bench_args_t);
float t;

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //aes256_encrypt_ecb( &(args->ctx), args->k, args->buf );

  uint8_t key[32], enckey[32], deckey[32], k[32], buf[32];
  static uint8_t aux[32], flag = 0;
  unsigned long long t0, tf;

  memcpy(key, args->ctx.key, sizeof key);
  memcpy(enckey, args->ctx.enckey, sizeof enckey);
  memcpy(deckey, args->ctx.deckey, sizeof deckey);
  memcpy(k, args->k, sizeof k);
  if (flag == 0) memcpy(aux, args->buf, sizeof args->buf);
  flag = 1;
  memcpy(buf, aux, sizeof buf);

  t0 = 2 * sds_clock_counter(); // AR# 67763
  aes256_encrypt_ecb( key, enckey, deckey, k, buf );
  tf = 2 * sds_clock_counter(); // AR# 67763
  t += (float)(tf-t0)/(sds_clock_frequency() / 1000);

  memcpy(args->buf, buf, sizeof args->buf);
}

/* Input format:
%%: Section 1
uint8_t[32]: key
%%: Section 2
uint8_t[16]: input-text
*/

void input_to_data(FIL fd, void *vdata) { // void input_to_data(int fd, void *vdata) {
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

void data_to_input(FIL fd, void *vdata) { // void data_to_input(int fd, void *vdata) {
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

void output_to_data(FIL fd, void *vdata) { // void output_to_data(int fd, void *vdata) {
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

void data_to_output(FIL fd, void *vdata) { // void data_to_output(int fd, void *vdata) {
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
