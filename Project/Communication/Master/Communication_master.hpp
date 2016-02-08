#ifndef Communication_master_hpp
#define Communication_master_hpp

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
#include <map>

#define MYPORT 3490
#define MAXDATASIZE 100
#define BACKLOG 10

using namespace std;

/* Class enabling communication beetween Master and multiple lifts
#  Master sends orders to the individual lift(destination)
#  and accepts responsos from it
#  Each lift has its own socket in terms of applying respones
#  There is one socket for sending orders
*/
class Communication_Master{

private:

    int sockfd, new_fd, sock_ord, numbytes; // listenning socket, sending socket,
                                            //  recieving socket and buffer size


    struct sockaddr_in my_addr; // dstruct containg Master info

    struct sockaddr_in their_addr; // truct containing Lift info

    socklen_t sin_size; // IP address length

    int yes; // socket option yes = 1

    char buf[ MAXDATASIZE ]; // buffer


    vector<string> received_replies;  // Vecotr storing all recieved replies
    map<string,const char*> lift_list; // Map sotirng list of lifts and their IPs


public:


/*
#  Function assigns each lift its IP address
*/
    map<string,const char*> full_lift_list();

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
#  Function contains infinite loop that accepts all replies
#  from lifts - each lift get its own socket in child thread - fork()
#  Saves all accepted orders in vector received_replies
*/
    void accept_replies();



/*
#  Function connects to the lift socket and sends orders
*/
    void send_orders(const char *message, const char *destination);

};

#endif
