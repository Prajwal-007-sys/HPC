#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "omp.h"

#define MAX_RAND 32767

int main()
{
    long npoints = 1000000;
    long sum = 0;
    double pi = 0;

    #pragma omp parallel default (private) shared(npoints) reduction(+: sum) num_threads(8)
    {
        int thread_id;
        int num_threads;
        long sample_points_per_thread;
        long i;
        double rand_no_x, rand_no_y;
        unsigned int seed;

        thread_id = omp_get_thread_num();
        seed = (unsigned int)time(NULL) ^ (thread_id + 1)

        num_threads = omp_get_num_threads();
        sample_points_per_thread = npoints / num_threads;

        for(int i = 0 ; i < sample_points_per_thread ; i++){
            rand_no_x = (double)rand_r(&seed) / MAX_RAND;
            rand_no_y = (double)rand_r(&seed) / MAX_RAND;

            double dx = rand_no_x - 0.5;
            double dy = rand_no_y - 0.5;

            if((dx * dy + dy * dy) < 0.25) {
                sum ++;
            }
        }

        pi = 4 * (sum / npoints)


    }

}
