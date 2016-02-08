#include "Communication_master.hpp"




using namespace std;


    int sockfd, new_fd, sock_ord, numbytes;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;
    int yes = 1;
    char buf[ MAXDATASIZE ];


    vector<string> received_replies;
    map<string, const char* > lift_list;


map<string,const char*> Communication_Master::full_lift_list(){

    // Some example lift names and addresses
    
    lift_list["lift1"] = "192.168.1.1";
    lift_list["lift2"] = "192.168.1.2";
    lift_list["lift3"] = "192.168.1.3";


    return lift_list;

}

void Communication_Master::initialize_server(){

        if(( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {

            perror( "socket error" );
            exit( 1 );
        }

        if( setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof( int ) ) == - 1 ) {
            perror( "setsockopt error" );
            exit( 1 );
        }


        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons( MYPORT );
        my_addr.sin_addr.s_addr = INADDR_ANY;
        memset( &( my_addr.sin_zero ), '\0', 8 );


        if( bind( sockfd,( struct sockaddr * ) & my_addr, sizeof( struct sockaddr ) ) == - 1 ) {
            perror( "bind error" );
            exit( 1 );
        }


    }

void Communication_Master::initialize_client(){

    if(( sock_ord = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {
        perror( "socket" );
        exit( 1 );
    }


    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons( MYPORT );
    inet_aton("127.0.1.1", &their_addr.sin_addr);
    memset( &( their_addr.sin_zero ), '\0', 8 );

}

void Communication_Master::accept_replies(){

        initialize_server();

        if( listen( sockfd, BACKLOG ) == - 1 ) {
                perror( "listen error" );
                exit( 1 );
            }

        while( 1 ) {

        sin_size = sizeof( struct sockaddr_in );


        if(( new_fd = accept( sockfd,( struct sockaddr * ) & their_addr, & sin_size ) ) == - 1 ) {
            perror( "accept error" );
            continue;
        }

    cout << inet_ntoa(my_addr.sin_addr) << endl;


    if( !fork() ) {
            close( sockfd );
            if(( numbytes = recv( new_fd, buf, MAXDATASIZE - 1, 0 ) ) == - 1 ) {
                perror( "recv error" );
                exit( 1 );
                }

            string order(buf);

            received_replies.push_back(order);

            buf[ numbytes ] = '\0';

            cout<< received_replies.back() << endl;

            close( new_fd );
            exit( 0 );
        }
        close( new_fd );
    }
}


 void Communication_Master::send_orders(const char *order, const char *destination){

    initialize_client();

    inet_aton(destination, &their_addr.sin_addr);

    if( connect( sock_ord,( struct sockaddr * ) & their_addr, sizeof( struct sockaddr ) ) == - 1 ) {
        perror( "connect error" );
        exit( 1 );
    }

    send( sock_ord, order, 50, 0 );

    }



