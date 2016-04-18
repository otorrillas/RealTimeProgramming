#include "Communication.hpp"
#include "Communication_Lift.hpp"

using namespace std;


void Communication_Lift::initializeReciever(){

        FD_ZERO( & head_set );
        FD_ZERO( & read_fds );

        if(( listener = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {

            perror( "socket error" );
            exit( 1 );
        }

        if( setsockopt( listener, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof( int ) ) == - 1 ) {
            perror( "setsockopt error" );
            exit( 1 );
        }

        ifr.ifr_addr.sa_family = AF_INET;
        strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
        ioctl(listener, SIOCGIFADDR, &ifr);

        my_addr.sin_family = AF_INET;
        inet_aton(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), &my_addr.sin_addr);
        my_addr.sin_port = htons(MYPORT_LIFT);
        memset( &( my_addr.sin_zero ), '\0', 8 );

        cout << " RCV IP: " << inet_ntoa(my_addr.sin_addr);

        if( bind( listener,( struct sockaddr * ) & my_addr, sizeof( struct sockaddr ) ) == - 1 ) {
            perror( "bind error recv" );
            exit( 1 );
        }


    }

void Communication_Lift::initializeSender(){

    if(( sock_ord = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {
        perror( "socket" );
        exit( 1 );
    }

     if( setsockopt( sock_ord, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof( int ) ) == - 1 ) {
            perror( "setsockopt error" );
            exit( 1 );
        }

        ifr.ifr_addr.sa_family = AF_INET;
        strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
        ioctl(sock_ord, SIOCGIFADDR, &ifr);

        my_addr.sin_family = AF_INET;
        inet_aton(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), &my_addr.sin_addr);
        my_addr.sin_port = htons(MYPORT_LIFT);
        memset( &( my_addr.sin_zero ), '\0', 8 );

        cout << " SEND IP: " << inet_ntoa(my_addr.sin_addr);

        their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons(MYPORT_MASTER);
        inet_aton("129.241.187.147", &their_addr.sin_addr);
        memset( &( their_addr.sin_zero ), '\0', 8 );

}

 void Communication_Lift::acceptOrders(){

        if( listen( listener, BACKLOG ) == - 1 ) {
                perror( "listen error" );
                exit( 1 );
            }

        FD_SET( listener, & head_set );
        fdmax = listener;


    while( 1 ) {

        read_fds = head_set;
        if( select( fdmax + 1, & read_fds, NULL, NULL, NULL ) == - 1 ) {
            perror( "select" );
            exit( 1 );
        }


        for(int i = 0; i <= fdmax; i++ ) {

            if( FD_ISSET( i, & read_fds ) ) {

                if( i == listener ) {

                        sin_size = sizeof( struct sockaddr_in );


                    if(( newfd = accept( listener,( struct sockaddr * ) & their_addr, & sin_size ) ) == - 1 ) {
                        perror( "accept error" );

                    } else {
                        FD_SET( newfd, & head_set );

                            if( newfd > fdmax ) {fdmax = newfd;}

                                printf( "selectserver: new connection from %s on "
                                "socket %d\n", inet_ntoa( their_addr.sin_addr ), newfd );
                                }
                 } else {

                    if(( numbytes = recieveAll( i) ) <= 0 ) {


                        if( numbytes == 0 ) {
                            printf( "selectserver: socket %d hung up\n", i );

                        }
                        else {
                            perror( "recv" );
                        }
                            close( i );
                            FD_CLR( i, & head_set );

                        } else {


                            cout << "Last recieved order: " << received_messages.back() << endl;
                            cout << "Number of messages:" << received_messages.size() << endl;
                            saveMessage("Client_messages.txt");


                        }
                    }
                }
            }
        }
      }

 void Communication_Lift::sendMessage(const char *message, const char *destination){

    inet_aton(destination, &their_addr.sin_addr);

    if(exist == 0){


    if( connect( sock_ord,( struct sockaddr * ) & their_addr, sizeof( struct sockaddr ) ) == - 1 ) {
        perror( "connect error" );
        exit( 1 );

    }
      exist = 1;
    }
    sendAll( sock_ord, message, inet_ntoa(my_addr.sin_addr), destination);
}

