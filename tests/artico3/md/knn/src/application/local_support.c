#include "md.h"
#include <string.h>
#include <sys/time.h> // struct timeval, gettimeofday()
#include "artico3.h"

int INPUT_SIZE = sizeof(struct bench_args_t);

#define EPSILON ((TYPE)1.0e-3) //((TYPE)1.0e-6)

#define ITERATIONS (1024)
#define ACCELERATORS (1)

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ md_kernel( args->force_x, args->force_y, args->force_z,
             //~ args->position_x, args->position_y, args->position_z,
             //~ args->NL );

  struct timeval t0, tf;
  float t;

  artico3_init();

  artico3_kernel_create("md", 32768, 2, 0);

  gettimeofday(&t0, NULL);
  for (int i = 0; i < ACCELERATORS; i++) {
      artico3_load("md", i, 0, 0, 0);
  }
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel loading : %.3f ms\n", t);

  a3data_t *NL = artico3_alloc(ITERATIONS * nAtoms * maxNeighbors * sizeof *NL, "md", "NL", A3_P_I);
  a3data_t *pack = artico3_alloc(ITERATIONS * 6 * nAtoms * sizeof *pack, "md", "pack", A3_P_IO);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < (nAtoms * maxNeighbors); j++) {
      NL[i*nAtoms*maxNeighbors + j] = args->NL[j];
    }
    for (int j = 0; j < nAtoms; j++) {
      pack[i*(6*nAtoms) + (0*nAtoms) + j] = ftoa3(args->force_x[j]);
      pack[i*(6*nAtoms) + (1*nAtoms) + j] = ftoa3(args->force_y[j]);
      pack[i*(6*nAtoms) + (2*nAtoms) + j] = ftoa3(args->force_z[j]);
      pack[i*(6*nAtoms) + (3*nAtoms) + j] = ftoa3(args->position_x[j]);
      pack[i*(6*nAtoms) + (4*nAtoms) + j] = ftoa3(args->position_y[j]);
      pack[i*(6*nAtoms) + (5*nAtoms) + j] = ftoa3(args->position_z[j]);
    }
  }

  gettimeofday(&t0, NULL);
  artico3_kernel_execute("md", ITERATIONS, 1);
  artico3_kernel_wait("md");
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel execution : %.3f ms (average : %.3f ms)\n", t, t/ITERATIONS);

  for (int j = 0; j < nAtoms; j++) {
    args->force_x[j] = a3tof(pack[0*nAtoms + j]);
    args->force_y[j] = a3tof(pack[1*nAtoms + j]);
    args->force_z[j] = a3tof(pack[2*nAtoms + j]);
    args->position_x[j] = a3tof(pack[3*nAtoms + j]);
    args->position_y[j] = a3tof(pack[4*nAtoms + j]);
    args->position_z[j] = a3tof(pack[5*nAtoms + j]);
  }


  artico3_free("md", "NL");
  artico3_free("md", "pack");

  artico3_kernel_release("md");

  artico3_exit();
}

/* Input format:
%% Section 1
TYPE[nAtoms]: x positions
%% Section 2
TYPE[nAtoms]: y positions
%% Section 3
TYPE[nAtoms]: z positions
%% Section 4
int32_t[nAtoms*maxNeighbors]: neighbor list
*/

void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->position_x, nAtoms);

  s = find_section_start(p,2);
  STAC(parse_,TYPE,_array)(s, data->position_y, nAtoms);

  s = find_section_start(p,3);
  STAC(parse_,TYPE,_array)(s, data->position_z, nAtoms);

  s = find_section_start(p,4);
  parse_int32_t_array(s, data->NL, nAtoms*maxNeighbors);
  free(p);
}

void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->position_x, nAtoms);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->position_y, nAtoms);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->position_z, nAtoms);

  write_section_header(fd);
  write_int32_t_array(fd, data->NL, nAtoms*maxNeighbors);

}

/* Output format:
%% Section 1
TYPE[nAtoms]: new x force
%% Section 2
TYPE[nAtoms]: new y force
%% Section 3
TYPE[nAtoms]: new z force
*/

void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, data->force_x, nAtoms);

  s = find_section_start(p,2);
  STAC(parse_,TYPE,_array)(s, data->force_y, nAtoms);

  s = find_section_start(p,3);
  STAC(parse_,TYPE,_array)(s, data->force_z, nAtoms);
  free(p);
}

void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->force_x, nAtoms);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->force_y, nAtoms);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, data->force_z, nAtoms);
}

int check_data( void *vdata, void *vref ) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  struct bench_args_t *ref = (struct bench_args_t *)vref;
  int has_errors = 0;
  int i;
  TYPE diff_x, diff_y, diff_z;

  for( i=0; i<nAtoms; i++ ) {
    diff_x = data->force_x[i] - ref->force_x[i];
    diff_y = data->force_y[i] - ref->force_y[i];
    diff_z = data->force_z[i] - ref->force_z[i];
    has_errors |= (diff_x<-EPSILON) || (EPSILON<diff_x);
    has_errors |= (diff_y<-EPSILON) || (EPSILON<diff_y);
    has_errors |= (diff_z<-EPSILON) || (EPSILON<diff_z);
  }

  // Return true if it's correct.
  return !has_errors;
}
