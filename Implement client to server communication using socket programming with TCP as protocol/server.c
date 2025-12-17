#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

void main()
{
    char *ip = "192.0.0.1";
    int port = 5566;

    int sock_id, client_id;

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    char buffer[1024];
    int n;

    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id < 0)
    {
        perror("[-]Socket  error");
        exit(1);
    }

    printf("[+]TCP server Socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    n = bind(sock_id, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("[-]bind error");
        exit(1);
    }

    printf("[+]Bind to ther port number : %d\n", port);

    listen(sock_id, 5);
    printf("Listening..\n");

    while (1)
    {
        addr_size = sizeof(client_addr);
        client_id = accept(
            sock_id,
            (struct sockaddr *)&client_addr,
            &addr_size
        );

        printf("[+]Client connected..\n");

        char p[1024];

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));

            int n = recv(
                client_id,
                buffer,
                sizeof(buffer) - 1,
                0
            );

            if (n <= 0)
            {
                printf("Client disconnected\n");
                close(client_id);
                break;
            }

            if (strncmp(buffer, "STOP", 4) == 0)
            {
                printf("Client sent STOP\n");
                close(client_id);
                exit(1);
            }

            printf("Client: %s\n", buffer);

            scanf("%s", p);
            send(client_id, p, strlen(p), 0);
        }
  }
}
