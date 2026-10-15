#include <stdio.h>
#include <omp.h>

int main() {
    double start_time, end_time, elapsed_time_1, elapsed_time_2;
    int n, i;

    printf("Enter n: \n");
    if (scanf("%d", &n) != 1) return 1;

    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(static)
    for (i = 0; i < n; i++) {
        int temp = i * 2;
    }
    end_time = omp_get_wtime();
    elapsed_time_1 = end_time - start_time;

    start_time = omp_get_wtime(); 
    #pragma omp parallel for schedule(static)
    for (i = 0; i < n; i++) {
        int temp = i * 2;
    }
    end_time = omp_get_wtime();
    elapsed_time_2 = end_time - start_time;

    printf("Elapsed time 1: %f seconds\n", elapsed_time_1);
    printf("Elapsed time 2: %f seconds\n", elapsed_time_2);

    return 0;
}