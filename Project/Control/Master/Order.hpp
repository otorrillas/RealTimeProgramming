/*
	Order.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/


class Order
{
	
public:
	Order(int startFloor) {
		this->startFloor = startFloor;
	}

	int targetFloor;
	int direction;

};