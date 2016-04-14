/*
	ClientMain.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "ClientControl.hpp"
#include "Communication_Lift.hpp"
#include "Communication.hpp"

#include <thread>
#include <fstream>
#include <vector>


using namespace std;

Communication_Lift commListener;
Communication_Lift commSender;
ClientControl contLift;
queue<Message> pendingMessages;

fstream msg_log;
int last_msg_line = 0;

string server_ip;


void listener_thread() {
	commListener.initializeReciever();
	commListener.initializeSender();
	commListener.sendMessage("HELLO", server_ip);
	commListener.acceptOrders();	
}

void read_new_messages() {
	msg_log.open("Recieved_messages.txt", ios::in);
	while(!msg_log.eof()){

		msg_log.getline(line_buf, sizeof(line_buf));
		string line(line_buf);


	}
	msg_log.close();
}

Message get_new_message() {
	Message msg = pendingMessages.front();
	pendingMessages.pop();
	return msg;
	
}

int main(int argc,char *argv[]) {

	server_ip = argv[1];

	thread listenThread(listener_thread);

    commSender.initializeReciever();
    commSender.initializeSender();
    commSender.sendMessage("HELLO",server_ip);

    Message new_message;

    while(1) {

    	/* ORDERS */
    	if(contLift.get_state == STATE_IDLE) {
    		if(pendingMessages.empty())
    			read_new_messages();
    		else {
    			new_message = get_new_message();
    			contLift.set_target_floor(new_message.targetFloor);
    		}
    	}

    	/* PANEL */

    	for(int i = 0; i < N_FLOORS; i++) {
    		for (int j = 0; j < N_BUTTONS; j++) {
    			if(panelInt.get_button_signal((PanelInterface::tag_elev_lamp_type)j, i) == 1) {
    				commSender.sendMessage("N " + , server_ip);
    			}
    		}
    	}

    	
    }
    

}