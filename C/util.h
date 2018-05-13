#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#ifdef _OPENMP
#include <omp.h>
#else
#include <sys/time.h>
#endif
//
// // define VERBOSE if you want to print info about work groups sizes
// //#define  VERBOSE 1
// #ifdef VERBOSE
// extern int err_code(cl_int);
// #endif

int OutputDeviceInfo(cl_device_id device_id);

char *LoadKernelProgram(const char *filename);

double wtime(void);
