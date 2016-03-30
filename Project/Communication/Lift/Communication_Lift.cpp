#include "Communication.hpp"
#include "Communication_lift.hpp"

using namespace std;



    Communication_Lift::Communication_Lift(){

    Communication();
    initialize_reciever();
    initialize_sender();
    send_message("HELLO", MASTER_IP);

    }

    void Communication_Lift::accept_orders(){


        if( listen( listener, BACKLOG ) == - 1 ) {
                perror( "listen error" );
                exit( 1 );
            }


        while( 1 ) {

            sin_size = sizeof( struct sockaddr_in );

            if(( listener = accept( sockfd,( struct sockaddr * ) & their_addr, & sin_size ) ) == - 1 ) {
                perror( "accept" );
                continue;
            }
            if( !fork() ) {

                close( sock_ord );

                if(( numbytes = recieve_all( listener) ) <= 0 ) {

                        if( numbytes == 0 ) {
                            printf( "selectserver: socket %d hung up\n", listener );
                        }
                        else {
                            perror( "recv" );
                        }
                            close( listener);

                        } else {

                send_message(lift_status, MASTER_IP);

                }
                close( listener );
        }
    }

}


