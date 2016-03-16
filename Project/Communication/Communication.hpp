#ifndef _COMMUNICATION_HPP
#define _COMMUNICATION_HPP

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
#include <sstream>
#include <stdexcept>
#include <thread>

#define MYPORT_RECV 3490
#define MYPORT_SEND 3491
#define MASTER_IP "0.0.0.0"
#define MAXDATASIZE 256
#define BACKLOG 10

using namespace std;


class Communication{

protected:

    fd_set head_set;
    fd_set read_fds;
    int fdmax;
    int listener;

    int sockfd, newfd, sock_ord, numbytes;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;
    int yes;
    char buf[ MAXDATASIZE ];
    char buf_temp[MAXDATASIZE];
    int connected_lifts_num = 0;
    bool underflow_packet_proc = false;
    bool overflow_packet_proc = false;

    vector<string> received_messages;
    map<string, string > lift_list;

public:

const char* lift_status = "IDLE";

void initialize_reciever();

void initialize_sender();

string marshal(const char *message, const char *from,const char *destination);

string unmarshal(string packet, string type);

int recieve_all(int socket);

int send_all( int s, const char *message, const char *from, const char *destination);

void send_message(const char *message, const char *destination);

void Thread_send();

};


#endif
