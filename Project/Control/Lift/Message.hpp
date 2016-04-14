/*
	Message.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include <string>

class Message
{
public:
	Message();
	~Message();

	string type;
	int targetFloor = -1;
	int button_type = -1;
	int value = -1;
	
};