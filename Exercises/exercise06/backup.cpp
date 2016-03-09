#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>

#define MYPORT 6667
#define BACKLOG 10

using namespace std;


int main(){


fstream file;
struct sockaddr_in my_addr;
struct sockaddr_in their_addr;
int listener, newfd, sock_alive;
int number, numbytes;
char buf[128];
int yes = 1;
socklen_t sin_size;
int state = 1;
const char *alive_msg = "Backup is alive";

struct timeval time;
time.tv_sec = 1;

fd_set head_set;
fd_set read_fds;
int fdmax;



        FD_ZERO( & head_set );
        FD_ZERO( & read_fds );


        if(( listener = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {

            perror( "socket error" );
            exit( 1 );
        }

        if( setsockopt( listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) ) == - 1 ) {
            perror( "setsockopt error" );
            exit( 1 );
        }


        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons( MYPORT );
        my_addr.sin_addr.s_addr = INADDR_ANY;
        memset( &( my_addr.sin_zero ), '\0', 8 );


        if( bind( listener,( struct sockaddr * ) & my_addr, sizeof( struct sockaddr ) ) == - 1 ) {
            perror( "bind error" );
            exit( 1 );
        }

          if( listen( listener, BACKLOG ) == - 1 ) {
        perror( "listen" );
        exit( 1 );
        }

         FD_SET( listener, & head_set );
        fdmax = listener;


while( 1 ) {


        read_fds = head_set;
        if( select( fdmax + 1, & read_fds, NULL, NULL, &time ) == - 1 ) {
            perror( "select" );
            exit( 1 );
        }


        for(int i = 0; i <= fdmax; i++ ) {
            if( FD_ISSET( i, & read_fds ) ) { // checking for ready socket(read)

        if( i == listener ) {

                sin_size = sizeof( struct sockaddr_in );


                if(( newfd = accept( listener,( struct sockaddr * ) & their_addr, & sin_size ) ) == - 1 ) {
                    perror( "accept error" );
                    }

                else {

                    FD_SET( newfd, & head_set );
                        if( newfd > fdmax ) {

                             fdmax = newfd;
                            }
                            printf( "selectserver: new connection from %s on "
                            "socket %d\n", inet_ntoa( their_addr.sin_addr ), newfd );
                        }
                    }

            else {     //have some data to recieve


                    if(( numbytes = recv(i, buf,sizeof(buf),0)) <= 0 ) {

                        if( numbytes == 0 ) {    // connection lost


                           cout << "Optimus prime is Dead! I am in control   now" << endl;



                        close( listener); // papa!
                        FD_CLR( i, & head_set );// usuń z głównego zestawu


                            int pid = fork();

                            if(pid == 0) {
                                setsid();
                                pid = fork();
                                if (pid == 0) {
                                    char *argv[] = {"gnome-terminal", "-x", "./backup.exe", 0};
                                    int rc = execv("/usr/bin/gnome-terminal", argv);
                                    if (rc == -1 )
                                        perror("Error at spawning backup");
                                }
                                exit(0);
                            }
                            else {

                            file.open("log.txt", ios::in);
                            file >> number;
                            file.close();

                            sleep(5);
                            if(( sock_alive = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {
                                perror( "socket" );
                                exit( 1 );
                            }

                            if( setsockopt( sock_alive, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) ) == - 1 ) {
                            perror( "setsockopt error" );
                            exit( 1 );
                            }


                            their_addr.sin_family = AF_INET;
                            their_addr.sin_port = htons( MYPORT );
                            their_addr.sin_addr.s_addr = INADDR_ANY;
                            memset( &( their_addr.sin_zero ), '\0', 8 );


                            if( connect( sock_alive,( struct sockaddr * ) & their_addr,
                            sizeof( struct sockaddr ) ) == - 1 ) {
                                perror( "connect" );
                                exit( 1 );
                            }



                        while(1){
                            cout << number+1 <<endl;
                            number++;


                            file.open("log.txt", ios::out);
                            file << number;
                            file.close();


                            send(sock_alive, alive_msg, strlen(alive_msg), 0);
                            sleep(1);

                            }

                         }


                         } else {

                            perror( "recv error" );
                        }
                       // close( i ); // papa!
                       // FD_CLR( i, & head_set );// usuń z głównego zestawu

                        } if(numbytes > 0){

                            cout << buf <<endl;

                          }

                    }
                }
            }
        }



return 0;
}
