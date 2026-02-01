#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

    listen(server_fd, 1);

    printf("Waiting for client...\n");

    client_fd = accept(server_fd, NULL, NULL);

    char buffer[256];

    while(1)
    {
        read(client_fd, buffer, sizeof(buffer));
        printf("Client: %s\n", buffer);

        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        write(client_fd, buffer, strlen(buffer)+1);
    }
}
