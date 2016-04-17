#include "Communication.hpp"
#include "Communication_Master.hpp"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

using namespace std;

int isAlive(){
    int m_recv;
    int m;

    m_recv = system("pgrep Master_send");

    m = system("pgrep Master_recv");

    cout << "Result: " << m_recv << ": " << m << endl;

    if (m || m_recv  != 0){

        return 0;

    }
    else{

        return 1;

    }

}

int main(){

    sleep(1);

    pid_t pids[2];

    pids[0] = fork();
    if (pids[0] == 0) {
        char *argv2[] = {"Recv", "-x", "./Master_recv", NULL};
        int rc2 = execv("/usr/bin/gnome-terminal",argv2);
        if (rc2 == -1 )
            perror("Error at spawning Master recv");

    }

    sleep(2);
    pids[1] = fork();
    if (pids[1] == 0) {
        char *argv[] = {"Sender", "-x", "./Master_send", NULL};
        int rc = execv("/usr/bin/gnome-terminal",argv);
        if (rc == -1 )
            perror("Error at spawning Master");
    }

    sleep(5);
    while(1){

        if (isAlive() == 0){

            system("pkill -9 Master_recv");
            system("pkill -9 Master_send");

            system("gnome-terminal -x ./Main_Master");
            break;

        }

    }
    exit(0);


    return 0;
}
