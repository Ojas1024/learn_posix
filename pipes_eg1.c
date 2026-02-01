#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main()
{
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if(pid == 0)
    {
        // CHILD (reader)
        close(fd[1]); // close write end

        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));

        printf("Child received: %s\n", buffer);

        close(fd[0]);
    }
    else
    {
        // PARENT (writer)
        close(fd[0]); // close read end

        char msg[] = "Hello from parent";

        write(fd[1], msg, strlen(msg)+1);

        close(fd[1]);
    }

    return 0;
}
