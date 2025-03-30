#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server\n");
    printf("Guess a number between 1 and 100\n");

    char buffer[BUFFER_SIZE];
    while(1) {
        printf("Enter your guess: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        send(sock, buffer, strlen(buffer), 0);

        int received = recv(sock, buffer, BUFFER_SIZE, 0);
        if(received <= 0) break;
        buffer[received] = '\0';

        printf("Server response: %s\n", buffer);

        if(strcmp(buffer, "correct") == 0) {
            printf("Congratulations! You won!\n");
            break;
        }
    }

    close(sock);
    return 0;
}
