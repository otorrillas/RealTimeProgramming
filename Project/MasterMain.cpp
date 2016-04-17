/*
	MasterMain.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "Communication.hpp"
#include "Communication_Master.hpp"
#include "Message.hpp"
#include "Order.hpp"
#include "LiftInfo.hpp"

#include <thread>
#include <fstream>
#include <vector>
#include <map>
#include <queue>

#define N_FLOORS 4

using namespace std;

Communication_Master commListener;
Communication_Master commSender;

map<string,string> liftMap;
vector<LiftInfo> liftInfoVec;

int last_msg_line = 0;
queue<Message> pendingMessages;
queue<Order> pendingOrders;

void read_new_messages() {
	

	bool newMessages;
	if(newMessages) {
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
        msg.sender = commSender.unmarshal(line, "from");
        // Add it to pendingMessages list
        pendingMessages.push(msg);
        // Increment line counter
        ++last_msg_line;
	}
	msg_log.close();
	}
}

Message get_new_message() {
	Message msg = pendingMessages.front();
	pendingMessages.pop();
	return msg;
	
}

Order get_new_order() {
	Order ord = pendingOrders.front();
	return ord;
}

void listener_thread() {
	if(access("Connected_lifts.txt", F_OK) != -1){
        remove("Connected_lifts.txt");
    }

    commListener.initializeReciever();
    commListener.acceptReplies();

}

void update_lift_list() {
	liftMap = commSender.checkAvailableLifts();
}

int find_in_infoVec(string id) {
	int i = 0;
	while(id != liftInfoVec[i].id) ++i;
	return i;
}

void update_idle_lift_info(Message msg) {
	int i = find_in_infoVec(msg.sender);
	liftInfoVec[i].currFloor = msg.targetFloor;
	liftInfoVec[i].direction = DIR_STOP;
	liftInfoVec[i].status = STATUS_IDLE;
}

void change_lift_status(string liftId, bool value) {
	int i = find_in_infoVec(liftId);
	liftInfoVec[i].status = value;
}

bool find_n_send_best_lift(Order order) {
	int minScore = N_FLOORS + 1;
	string bestLiftId;
	
	for(LiftInfo tmpInfo : liftInfoVec) {
		if(tmpInfo.status == STATUS_IDLE) {
			int tmpScore = order.targetFloor - tmpInfo.currFloor; 
			if(tmpScore < 0)
				tmpScore = -tmpScore;

			if(minScore > tmpScore)
				bestLiftId = tmpInfo.id;
		}
			
	}

	string send_msg = "O "
					+ to_string(order.targetFloor);

	commSender.sendOrders(send_msg.c_str(), commSender.getLiftIp(bestLiftId));
	change_lift_status(bestLiftId, STATUS_BUSY);

}

void spawn_light_notification(Message msg) {

	string send_msg = "L " 
					+ to_string(msg.targetFloor) + " "
					+ to_string(msg.buttonType) + " "
					+ to_string(msg.value);
	for(LiftInfo tmpInfo : liftInfoVec)
		if(tmpInfo.id != msg.sender)
			commSender.sendOrders(send_msg.c_str(), commSender.getLiftIp(tmpInfo.id));
}


int main(int argc,char *argv[]) {

	thread listenerThread(listener_thread);

	commSender.initializeSender();

	while(1) {
		update_lift_list();

		/* Reading messages */

		if(pendingMessages.empty())
			read_new_messages();
		else {
			Message msg = get_new_message();
			if(msg.type == "L") {
				if(msg.value == 1) {
					Order newOrd(msg.targetFloor, msg.buttonType);
					pendingOrders.push(newOrd);
				}
				spawn_light_notification(msg);
			}
			else if(msg.type == "I") {
				update_idle_lift_info(msg);
			}
		}

		/* Pending orders */
		if(!pendingOrders.empty()) {
			Order newOrd = get_new_order();

			bool success = find_n_send_best_lift(newOrd);
			// in case we could allocate the order
			// we remove it from the order queue
			if(success)
				pendingOrders.pop();
		}

	}


	return 0;
}