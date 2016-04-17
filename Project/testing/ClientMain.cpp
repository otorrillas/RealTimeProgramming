/*
	ClientMain.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "Communication.hpp"
#include "Communication_Lift.hpp"
#include "Message.hpp"
#include "ClientControl.hpp"


#include "utils.hpp"

#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

#define LIGHT_OFF 0
#define LIGHT_ON 1


ClientControl contLift;

const char * server_ip;
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
        string content = commSender.unmarshal(line, "message");
               
        // Translate the message
        Message msg(content);
        
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

    if(argc > 2 and argv[2] == "L") {
        cout << "listener_thread initialized";
        listener_thread();
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        char *argv2[] = {"ClientMain", "-x", "./ClientMain", argv[1],"L", NULL};
        int rc2 = execv("/usr/bin/gnome-terminal",argv2);
        if (rc2 == -1 )
            perror("Error at spawning Master recv");
    }

    
    
    cout << "Server IP" << server_ip << endl;
	
    

    commSender.initializeReciever();
    commSender.initializeSender();

    cout << "Sender initialized" << endl;
    commSender.sendMessage("HELLO", server_ip);
    

    int prevState = STATE_BUSY;
    int prevFloor = -1;
    vector<vector<bool>> lights_state(2, vector<bool>(N_FLOORS, false));


    while(1) {
        //cout << "Starting main loop" << endl;
    	int currState = contLift.get_state();
        int currFloor = contLift.get_current_floor();
    	/* ORDERS */
    	if(currState == STATE_IDLE and prevState == STATE_BUSY) {
            //cout << "Notifying master" << endl;
            // Notify the master we are idle :)
            string send_msg = "I " + to_string(currFloor);
            commSender.sendMessage(send_msg.c_str(), server_ip);
        }
        else if(currState == STATE_BUSY) {
            if(prevFloor != currFloor) {
                string send_msg = "B "
                                + to_string(currFloor) + " "
                                + to_string(contLift.get_direction());
                commSender.sendMessage(send_msg.c_str(), server_ip);
            }
        }



        //cout << "Reading messages" << endl;
		if(pendingMessages.empty());
			//read_new_messages();
		else {
            //cout << "Getting new message" << endl;
			Message msg = get_new_message();
            if(msg.type == "O") {
            	contLift.set_target_floor(msg.targetFloor);
            }
                
            else if(msg.type == "L") {
                contLift.set_light(msg.buttonType, msg.targetFloor, msg.value);
                lights_state[msg.buttonType][msg.targetFloor] = msg.value;

            }
		}
            
    	
            

    	/* ETASJEPANELPANEL BUTTONS */


    	for(int i = 0; i < N_FLOORS; i++) {
    		for (int j = 0; j < 2; j++) {
                bool b =  contLift.is_button_active(j, i);
                int lvalue = 0;
    			if(b) {
                    // Switch on the light
                    lvalue = 1;
                    contLift.set_light(j, i, LIGHT_ON);
                    lights_state[j][i] = LIGHT_ON;
    			}
                
                if(b != lights_state[j][i]){
                    string send_msg = "N " 
                                    + to_string(i) + " " 
                                    + to_string(j) + " "
                                    + to_string(lvalue);
                    commSender.sendMessage(send_msg.c_str(), server_ip);
                    lights_state[j][i] = b;
                }
                    
    		}
    	}

    	prevState = currState;
        prevFloor = currFloor;
    	
    }
    
    return 0;

}