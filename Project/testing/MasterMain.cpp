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
#include "utils.hpp"

#include <thread>
#include <vector>
#include <fstream>
#include <list>
#include <map>
#include <queue>

#define N_FLOORS 4

using namespace std;

Communication_Master commListener;
Communication_Master commSender;

map<string,string> liftMap;
list<LiftInfo> liftInfoList;

int last_msg_line = 0;
queue<Message> pendingMessages;
queue<Order> pendingOrders;


void update_lift_list() {
	
	if(access("Connected_lifts.txt", F_OK) != -1){
		//cout << "Update info lift" << endl;

		fstream msg_log;
		char line_buf[255];
		int line_count = 0;

		msg_log.open("Master_messages.txt", ios::in);

		while(!msg_log.eof()){
	        // Read the line
	    	msg_log.getline(line_buf, sizeof(line_buf));
	    	string line(line_buf);

	    	if(!line.empty()) {
	    		vector<string> splited = split(line, ' ');
	    		string id = splited[0];

	    	}
	    }


		liftMap = commSender.checkAvailableLifts();
		

		if(liftInfoList.empty()) {
			for(auto& kv : liftMap) {
				LiftInfo tmpInfo(kv.first);
				liftInfoList.insert(tmpInfo);
			}
		}

		else if (liftMap.size() > liftInfoList.size()) {
			for(auto& kv : liftMap) {
				bool b = true;
				list<LiftInfo>::iterator it = liftInfoList.begin();
				while(b and it != liftInfoList.end()) {
					if((*it).id == kv.first)
						b = false;

					++it;
				}
				if(b) {
					LiftInfo tmpInfo(kv.first);
					liftInfoList.insert(tmpInfo);
				}
			}
		}
		else if(liftMap.size() < liftInfoList.size()) {
			 list<LiftInfo> tmpInfoList = liftInfoList;

			 for(LiftInfo tmpInfo : liftInfoList) {
			 	if(liftMap.count(tmpInfo.id) == 0)
			 		tmpInfoList.remove(tmpInfo);
			 }

			 liftInfoList = tmpInfoList;
		}
		
	}

}

void read_new_messages() {
	//cout << "Reading Master_messages" << endl;
	if(access("Master_messages.txt", F_OK) != -1){
		
		fstream msg_log;
		char line_buf[255];
		int line_count = 0;

		msg_log.open("Master_messages.txt", ios::in);

		//cout << "Reading Master_messages" << endl;
	    
	    while(line_count < last_msg_line and !msg_log.eof()) {
	        msg_log.getline(line_buf, sizeof(line_buf));
	        ++line_count;
	    }
	    

	    while(!msg_log.eof()){
	        // Read the line
	    	msg_log.getline(line_buf, sizeof(line_buf));
	    	string line(line_buf);

	    	if(!line.empty()) {
	    		cout << "Line read: " << line << endl;
		        // Unmarshal the message and split it between spaces
		    	string content = commSender.unmarshal(line, "message");

		        // Translate the message
		    	if(content != "HELLO") {
		    		cout << "Pushing pending messages" << endl;
		    		update_lift_list();
		    		cout << "LiftMap.size() " << liftMap.size() << endl;
		    		Message msg(content);
		    		string senderIp = commSender.unmarshal(line, "from");
		    		for(auto& kv : liftMap) {
		    			cout << "LiftMap[" << kv.first << "]=" << kv.second << endl;
						if(kv.second == senderIp) {
							msg.sender = kv.first;
						}
					}
					// Add it to pendingMessages list
		    		pendingMessages.push(msg);
		    	}

		        // Increment line counter
		    	++last_msg_line;

	    	}

	    	
	    }
    	msg_log.close();
    	//cout << "Leaving read_new_messages" << endl;	
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



void update_idle_lift_info(Message msg) {

	for(LiftInfo tmpInfo : liftInfoList) {
	 	if(msg.sender == tmpInfo.id) {
	 		tmpInfo.currFloor = msg.targetFloor;
			tmpInfo.direction = DIR_STOP;
			tmpInfo.status = STATUS_IDLE;
	 		break;
	 	}
	 }
	
}

void update_busy_lift_info(Message msg) {
	/*
	for(LiftInfo tmpInfo : liftInfoVec)
		cout << "tmpInfo.id " << tmpInfo.id << endl;
	int i = find_in_infoVec(msg.sender);
	
	liftInfoVec[i].currFloor = msg.targetFloor;
	liftInfoVec[i].direction = msg.value;
	liftInfoVec[i].status = STATUS_BUSY;
	*/
}

void change_lift_status(string liftId, bool value) {
	
}

bool find_n_send_best_lift(Order order) {
	int minScore = N_FLOORS + 1;
	string bestLiftId;
	
	for(LiftInfo tmpInfo : liftInfo) {
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
	for(LiftInfo tmpInfo : liftInfoList)
		if(tmpInfo.id != msg.sender)
			commSender.sendOrders(send_msg.c_str(), commSender.getLiftIp(tmpInfo.id));
}


int main(int argc,char *argv[]) {

	pid_t pid = fork();
    if (pid == 0) {
        char *argv2[] = {"MasterListener", "-x", "./MasterListener", NULL};
        int rc2 = execv("/usr/bin/gnome-terminal",argv2);
        if (rc2 == -1 )
            perror("Error at spawning MasterListener");
    }

    sleep(1);

	commSender.initializeSender();

	cout << "Sender initialized" << endl;

	while(1) {
		update_lift_list();

		/* Reading messages */
		if(pendingMessages.empty())
			read_new_messages();
		else {
			Message msg = get_new_message();
			cout << "Message" << endl;
			if(msg.type == "N") {
				cout << "Light notification received" << endl;
				if(msg.value == 1) {
					Order newOrd(msg.targetFloor, msg.buttonType);
					pendingOrders.push(newOrd);
				}
				spawn_light_notification(msg);
			}
			else if(msg.type == "I") {
				update_idle_lift_info(msg);
			}
			else if(msg.type == "B") {
				cout << "Update busy info" << endl;
				update_busy_lift_info(msg);
			}
		}

		
		/* Pending orders */
		if(!pendingOrders.empty()) {
			Order newOrd = get_new_order();
			cout << "Order being processed.." << endl;

			bool success = find_n_send_best_lift(newOrd);
			// in case we could allocate the order
			// we remove it from the order queue
			if(success)
				pendingOrders.pop();
		}

	}


	return 0;
}