// client.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    fd_set readfds;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9090);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Connected to chat server...\n");

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);       // stdin
        FD_SET(sock, &readfds);    // socket

        select(sock + 1, &readfds, NULL, NULL, NULL);

        // message from server
        if (FD_ISSET(sock, &readfds)) {
            int n = read(sock, buffer, sizeof(buffer));
            if (n <= 0) break;
            buffer[n] = '\0';
            printf("%s", buffer);
        }

        // message from user
        if (FD_ISSET(0, &readfds)) {
            fgets(buffer, sizeof(buffer), stdin);
            write(sock, buffer, strlen(buffer));
        }
    }

    close(sock);
    return 0;
}
