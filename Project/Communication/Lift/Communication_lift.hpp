#ifndef Communication_lift_hpp
#define Communication_lift_hpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include <vector>

#define MYPORT 3490
#define MAXDATASIZE 100
#define BACKLOG 10

using namespace std;


/* Class enabling communication beetween Lifts and Master
#  Lift recieves orders from Master and sends responsos to it
#  Each lift has its own socket in terms of sending respones
#  Tand one socket for recieving  orders
*/
class Communication_Lift{

private:

    int sockfd, new_fd, sock_rep, numbytes; // listenning socket, sending socket,
                                            //  recieving socket and buffer size

    struct sockaddr_in my_addr; // truct containing Lift info

    struct sockaddr_in their_addr; // dstruct containg Master info

    const char *master_address  // IP address of Master

    socklen_t sin_size; // IP address length

    int yes; // socket option yes = 1

    char buf[ MAXDATASIZE ]; // buffer



    vector<string> received_orders; // Vecotr storing all recieved orders

public:
/*
#  Function creates socket for listenning, set its option
#  assigns IP address and port number, binds
*/
    void initialize_server();


/*
#  Function creates socket for sending replies, assigns
#  IP address and port
*/
    void initialize_client();


/*
#  Function contains infinite loop that accepts all orders
#  from Master
#  Saves all accepted orders in vector received_orders
*/
    void listen_and_accept();


/*
#  Function connects to the Master`s socket and sends replies
*/
    void reply(const char *message);

};

#endif
