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

    if (argc < 2) exit(1);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_ADDRCONFIG|AI_PASSIVE;

    struct addrinfo *address;

    int error;
    if ((error = getaddrinfo(NULL, argv[1], &hints, &address)) < 0) {
        printf("Getting address: %s\n", gai_strerror(error));
        exit(1);
    }

    struct sockaddr_in peer;
    int size = sizeof(peer);
    printf("Peer: %s\n", inet_ntoa(peer.sin_addr));

    int listen_socket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (listen_socket < 0) {
        perror("Creating socket");
        exit(1);
    }

    if (bind(listen_socket, address->ai_addr, address->ai_addrlen) < 0) {
        perror("Binding socket");
        exit(1);
    }

    if (listen(listen_socket, 0) < 0) {
        perror("Listening socket");
        exit(1);
    }

    int send_socket;
    if ((send_socket = accept(listen_socket, (struct sockaddr *)&peer, &size)) < 0) {
        perror("Accepting socket");
        exit(1);
    }
    printf("Peer: %s\n", inet_ntoa(peer.sin_addr));

    printf("Connection sucessfully established.\n");

    int readed;
    while((readed = read(0, buffer, sizeof(buffer))) > 0) {
        send(send_socket, buffer, readed, 0);
    }

    if ((shutdown(send_socket, SHUT_RDWR)) < 0) {
        perror("Shutting down socket");
        exit(1);
    }

    if ((shutdown(listen_socket, SHUT_RDWR)) < 0) {
        perror("Shutting down socket");
        exit(1);
    }

    close(send_socket);
    close(listen_socket);
    exit(0);
}
