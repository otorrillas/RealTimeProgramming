#include "Communication.hpp"
#include "Communication_master.hpp"

using namespace std;


    Communication_Master::Communication_Master(){

    Communication();
    initialize_reciever();

    }

    Communication_Master::Communication_Master(string sender){

    Communication();
    initialize_sender();

    }

    void Communication_Master::accept_replies(){


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

            if( FD_ISSET( i, & read_fds ) ) { // mamy jednego!!

                if( i == listener ) {

                        sin_size = sizeof( struct sockaddr_in );


                    if(( newfd = accept( listener,( struct sockaddr * ) & their_addr, & sin_size ) ) == - 1 ) {
                        perror( "accept error" );

                    } else {
                        FD_SET( newfd, & head_set ); // dodaj do głównego zestawu

                            if( newfd > fdmax ) {fdmax = newfd;}// śledź maksymalny

                                printf( "selectserver: new connection from %s on "
                                "socket %d\n", inet_ntoa( their_addr.sin_addr ), newfd );
                                }
                 } else {

                    if(( numbytes = recieve_all( i) ) <= 0 ) {


                        if( numbytes == 0 ) {
                            printf( "selectserver: socket %d hung up\n", i );
                        }
                        else {
                            perror( "recv" );
                        }
                            close( i );
                            FD_CLR( i, & head_set );

                        } else {

                            string packet(buf);

                            if(unmarshal(packet, "message") == "HELLO" ){

                                connected_lifts_num++;
                                string lift_num = "Lift" + to_string(connected_lifts_num);
                                lift_list.insert(pair<string,string>(lift_num,unmarshal(packet,"from")));

                            }

                        }
                    }
                }
            }
        }
    }


