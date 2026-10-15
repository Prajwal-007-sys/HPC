#include<stdio.h>
#include<omp.h>

void func1() {
    int v[100];
    int indices[100];
    int n = 100;

    #pragma omp parallel for default(none) shared(v, indices, n)
    for(int i = 0; i < n; i++) {
        v[indices[i]] += i;
    }
}

void func2() {
    int v[100];
    int indices[100];
    int n = 100;

    #pragma omp parallel for default(none) shared(v, indices, n)
    for(int i = 0; i < n; i++) {
        #pragma omp critical
        v[indices[i]] += i;
    }
}

int main() { 
    func1();
    func2();
}