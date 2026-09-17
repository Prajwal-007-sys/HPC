#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <limits.h>

#define N 50000
#define NUM_THREADS 5

long long *numbers;

// Global variables for Mutex version
long long parallel_mutex_total = 0;
pthread_mutex_t lock;

// Returns time in seconds from a timespec struct
double get_time_sec(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

// ----------------------------------------------------------------
// 1. SERIAL IMPLEMENTATION
// ----------------------------------------------------------------
double run_serial(long long *result) {
    struct timespec start, end;
    long long total = 0;

    timespec_get(&start, TIME_UTC);
    for (long long i = 0; i < N; i++) {
        total += numbers[i];
    }
    timespec_get(&end, TIME_UTC);

    *result = total;
    return get_time_sec(start, end);
}

// ----------------------------------------------------------------
// 2. PARALLEL WITH MUTEX LOCK (Worker + Runner)
// ----------------------------------------------------------------
void* sum_runner_mutex(void *arg) {
    long index = (long)arg;
    long long chunk_size = N / NUM_THREADS;
    long long start = index * chunk_size;
    long long end = (index == NUM_THREADS - 1) ? N : start + chunk_size;

    long long local_sum = 0;
    for (long long i = start; i < end; i++) {
        local_sum += numbers[i];
    }

    // Mutex lock forces threads to wait for each other at the end
    pthread_mutex_lock(&lock);
    parallel_mutex_total += local_sum;
    pthread_mutex_unlock(&lock);

    return NULL;
}

double run_parallel_mutex(long long *result) {
    struct timespec start, end;
    pthread_t threads[NUM_THREADS];

    parallel_mutex_total = 0;
    pthread_mutex_init(&lock, NULL);

    timespec_get(&start, TIME_UTC);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, sum_runner_mutex, (void*)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    timespec_get(&end, TIME_UTC);
    pthread_mutex_destroy(&lock);

    *result = parallel_mutex_total;
    return get_time_sec(start, end);
}

// ----------------------------------------------------------------
// 3. PARALLEL LOCK-FREE / REDUCTION (Worker + Runner)
// ----------------------------------------------------------------
void* sum_runner_lockfree(void *arg) {
    long index = (long)arg;
    long long chunk_size = N / NUM_THREADS;
    long long start = index * chunk_size;
    long long end = (index == NUM_THREADS - 1) ? N : start + chunk_size;

    // Allocate heap memory to return the thread's partial sum safely
    long long *local_sum = malloc(sizeof(long long));
    *local_sum = 0;

    for (long long i = start; i < end; i++) {
        *local_sum += numbers[i];
    }

    return (void*)local_sum; // Return local sum pointer directly
}

double run_parallel_lockfree(long long *result) {
    struct timespec start, end;
    pthread_t threads[NUM_THREADS];
    long long total_sum = 0;

    timespec_get(&start, TIME_UTC);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, sum_runner_lockfree, (void*)i);
    }

    // Join threads and safely aggregate returned local sums sequentially
    for (int i = 0; i < NUM_THREADS; i++) {
        long long *subtotal;
        pthread_join(threads[i], (void**)&subtotal);
        total_sum += *subtotal;
        free(subtotal); // Free thread heap allocation
    }

    timespec_get(&end, TIME_UTC);

    *result = total_sum;
    return get_time_sec(start, end);
}

// ----------------------------------------------------------------
// MAIN FUNCTION & PERFORMANCE METRICS
// ----------------------------------------------------------------
int main() {
    numbers = malloc(N * sizeof(long long));
    if (numbers == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (long long i = 0; i < N; i++) {
        numbers[i] = i;
    }

    long long res_serial = 0, res_mutex = 0, res_lockfree = 0;

    double t_serial   = run_serial(&res_serial);
    double t_mutex    = run_parallel_mutex(&res_mutex);
    double t_lockfree = run_parallel_lockfree(&res_lockfree);

    // Speedup calculations
    double s_mutex    = t_serial / t_mutex;
    double s_lockfree = t_serial / t_lockfree;

    // Output Results Comparison Table
    printf("========================================================================\n");
    printf("                       EXECUTION COMPARISON METRICS                     \n");
    printf("========================================================================\n");
    printf("Array Elements (N) : %d | Threads (M) : %d\n", N, NUM_THREADS);
    printf("------------------------------------------------------------------------\n");
    printf("%-22s %-20s %-15s %-10s\n", "Implementation", "Total Sum", "Time (sec)", "Speedup");
    printf("------------------------------------------------------------------------\n");
    printf("%-22s %-20lld %-15.6f %-10.2fx\n", "1. Serial Engine", res_serial, t_serial, 1.00);
    printf("%-22s %-20lld %-15.6f %-10.2fx\n", "2. Parallel (Mutex)", res_mutex, t_mutex, s_mutex);
    printf("%-22s %-20lld %-15.6f %-10.2fx\n", "3. Parallel (Lock-Free)", res_lockfree, t_lockfree, s_lockfree);
    printf("========================================================================\n");

    free(numbers);
    return 0;
}