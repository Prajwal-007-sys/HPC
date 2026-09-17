#include <pthread.h>

int main() { 
    
    int pthread_create (
        pthread_t *thread_handle,
        const pthread_attr_t *attribute, 
        void * (*thread_function) (void *), 
        void *arg 
    );