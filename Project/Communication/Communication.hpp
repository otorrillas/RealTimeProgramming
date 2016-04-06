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

#include <fstream>
#include <sys/ioctl.h>
#include <net/if.h>


#define MYPORT_MASTER 3490
#define MYPORT_LIFT 3492
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

    int sockfd, newfd, sock_ord, numbytes, socky, exist = 0;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    struct ifreq ifr;
    socklen_t sin_size;
    int yes;
    char buf[ MAXDATASIZE ];
    char buf_temp[MAXDATASIZE];
    int connected_lifts_num = 0;
    bool underflow_packet_proc = false;
    bool overflow_packet_proc = false;

    vector<string> received_messages;
    fstream lift_file;


public:

map<string, string > lift_list;
map<string, int> sockets;

string marshal(const char *message, const char *from,const char *destination);

string unmarshal(string packet, string type);

int recieveAll(int socket);

int sendAll( int s, const char *message, const char *from, const char *destination);


};


#endif
