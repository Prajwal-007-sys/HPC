#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define s1 10
#define s2 100
#define s3 10000
#define s4 1000000
#define s5 10000000
#define s6 100000000
#define s7 1000000000

// Shared global count
long long global_count = 0;

// Mutex to protect global_count
pthread_mutex_t mutex;

// Structure containing information given to each thread
typedef struct
{
    long long samples;
    unsigned int seed;
} ThreadData;


// Function executed by each thread
void *calculate(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    long long local_count = 0;

    for (long long i = 0; i < data->samples; i++)
    {
        // Generate random x and y between -1 and 1
        double x = -1.0 + 2.0 * rand_r(&data->seed) / (double)RAND_MAX;
        double y = -1.0 + 2.0 * rand_r(&data->seed) / (double)RAND_MAX;

        // Check whether point is inside unit circle
        if (x * x + y * y <= 1.0)
        {
            local_count++;
        }
    }

    // Critical section
    pthread_mutex_lock(&mutex);

    global_count += local_count;

    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main()
{
    // Number of threads
    int p = 4;

    // All sample sizes
    long long samples[] = { s1,s2,s3,s4,s5,s6,s7 };

    int number_of_samples = 7;

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    for (int j = 0; j < number_of_samples; j++)
    {
        long long s = samples[j];

        // Reset global count for this sample size
        global_count = 0;

        // Array of threads
        pthread_t threads[p];

        // Data for each thread
        ThreadData thread_data[p];

        // Divide samples among threads
        long long samples_per_thread = s / p;

        printf("\nSample size = %lld\n", s);
        printf("Threads = %d\n", p);
        printf("Samples per thread = %lld\n", samples_per_thread);

        // Create threads
        for (int i = 0; i < p; i++)
        {
            thread_data[i].samples = samples_per_thread;

            // Give each thread a different random seed
            thread_data[i].seed =
                (unsigned int)time(NULL) + i;

            pthread_create(
                &threads[i],
                NULL,
                calculate,
                &thread_data[i]
            );
        }

        // Wait for all threads to finish
        for (int i = 0; i < p; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // Calculate ratio
        double r =
            (double)global_count / s;

        // Calculate pi
        double pi = 4.0 * r;

        printf("Global count = %lld\n", global_count);
        printf("Pi = %.10f\n", pi);
    }

    // Destroy mutex
    pthread_mutex_destroy(&mutex);
    pthread_exit(0);
    return 0;
}