#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

void main() {

    int port = 5566;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;

    char buffer[1024];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }

    printf("[+]TCP server Socket created.\n");

    memset(&addr, '\0', sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to the server..\n");

    char p[1024];

    while (1) {

        scanf("%s", p);
        send(sock, p, strlen(p), 0);

        memset(buffer, 0, sizeof(buffer));
        int n = recv(sock, buffer, sizeof(buffer) - 1, 0);

        if (n <= 0) {
            printf("Server disconnected\n");
            close(sock);
            break;
        }

        if (strncmp(buffer, "STOP", 4) == 0) {
            printf("Disconnected from server\n");
            close(sock);
            break;
        }

        printf("Server: %s\n", buffer);
    }
}
