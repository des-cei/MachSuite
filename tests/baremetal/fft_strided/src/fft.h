#include <stdio.h>
#include <stdlib.h>
#include "support.h"

#define FFT_SIZE 1024
#define twoPI 6.28318530717959

//void fft(double real[FFT_SIZE], double img[FFT_SIZE], double real_twid[FFT_SIZE/2], double img_twid[FFT_SIZE/2]);
void fft(float real[FFT_SIZE], float img[FFT_SIZE], float real_twid[FFT_SIZE], float img_twid[FFT_SIZE]);


////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t {
        double real[FFT_SIZE];
        double img[FFT_SIZE];
        double real_twid[FFT_SIZE/2];
        double img_twid[FFT_SIZE/2];
};
