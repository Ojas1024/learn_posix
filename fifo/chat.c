#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#define SIZE 256

int readfd, writefd;

void* reader_thread(void* arg)
{
    char buffer[SIZE];

    while(1)
    {
        int n = read(readfd, buffer, SIZE);
        if(n > 0)
        {
            printf("\nFriend: %s\nYou: ", buffer);
            fflush(stdout);
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./chat A or ./chat B\n");
        return 1;
    }

    // Decide direction
    if(argv[1][0] == 'A')
    {
        writefd = open("fifo_ab", O_WRONLY);
        readfd  = open("fifo_ba", O_RDONLY);
        printf("Chat started as A\n");
    }
    else
    {
        writefd = open("fifo_ba", O_WRONLY);
        readfd  = open("fifo_ab", O_RDONLY);
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

        write(writefd, msg, strlen(msg)+1);
    }

    return 0;
}
