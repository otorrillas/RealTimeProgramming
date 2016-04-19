#include "Message.hpp"
#include "Communication.hpp"

#include "utils.hpp"

#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

#define LIGHT_OFF 0
#define LIGHT_ON 1


//ClientControl contLift;

const char * server_ip;


int last_msg_line = 0;
queue<Message> pendingMessages;

string unmarshal(string packet, string type){
try{
    int dollar_sign = packet.find("$");
    int slash = packet.find("/");
    int percent = packet.find("%");
    int open_mark = packet.find("<");
    int close_mark = packet.find(">");

    string packet_len = packet.substr(dollar_sign+1, slash-(dollar_sign+1));
    string from = packet.substr(slash+1, percent-(slash+1) );
    string destination = packet.substr(percent+1, open_mark-(percent+1) );
    string message = packet.substr(open_mark+1, close_mark-(open_mark+1) );

    if(type == "from"){return from;}
    if(type == "destination"){return destination;}
    if(type == "message"){return message;}
}catch(const out_of_range &oor){cout << "Substring Error"<<endl;}
}


void read_new_messages() {

    if(access("Client_messages.txt", F_OK) != -1){
        fstream msg_log;
        char line_buf[255];
        int line_count = 0;

        msg_log.open("Client_messages.txt", ios::in);

        while(line_count < last_msg_line and !msg_log.eof()) {
            msg_log.getline(line_buf, sizeof(line_buf));
            ++line_count;
        }

        while(!msg_log.eof()) {
            // Read the line
            msg_log.getline(line_buf, sizeof(line_buf));
            string line(line_buf);
            if(!line.empty()) {

                // Unmarshal the message and split it between spaces
                string content = unmarshal(line, "message");

                // Translate the message
                Message msg(content);
                
                // Add it to pendingMessages list
                pendingMessages.push(msg);
                // Increment line counter
                ++last_msg_line;
            }
        }
        msg_log.close();
    }

}

Message get_new_message() { 
    Message msg = pendingMessages.front();
    pendingMessages.pop();
    return msg;
    
}

int main(int argc,char *argv[]) {

    /*
    server_ip = argv[1];
    cout << "Server IP" << server_ip << endl;
    
    pid_t pid = fork();
    if (pid == 0) {
        char *argv2[] = {"ClientListener", "-x", "./ClientListener", argv[1], NULL};
        int rc2 = execv("/usr/bin/gnome-terminal",argv2);
        if (rc2 == -1 )
            perror("Error at spawning ClientListener");
    }


    sleep(1);

    cout << "Sender initialized" << endl;
    

    commSender.initializeReciever();
    commSender.initializeSender();
    commSender.sendMessage("HELLO", server_ip);

    string send_msg = "I 2";
    commSender.sendMessage(send_msg.c_str(), server_ip);

    */ 

    while(1) {
        if(pendingMessages.empty())
            read_new_messages();
        else {
            //cout << "Getting new message" << endl;
            Message msg = get_new_message();
            if(msg.type == "O") {
                cout << "Order received" << endl;
                //contLift.set_target_floor(msg.targetFloor);
            }
                
            else if(msg.type == "L") {
                cout << "Light notification received" << endl;
                //contLift.set_light(msg.buttonType, msg.targetFloor, msg.value);
                //lights_state[msg.buttonType][msg.targetFloor] = msg.value;

            }
        }
    }
}