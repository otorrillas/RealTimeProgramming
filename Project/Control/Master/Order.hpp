/*
	Order.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/


class Order
{
private:
	
	int startFloor;
	int targetFloor;

public:
	Order(int startFloor);

	int get_start_floor();
	int get_target_floor();

	void set_start_floor(int startFloor);
	void set_target_floor(int targetFloor);

};