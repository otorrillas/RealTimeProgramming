/*
	Order.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/


class Order
{
	
public:
	Order(int targetFloor, int direction) {
		this->targetFloor = targetFloor;
	}

	int targetFloor;
	int direction;

};