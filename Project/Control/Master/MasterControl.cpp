/*
	MasterControl.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "MasterControl.hpp"
#include "PanelInterface.hpp"

MasterControl::MasterControl() {

}

void MasterControl::add_order(int startFloor) {
	Order tmp_order(startFloor);
	orderList.add(tmp_order);

	// look for queue.emplace()
}
void MasterControl::add_order(Order order) {
	orderList.add(order);
}

bool MasterControl::get_next_order(Order* new_order) {

	if(orderList.empty())
		return false;
	else {
		*order = orderList.front();
		orderList.pop();
	}
}