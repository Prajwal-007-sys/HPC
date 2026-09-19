#include<stdio.h>
#include<omp.h>

int main() {
    int sum = 0;

    #pragma omp parallel for num_threads(4)
    for(int i = 0; i < 10; i++) {
        int local_sum = omp_get_thread_num();

        //#pragma omp critical
        printf("Thread %d adding %d to sum\n", omp_get_thread_num(), local_sum);
        sum += local_sum;
    }
    printf("Total sum: %d\n", sum);
}