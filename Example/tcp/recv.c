#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    char buffer[64];
    if (argc < 3) exit(1);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_ADDRCONFIG;

    struct addrinfo *address;

    if (getaddrinfo(argv[1], argv[2], &hints, &address) < 0) {
        perror("Getting address");
        exit(1);
    }

    int recv_socket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (recv_socket < 0) {
        perror("Creating socket");
        exit(1);
    }

    if (connect(recv_socket, address->ai_addr, address->ai_addrlen) < 0) {
        perror("Connecting socket");
        exit(1);
    }
    printf("Connection sucessfully established.\n");

    int received;
    while(1) {
        received = recv(recv_socket, buffer, sizeof(buffer), 0);
        write(1, buffer, received);
    }
}
