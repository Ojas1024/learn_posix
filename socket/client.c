#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    char buffer[256];

    while(1)
    {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        write(sock, buffer, strlen(buffer)+1);

        read(sock, buffer, sizeof(buffer));
        printf("Server: %s\n", buffer);
    }
}
