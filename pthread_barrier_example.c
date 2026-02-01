#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

pthread_barrier_t barrier;

void* worker(void* arg)
{
    int id = *(int*)arg;

    printf("Thread %d doing phase 1 work\n", id);
    sleep(id); 

    printf("Thread %d reached barrier\n", id);

    pthread_barrier_wait(&barrier);

    printf("Thread %d starting phase 2\n", id);

    return NULL;
}

int main()
{
    pthread_t threads[N];
    int ids[N];

    pthread_barrier_init(&barrier, NULL, N);

    for(int i=0;i<N;i++)
    {
        ids[i] = i+1;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for(int i=0;i<N;i++)
        pthread_join(threads[i], NULL);

    pthread_barrier_destroy(&barrier);

    return 0;
}
