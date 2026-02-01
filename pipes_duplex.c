#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main()
{
    int p2c[2]; // parent to child
    int c2p[2]; // child to parent

    pipe(p2c);
    pipe(c2p);

    pid_t pid = fork();

    if(pid == 0)
    {
        // ===== CHILD =====
        close(p2c[1]); // close write
        close(c2p[0]); // close read

        char buffer[100];

        read(p2c[0], buffer, sizeof(buffer));
        printf("Child got: %s\n", buffer);

        char reply[] = "Roger that, parent";
        write(c2p[1], reply, strlen(reply)+1);

        close(p2c[0]);
        close(c2p[1]);
    }
    else
    {
        // ===== PARENT =====
        close(p2c[0]);
        close(c2p[1]);

        char msg[] = "Hello child";
        write(p2c[1], msg, strlen(msg)+1);

        char buffer[100];
        read(c2p[0], buffer, sizeof(buffer));

        printf("Parent got: %s\n", buffer);

        close(p2c[1]);
        close(c2p[0]);
    }

    return 0;
}
