#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
pthread_cond_t cond;

int items = 0;

void* consumer(void* arg)
{
    int id = *(int*)arg;

    pthread_mutex_lock(&lock);

    while(items == 0)
    {
        printf("Consumer %d waiting\n", id);
        pthread_cond_wait(&cond, &lock);
    }

    items--;
    printf("Consumer %d consumed item\n", id);

    pthread_mutex_unlock(&lock);
    return NULL;
}

void* producer(void* arg)
{
    sleep(2);

    pthread_mutex_lock(&lock);

    items++;
    printf("Producer made 1 item\n");

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main()
{
    pthread_t c[3], p;
    int id[3] = {1,2,3};

    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond,NULL);

    for(int i=0;i<3;i++)
        pthread_create(&c[i],NULL,consumer,&id[i]);

    pthread_create(&p,NULL,producer,NULL);

    for(int i=0;i<3;i++)
        pthread_join(c[i],NULL);

    pthread_join(p,NULL);
}
