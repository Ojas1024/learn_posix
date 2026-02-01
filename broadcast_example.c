#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
pthread_cond_t cond;

int shutdown_flag = 0;

void* worker(void* arg)
{
    int id = *(int*)arg;

    pthread_mutex_lock(&lock);

    while(!shutdown_flag)
    {
        printf("Thread %d waiting...\n", id);
        pthread_cond_wait(&cond, &lock);
    }

    printf("Thread %d exiting\n", id);

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main()
{
    pthread_t t[3];
    int id[3] = {1,2,3};

    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond,NULL);

    for(int i=0;i<3;i++)
        pthread_create(&t[i],NULL,worker,&id[i]);

    sleep(3);

    pthread_mutex_lock(&lock);

    shutdown_flag = 1;
    printf("Broadcasting shutdown...\n");

    pthread_cond_broadcast(&cond); // 🔹 wake ALL

    pthread_mutex_unlock(&lock);

    for(int i=0;i<3;i++)
        pthread_join(t[i],NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}