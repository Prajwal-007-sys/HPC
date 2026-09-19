#include<stdio.h>
#include<omp.h>

int main ()
{
    int x = 10;
    #pragma omp parallel for lastprivate(x)
    for(int i = 0; i < 4; i++) {
        x = x + omp_get_thread_num();
        printf("Thread %d: x = %d\n: Iteration %d", omp_get_thread_num(), x, i);
    }
    printf("Final value of x after the loop: %d\n", x);
    return 0;
}