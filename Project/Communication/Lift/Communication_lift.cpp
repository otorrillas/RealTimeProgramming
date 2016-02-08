#include "Communication_lift.hpp"



using namespace std;


    int sockfd, new_fd, sock_rep, numbytes;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    const char *master_address = "0.0.0.0";
    socklen_t sin_size;
    int yes = 1;
    char buf[ MAXDATASIZE ];



    vector<string> received_orders;


void Communication_Lift::initialize_server(){

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

void Communication_Lift::initialize_client(){

    if(( sock_rep = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {
        perror( "socket" );
        exit( 1 );
    }


    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons( MYPORT );
    inet_aton(master_address, &their_addr.sin_addr);
    memset( &( their_addr.sin_zero ), '\0', 8 );

}

void Communication_Lift::listen_and_accept(){

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



        if(( numbytes = recv( new_fd, buf, MAXDATASIZE - 1, 0 ) ) == - 1 ) {
                perror( "recv error" );
                exit( 1 );
                }

            string order(buf);

            received_orders.push_back(order);

            buf[ numbytes ] = '\0';

            cout<< received_orders.back() << endl;


            }

             close( new_fd );
                exit( 0 );
        }


 void Communication_Lift::reply(const char *message){

    initialize_client();

    cout << inet_ntoa(their_addr.sin_addr) << endl;

    if( connect( sock_rep,( struct sockaddr * ) & their_addr, sizeof( struct sockaddr ) ) == - 1 ) {
        perror( "connect error" );
        exit( 1 );
    }

    send( sock_rep, message, 50, 0 );

    }




