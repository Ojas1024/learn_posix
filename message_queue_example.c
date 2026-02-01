#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SIZE 256

mqd_t send_q, recv_q;

void* reader_thread(void* arg)
{
    char buffer[SIZE];

    while(1)
    {
        mq_receive(recv_q, buffer, SIZE, NULL);
        printf("\nFriend: %s\nYou: ", buffer);
        fflush(stdout);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./chat A or ./chat B\n");
        return 1;
    }

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = SIZE;
    attr.mq_curmsgs = 0;

    if(argv[1][0] == 'A')
    {
        send_q = mq_open("/a_to_b", O_CREAT | O_WRONLY, 0644, &attr);
        recv_q = mq_open("/b_to_a", O_CREAT | O_RDONLY, 0644, &attr);
        printf("Chat started as A\n");
    }
    else
    {
        send_q = mq_open("/b_to_a", O_CREAT | O_WRONLY, 0644, &attr);
        recv_q = mq_open("/a_to_b", O_CREAT | O_RDONLY, 0644, &attr);
        printf("Chat started as B\n");
    }

    pthread_t tid;
    pthread_create(&tid, NULL, reader_thread, NULL);

    char msg[SIZE];

    while(1)
    {
        printf("You: ");
        fflush(stdout);

        fgets(msg, SIZE, stdin);
        msg[strcspn(msg, "\n")] = 0;

        mq_send(send_q, msg, strlen(msg)+1, 0);
    }

    return 0;
}
