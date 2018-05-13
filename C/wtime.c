#ifdef _OPENMP
#include <omp.h>
#else
#include <sys/time.h>
#endif

#include <stdlib.h>

double wtime()
{
#ifdef _OPENMP
    /* Use omp_get_wtime() if we can */
    return omp_get_wtime();
#else
    /* Use a generic timer */
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
#endif
}
