/*
	Message.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include <string>
#include "utils.hpp"

class Message
{
public:
	Message(string content) {
		vector<string> splt_content = split(content, ' '); 
		this->type = splt_content[0];
        this->targetFloor = splt_content[1];
        if(this->type == "L" or this->type == "N") {
            this->button_type = splt_content[2];
            this->value = splt_content[3];
        }
	}
	~Message();

	int targetFloor = -1;
	int buttonType = -1;
	int value = -1;
	string type;
	string sender;
};