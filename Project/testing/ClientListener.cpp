#include "Communication.hpp"
#include "Communication_Lift.hpp"

using namespace std;


int main(int argc, const char *argv[]){

	cout << "ClientListener initialized" << endl; 

	if(access("Client_messages.txt", F_OK) != -1){
        remove("Client_messages.txt");
    }

    Communication_Lift l;
    l.initializeReciever();
    l.acceptOrders();

	return 0;
}
