#ifndef Communication_master_hpp
#define Communication_master_hpp

#include <map>

using namespace std;

class Communication_Master : public Communication{

public:

    //initialize sending part of the master communication module
    void initializeSender();

    //initialize recieving part of the master communication module
    void initializeReciever();

    //keep trakcs of the incoming connections, updates the file when new come
    void updateConnectedLifts();

    //keep trakcs of the  connections, updates the file when the already connected lift goes down
    void updateDownLifts(int sock_num);

    //checks for available lifts and returns map containing Lift ID and corresponding IP
    map<string,string> checkAvailableLifts();

    //return Lift IP based on its ID
    const char * getLiftIp(string lift_id);

    //loop waiting for the messages to come, store them, accepts new connections
    void acceptReplies();

    // sends messages, create the connection to appropriate lift
    void sendOrders(const char *message, const char *destination);



};

#endif
