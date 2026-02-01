#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem;

void* worker(void* arg)
{
    int id = *(int*)arg;

    printf("Thread %d waiting...\n", id);

    sem_wait(&sem);   
    printf("Thread %d ENTERED\n", id);
    sleep(2);
    printf("Thread %d LEAVING\n", id);

    sem_post(&sem); 

    return NULL;
}

int main()
{
    pthread_t threads[5];
    int ids[5];
    sem_init(&sem, 0, 2);

    for(int i=0;i<5;i++)
    {
        ids[i] = i+1;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for(int i=0;i<5;i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&sem);

    return 0;
}
