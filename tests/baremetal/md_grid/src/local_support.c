#include "md.h"
#include <string.h>
#include <ff.h>      // FIL, f_xxx()
#include <sds_lib.h> // sds_clock_counter()

int INPUT_SIZE = sizeof(struct bench_args_t);

#define EPSILON ((TYPE)1.0e-3) //((TYPE)1.0e-6)
float t;

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //md( args->n_points, args->force, args->position );

  int32_t n_points[blockSide][blockSide][blockSide];
  TYPE force_x[blockSide][blockSide][blockSide][densityFactor];
  TYPE force_y[blockSide][blockSide][blockSide][densityFactor];
  TYPE force_z[blockSide][blockSide][blockSide][densityFactor];
  TYPE position_x[blockSide][blockSide][blockSide][densityFactor];
  TYPE position_y[blockSide][blockSide][blockSide][densityFactor];
  TYPE position_z[blockSide][blockSide][blockSide][densityFactor];
  unsigned long long t0, tf;

  memcpy(n_points, args->n_points, sizeof n_points);
  for (int i = 0; i < blockSide; i++) {
    for (int j = 0; j < blockSide; j++) {
      for (int k = 0; k < blockSide; k++) {
        for (int l = 0; l < densityFactor; l++) {
          position_x[i][j][k][l] = args->position[i][j][k][l].x;
          position_y[i][j][k][l] = args->position[i][j][k][l].y;
          position_z[i][j][k][l] = args->position[i][j][k][l].z;
        }
      }
    }
  }
  memset(force_x, 0, sizeof force_x);
  memset(force_y, 0, sizeof force_y);
  memset(force_z, 0, sizeof force_z);

  t0 = 2 * sds_clock_counter(); // AR# 67763
  md( n_points, force_x, force_y, force_z, position_x, position_y, position_z );
  tf = 2 * sds_clock_counter(); // AR# 67763
  t += (float)(tf-t0)/(sds_clock_frequency() / 1000);

  for (int i = 0; i < blockSide; i++) {
    for (int j = 0; j < blockSide; j++) {
      for (int k = 0; k < blockSide; k++) {
        for (int l = 0; l < densityFactor; l++) {
          args->force[i][j][k][l].x = force_x[i][j][k][l];
          args->force[i][j][k][l].y = force_y[i][j][k][l];
          args->force[i][j][k][l].z = force_z[i][j][k][l];
        }
      }
    }
  }
}

/* Input format:
%% Section 1
int32_t[blockSide^3]: grid populations
%% Section 2
TYPE[blockSide^3*densityFactor]: positions
*/

void input_to_data(FIL fd, void *vdata) { // void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  parse_int32_t_array(s, (int32_t *)(data->n_points), blockSide*blockSide*blockSide);

  s = find_section_start(p,2);
  STAC(parse_,TYPE,_array)(s, (double *)(data->position), 3*blockSide*blockSide*blockSide*densityFactor);
  free(p);
}

void data_to_input(FIL fd, void *vdata) { // void data_to_input(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  write_int32_t_array(fd, (int32_t *)(data->n_points), blockSide*blockSide*blockSide);

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, (double *)(data->position), 3*blockSide*blockSide*blockSide*densityFactor);

}

/* Output format:
%% Section 1
TYPE[blockSide^3*densityFactor]: force
*/

void output_to_data(FIL fd, void *vdata) { // void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);

  s = find_section_start(p,1);
  STAC(parse_,TYPE,_array)(s, (double *)data->force, 3*blockSide*blockSide*blockSide*densityFactor);
  free(p);
}

void data_to_output(FIL fd, void *vdata) { // void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;

  write_section_header(fd);
  STAC(write_,TYPE,_array)(fd, (double *)data->force, 3*blockSide*blockSide*blockSide*densityFactor);
}

int check_data( void *vdata, void *vref ) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  struct bench_args_t *ref = (struct bench_args_t *)vref;
  int has_errors = 0;
  int i, j, k, d;
  TYPE diff_x, diff_y, diff_z;

  for(i=0; i<blockSide; i++) {
    for(j=0; j<blockSide; j++) {
      for(k=0; k<blockSide; k++) {
        for(d=0; d<densityFactor; d++) {
          diff_x = data->force[i][j][k][d].x - ref->force[i][j][k][d].x;
          diff_y = data->force[i][j][k][d].y - ref->force[i][j][k][d].y;
          diff_z = data->force[i][j][k][d].z - ref->force[i][j][k][d].z;
          has_errors |= (diff_x<-EPSILON) || (EPSILON<diff_x);
          has_errors |= (diff_y<-EPSILON) || (EPSILON<diff_y);
          has_errors |= (diff_z<-EPSILON) || (EPSILON<diff_z);
        }
      }
    }
  }

  // Return true if it's correct.
  return !has_errors;
}
