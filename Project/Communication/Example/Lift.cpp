#include "Communication.hpp"
#include "Communication_Lift.hpp"

using namespace std;


int main(int argc, const char *argv[]){

    Communication_Lift l;
    l.initializeReciever();
    l.initializeSender();
    l.sendMessage("HELLO",argv[1]);
    l.acceptOrders();

return 0;
}
