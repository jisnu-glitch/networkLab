// server.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    int clients[10], client_count = 0;
    char buffer[1024], msg[1200];
    fd_set readfds;
    int max_fd, i;

    // initialize client array
    for (i = 0; i < 10; i++)
        clients[i] = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9090);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Chat server started on port 9090...\n");

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_fd = server_fd;

        // add client sockets to fd set
        for (i = 0; i < 10; i++) {
            if (clients[i] > 0) {
                FD_SET(clients[i], &readfds);
                if (clients[i] > max_fd)
                    max_fd = clients[i];
            }
        }

        select(max_fd + 1, &readfds, NULL, NULL, NULL);

        // new connection
        if (FD_ISSET(server_fd, &readfds)) {
            client_fd = accept(server_fd, NULL, NULL);
            clients[client_count++] = client_fd;

            sprintf(buffer, "Client%d joined the chat\n", client_count);
            printf("%s", buffer);

            // notify others
            for (i = 0; i < client_count; i++)
                write(clients[i], buffer, strlen(buffer));
        }

        // message from client
        for (i = 0; i < client_count; i++) {
            int sd = clients[i];
            if (FD_ISSET(sd, &readfds)) {
                int val = read(sd, buffer, sizeof(buffer));
                if (val == 0) {
                    close(sd);
                    clients[i] = 0;
                } else {
                    buffer[val] = '\0';
                    sprintf(msg, "Client%d: %s", i + 1, buffer);

                    // broadcast
                    for (int j = 0; j < client_count; j++) {
                        if (clients[j] != sd && clients[j] != 0) {
                            write(clients[j], msg, strlen(msg));
                        }
                    }
                }
            }
        }
    }
}
