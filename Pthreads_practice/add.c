#include<stdio.h>
#include<pthread.h>
#include<time.h>

#define N 4

typedef struct {
    int start;
    int end;
} Segment;

int total_sum = 0;

void* segment_sum(void* arg){
    Segment
}

int main() {

    clock_t clock_start_1, clock_end_1 ;
    clock_start_1 = CLOCKS_PER_SEC;
    int num_arr[N];

    for (int i=0;i<N;i++) {
        num_arr[i] = i;
    }
    clock_end_1 = CLOCKS_PER_SEC;

    pthread_t thread_1, thread_2;
    Segment args[2];
    int local_sum = 0;

    int chunk = N / 2;

    int T1 = pthread_create(thread_1, local_sum, add_parallel, &num_arr);
    int T2 = pthread_create(thread_2, local_sum, add_parallel, &num_arr);



    return 0;

}