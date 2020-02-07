#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <ff.h>      // FIL, f_xxx()

#define WRITE_OUTPUT
#define CHECK_OUTPUT

#include "support.h"

#define ITERATIONS (1024)
extern float t;

int main(int argc, char **argv)
{
  // Parse command line.
  char *in_file;
  #ifdef CHECK_OUTPUT
  char *check_file;
  #endif
  assert( argc<4 && "Usage: ./benchmark <input_file> <check_file>" );
  in_file = "viterbi/viterbi/input.dat"; // in_file = "input.data";
  #ifdef CHECK_OUTPUT
  check_file = "viterbi/viterbi/check.dat"; // check_file = "check.data";
  #endif
  if( argc>1 )
    in_file = argv[1];
  #ifdef CHECK_OUTPUT
  if( argc>2 )
    check_file = argv[2];
  #endif

  // Mount FatFs
  FATFS fs;
  FRESULT fres;
  fres = f_mount(&fs, "", 0);
  assert( fres==0 && "Couldn't mount filesystem" );

  // Load input data
  FIL in_fd; // int in_fd;
  char *data;
  data = malloc(INPUT_SIZE);
  assert( data!=NULL && "Out of memory" );
  fres = f_open(&in_fd, in_file, FA_READ); // in_fd = open( in_file, O_RDONLY );
  assert( fres==FR_OK && "Couldn't open input data file" ); // assert( in_fd>0 && "Couldn't open input data file" );
  input_to_data(in_fd, data);
  f_close(&in_fd);

  // Unpack and call
  t = 0;
  for (int i = 0; i < ITERATIONS; i++) run_benchmark( data );
  printf("<VITERBI/VITERBI> Elapsed time : %.3f ms (average : %.3f ms)\n", t, t/ITERATIONS);

  #ifdef WRITE_OUTPUT
  FIL out_fd; // int out_fd;
  fres = f_open(&out_fd, "viterbi/viterbi/output.dat", FA_WRITE | FA_CREATE_ALWAYS); // out_fd = open("output.data", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
  assert( fres==FR_OK && "Couldn't open output data file" ); // assert( out_fd>0 && "Couldn't open output data file" );
  data_to_output(out_fd, data);
  f_close(&out_fd); // close(out_fd);
  #endif

  // Load check data
  #ifdef CHECK_OUTPUT
  FIL check_fd; // int check_fd;
  char *ref;
  ref = malloc(INPUT_SIZE);
  assert( ref!=NULL && "Out of memory" );
  fres = f_open(&check_fd, check_file, FA_READ); // check_fd = open( check_file, O_RDONLY );
  assert( fres==FR_OK && "Couldn't open check data file" ); // assert( check_fd>0 && "Couldn't open check data file" );
  output_to_data(check_fd, ref);
  f_close(&check_fd);
  #endif

  // Validate benchmark results
  #ifdef CHECK_OUTPUT
  if( !check_data(data, ref) ) {
    fprintf(stderr, "Benchmark results are incorrect\n");
    return -1;
  }
  free(ref);
  #endif
  free(data);

  // Unmount FatFs
  f_mount(NULL, "", 0);

  printf("Success.\n");
  return 0;
}
