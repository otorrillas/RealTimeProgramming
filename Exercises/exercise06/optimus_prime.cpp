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

using namespace std;

int main(){


int pid = fork();

if(pid == 0) {
    setsid();
    pid = fork();
    if (pid == 0) {
        char *argv[] = {"gnome-terminal", "-x", "./backup3.exe", 0};
        int rc = execv("/usr/bin/gnome-terminal", argv);
        if (rc == -1 )
            perror("Error at spawning backup");
    }
}
else {
    sleep(2);
    fstream file;
    int sock_alive;
    int number = 1;
    unsigned int time = 1;
    struct sockaddr_in their_addr;
    const char *alive_msg = "Optimus prime is alive!";



        if(( sock_alive = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {
            perror( "socket" );
            exit( 1 );
        }


        their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons( MYPORT );
        inet_aton("127.0.1.1", &their_addr.sin_addr);
        memset( &( their_addr.sin_zero ), '\0', 8 );


        if( connect( sock_alive,( struct sockaddr * ) & their_addr,
        sizeof( struct sockaddr ) ) == - 1 ) {
            perror( "connect" );
            exit( 1 );
        }

        while(1){

            cout << number << endl;


            file.open("log.txt",ios::out);
            if(file.bad()){cout << "File cannot be open" <<endl;}

            file << number;
            file << "\n";
            file.close();

            number++;

            int n = send(sock_alive, alive_msg, strlen(alive_msg), 0);
            sleep(time);

        }

    }
    return 0;
}
