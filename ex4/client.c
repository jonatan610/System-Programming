// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 12345
#define THREAD_COUNT 5
#define BUFFER_SIZE 4096

const char *messages[THREAD_COUNT] = {
    "hello server!\n",
    "this is client 2.\n",
    "how are you?\n",
    "echo this line.\n",
    "last client here!\n"
};

// Thread function for each client
void* client_thread(void *arg) {
    int idx = *(int*)arg;
    free(arg);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket failed");
        return NULL;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
    };

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock);
        return NULL;
    }

    const char *msg = messages[idx];
    send(sock, msg, strlen(msg), 0);

    char buffer[BUFFER_SIZE];
    ssize_t received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        printf("Client %d received: %s", idx + 1, buffer);
    }

    close(sock);
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        int *idx = malloc(sizeof(int));
        *idx = i;
        pthread_create(&threads[i], NULL, client_thread, idx);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
