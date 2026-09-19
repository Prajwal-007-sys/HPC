#include<stdio.h>
#include<omp.h>

int main() {
    int i, n = 10;
    int a[10];
    
    #pragma omp parallel for num_threads(4)
    for(i = 0; i < n; i++) {
        a[i] = i * i;
        printf("Thread %d computed a[%d] = %d\n", omp_get_thread_num(), i, a[i]);
    }
    return 0;
}