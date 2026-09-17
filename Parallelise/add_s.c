#include<stdio.h>
#include<time.h>

#define N 1000000

int main() {
    struct timespec start, end;
    long long total = 0;

    long long numbers[N];
    for(long long i=0 ; i<N ; i++){
        numbers[i] = i;
    }

    clock_t s = clock();
    timespec_get(&start, TIME_UTC);
    for (long long i =0 ; i<N ; i++){
        total += numbers[i];
    }
    timespec_get(&end, TIME_UTC);
    clock_t e = clock();

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double cpu_time_used = ((double)(e - s)) / CLOCKS_PER_SEC;
    printf("CPU time used: %.6f seconds\n", cpu_time_used);
    printf("Elapsed time: %.6f seconds\n", elapsed);
    printf("%lld", total);
    return 0;
}

// Possible errors on large scale inputs
// integer overflow negative/incorrect totals
// stack overflow causing segmentation fault - On Linux, the default thread stack size is typically 8 MB.
// For N = 3,000,000, an int array requires approx 12 MB