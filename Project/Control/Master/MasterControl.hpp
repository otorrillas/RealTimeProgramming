/*
	MasterControl.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include <queue>

#include "Order.hpp"

using namespace std;

class MasterControl
{
public:
	MasterControl();
	void add_order(int startFloor);
	bool get_next_order(Order* new_order);
	void delete_order(Order order);

private:
	queue<Order> orderList;
	
};