/*
	ClientMain.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "ClientControl.hpp"
#include "Communication_Lift.hpp"
#include "Communication.hpp"
#include "Message.hpp"

#include "utils.hpp"

#include <thread>
#include <fstream>
#include <vector>

using namespace std;

#define LIGHT_OFF 0
#define LIGHT_ON 1


ClientControl contLift;

string server_ip;
Communication_Lift commListener;
Communication_Lift commSender;

int last_msg_line = 0;
queue<Message> pendingMessages;


void listener_thread() {
	commListener.initializeReciever();
	commListener.initializeSender();
	commListener.sendMessage("HELLO", server_ip);
	commListener.acceptOrders();	
}

void read_new_messages() {

    fstream msg_log;
    char line_buf[255];
    int line_count = 0;

    msg_log.open("Recieved_messages.txt", ios::in);

    while(line_count < last_msg_line and !msg_log.eof()) {
        msg_log.getline(line_buf, sizeof(line_buf));
        ++line_count;
    }

	while(!msg_log.eof()){
        // Read the line
		msg_log.getline(line_buf, sizeof(line_buf));
		string line(line_buf);

        // Unmarshal the message and split it between spaces
        string content = commSender.unmarshal(packet, "message");
        vector<string> splt_content = split(content, ' ');        

        // Translate the message
        Message msg;
        msg.type = splt_content[0];
        msg.targetFloor = splt_content[1];
        if(msg.type == "L") {
            msg.button_type = splt_content[2];
            msg.value = splt_content[3];
        }

        // Add it to pendingMessages list
        pendingMessages.push(msg);
        // Increment line counter
        ++last_msg_line;
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
    commSender.sendMessage("HELLO", server_ip);

    int prev_state = STATE_IDLE;


    while(1) {

    	/* ORDERS */
    	if(contLift.get_state == STATE_IDLE) {
            if(prev_state == STATE_BUSY) {
                // Notify the master we are idle :)
                string send_msg = "I " + to_string(contLift.get_current_floor());
                commSender.sendMessage(send_msg, server_ip);
            }
    		if(pendingMessages.empty())
    			read_new_messages();
    		else {
    			Message msg = get_new_message();
                if(msg.type == "O")
                    contLift.set_target_floor(msg.targetFloor);
                else if(msg.type == "L")
                    contLift.set_light(msg.button_type, msg.targetFloor, msg.value);
    		}
            prev_state = STATE_IDLE;
    	}
        else
            prev_state = STATE_BUSY;

    	/* ETASJEPANELPANEL */


    	for(int i = 0; i < N_FLOORS; i++) {
    		for (int j = 0; j < 2; j++) { 
    			if(contLift.is_button_active(j, i)) {
                    // Switch on the light
                    contLift.set_light(j, i, LIGHT_ON);
                    // Notify the server to spawn it
                    string send_msg = "N " + to_string(i) + " " + to_string(j);
    				commSender.sendMessage(send_msg, server_ip);

    			}
    		}
    	}

    	
    }
    

}