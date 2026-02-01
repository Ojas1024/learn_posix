#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int data = 0;
int ready = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

void* producer(void* arg)
{
    sleep(2);

    pthread_mutex_lock(&lock);

    data = 42;
    ready = 1;

    printf("Producer: Data ready\n");

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    return NULL;
}

void* consumer(void* arg)
{
    pthread_mutex_lock(&lock);

    while(!ready)
    {
        printf("Consumer: waiting...\n");
        pthread_cond_wait(&cond, &lock);
    }

    printf("Consumer: got data = %d\n", data);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main()
{
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, consumer, NULL);
    pthread_create(&t2, NULL, producer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
