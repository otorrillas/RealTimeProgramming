/*
	Message.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include <string>

class Message
{
public:
	Message() {
		processed = false;
	}
	~Message();

	
	int targetFloor;

};