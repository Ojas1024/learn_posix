#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define THREAD_POOL_SIZE 5
#define QUEUE_SIZE 100

typedef struct {
    int sockets[QUEUE_SIZE];
    int head;
    int tail;
    int count;
} TaskQueue;

TaskQueue queue;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

void init_queue(TaskQueue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

void enqueue(TaskQueue *q, int client_socket) {
    q->sockets[q->tail] = client_socket;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count++;
}

int dequeue(TaskQueue *q) {
    int client_socket = q->sockets[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;
    return client_socket;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Received: %s\n", buffer);
    
    char *response = "Hello from Server";
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
}

void *thread_function(void *arg) {
    while (1) {
        int client_socket;

        pthread_mutex_lock(&mutex);
        while (queue.count == 0) {
            pthread_cond_wait(&cond_var, &mutex);
        }
        client_socket = dequeue(&queue);
        pthread_mutex_unlock(&mutex);

        handle_client(client_socket);
    }
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t thread_pool[THREAD_POOL_SIZE];

    init_queue(&queue);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], NULL, thread_function, NULL);
    }

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (queue.count < QUEUE_SIZE) {
            enqueue(&queue, new_socket);
            pthread_cond_signal(&cond_var);
        } else {
            close(new_socket);
        }
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}
