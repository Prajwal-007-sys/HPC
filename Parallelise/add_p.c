
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>

#define N 50000
#define M 4  // Number of threads

long long *numbers;
long long total_sum = 0;
pthread_mutex_t lock;

void* sum_runner(void *arg) {
    long index = (long)arg; // typecasting and dereferencing
    int start = index * (N / M); // indexes of the array. Assigning N/M elements for each thread
    int end = (index == M - 1) ? N : start + (N / M);

    long long local_sum = 0;
    for (long i = start; i < end; i++) {
        local_sum += numbers[i];
    }

    pthread_mutex_lock(&lock);
    total_sum += local_sum;
    pthread_mutex_unlock(&lock);

    if (total_sum > INT_MAX) {
    // Handling positive overflow safely before it occurs
        printf("Error: Integer overflow detected!\n");
    }

    return NULL;
}
//void *print_message_function( void *ptr );

int main()
{
    struct timespec start = {0}, end = {0};
    pthread_t threads[M];

    numbers = malloc(N * sizeof(long long));
    if (numbers == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (long i=0; i<N ; i++) {
        numbers[i] = i;
    }

    timespec_get(&start, TIME_UTC);
    pthread_mutex_init(&lock, NULL);

    for(long i=0 ; i<M; i++){
        pthread_create(&threads[i], NULL, sum_runner, (void*)i);
    }

    for (int i =0; i<M ; i++) {
        pthread_join(threads[i], NULL);
    }
    timespec_get(&end, TIME_UTC);
    pthread_mutex_destroy(&lock);
    // The main thread, does not stop while the other threads are executing their tasks
    // the join command tells the main thread to stop execution until the following threads are implemented
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Parallel Elapsed time: %.6f seconds\n", elapsed);
    printf("Total: %lld\n", total_sum);

    free(numbers);
    return 0;
}
//