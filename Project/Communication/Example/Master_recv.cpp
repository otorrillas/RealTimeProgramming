#include "Communication.hpp"
#include "Communication_Master.hpp"

using namespace std;


int main(){

    Communication_Master m;

    if(access("Connected_lifts.txt", F_OK) != -1){

        remove("Connected_lifts.txt");

    }

    m.initializeReciever();
    m.acceptReplies();


return 0;
}
