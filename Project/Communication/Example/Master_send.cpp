#include "Communication.hpp"
#include "Communication_Master.hpp"

using namespace std;


int main(){

    Communication_Master m;
    m.initializeSender();

    while(1){

        cout << m.checkAvailableLifts().size()<< endl;
        switch(m.checkAvailableLifts().size()){

            case 1: m.sendOrders("MASTER HERE1",m.getLiftIp("Lift1"));
            //case 2: { m.sendOrders("MASTER HERE1",m.getLiftIp("Lift1"));
              //          m.sendOrders("MASTER HERE1",m.getLiftIp("Lift2"));}
        }


        map<string,string>::iterator iteri;

        for(iteri = m.lift_list.begin(); iteri != m.lift_list.end(); iteri++ ){
        cout << "Map content: " << iteri->first << " "<< iteri->second << endl;
        }
    }

return 0;
}
