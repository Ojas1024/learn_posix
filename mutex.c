#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;

void* worker(void* arg)
{
    int id = *(int*)arg;

    printf("Thread %d waiting...\n", id);

    pthread_mutex_lock(&lock);
    printf("Thread %d ENTERED \n", id);
    sleep(2);
    printf("Thread %d LEAVING\n", id);
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main()
{
    pthread_t threads[5];
    int ids[5];
    pthread_mutex_init(&lock, NULL);
    for(int i=0;i<5;i++)
    {
        ids[i] = i+1;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }
    for(int i=0;i<5;i++)
        pthread_join(threads[i], NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}
