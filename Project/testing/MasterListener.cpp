#include "Communication.hpp"
#include "Communication_Master.hpp"

#include <iostream>

using namespace std;


int main() {

    Communication_Master m;

    cout << "MasterListener started" << endl;

    if(access("Connected_lifts.txt", F_OK) != -1){
        remove("Connected_lifts.txt");
    }

    if(access("Master_messages.txt", F_OK) != -1){
        remove("Master_messages.txt");
    }


    m.initializeReciever();
    m.acceptReplies();


	return 0;
}
