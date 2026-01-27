// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 12345
#define BUFFER_SIZE 4096

int client_count = 0; // Global client counter
pthread_mutex_t client_count_mutex = PTHREAD_MUTEX_INITIALIZER;

// Helper function to send all data
ssize_t send_all(int sockfd, const void *buffer, size_t length) {
    size_t total_sent = 0;
    const char *ptr = buffer;

    while (total_sent < length) {
        ssize_t sent = send(sockfd, ptr + total_sent, length - total_sent, 0);
        if (sent <= 0) return -1;
        total_sent += sent;
    }
    return total_sent;
}

// Helper function to receive all data until newline or buffer full
ssize_t recv_all(int sockfd, char *buffer, size_t max_len) {
    size_t total_received = 0;
    while (total_received < max_len - 1) {
        ssize_t received = recv(sockfd, buffer + total_received, 1, 0);
        if (received <= 0) break;
        if (buffer[total_received] == '\n') {
            total_received++;
            break;
        }
        total_received++;
    }
    buffer[total_received] = '\0';
    return total_received;
}

// Function to convert lowercase to uppercase in a string
void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}

// Thread function for handling each client
void* handle_client(void *arg) {
    int client_sock = *(int*)arg;
    free(arg);

    // Increment client count
    pthread_mutex_lock(&client_count_mutex);
    client_count++;
    printf("Client connected. Total: %d\n", client_count);
    pthread_mutex_unlock(&client_count_mutex);

    char buffer[BUFFER_SIZE];
    ssize_t received = recv_all(client_sock, buffer, BUFFER_SIZE);
    if (received > 0) {
        to_uppercase(buffer);
        send_all(client_sock, buffer, strlen(buffer));
    }

    close(client_sock);

    // Decrement client count
    pthread_mutex_lock(&client_count_mutex);
    client_count--;
    printf("Client disconnected. Total: %d\n", client_count);
    pthread_mutex_unlock(&client_count_mutex);

    return NULL;
}

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
    };

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 10) < 0) {
        perror("listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int *client_sock = malloc(sizeof(int));
        *client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addrlen);
        if (*client_sock < 0) {
            perror("accept failed");
            free(client_sock);
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, client_sock) != 0) {
            perror("pthread_create failed");
            close(*client_sock);
            free(client_sock);
        } else {
            pthread_detach(tid); // Auto-clean thread
        }
    }

    close(server_sock);
    return 0;
}
