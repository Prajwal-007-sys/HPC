#include<stdio.h>
#include<omp.h>

#define N 1000
#define CHUNK_SIZE 4

void* Static(){
    printf("Static scheduling\n");
    #pragma omp parallel for schedule(static, CHUNK_SIZE)
    for(int i = 0; i < N; i++) {
        printf("Thread %d processing iteration %d\n", omp_get_thread_num(), i);
    }
}

void* Dynamic(){
    printf("Dynamic scheduling\n");
    #pragma omp parallel for schedule(dynamic, CHUNK_SIZE)
    for(int i = 0; i < N; i++) {
        printf("Thread %d processing iteration %d\n", omp_get_thread_num(), i);
    }
}

void* Guided(){
    printf("Guided scheduling\n");
    #pragma omp parallel for schedule(guided, CHUNK_SIZE)
    for(int i = 0; i < N; i++) {
        printf("Thread %d processing iteration %d\n", omp_get_thread_num(), i);
    }
}

void* Runtime(){
    printf("Runtime scheduling\n");
    #pragma omp parallel for schedule(runtime)
    for(int i = 0; i < N; i++) {
        printf("Thread %d processing iteration %d\n", omp_get_thread_num(), i);
    }
}

void* Auto(){
    printf("Auto scheduling\n");
    #pragma omp parallel for schedule(auto)
    for(int i = 0; i < N; i++) {
        printf("Thread %d processing iteration %d\n", omp_get_thread_num(), i);
    }
}

int main() {

    Static();
    Dynamic();
    Guided();
    Runtime();
    Auto();
    return 0;
}