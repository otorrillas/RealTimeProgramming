#ifndef Communication_lift_hpp
#define Communication_lift_hpp

using namespace std;

class Communication_Lift : public Communication{

    int exist = 0;

    public:

    //initialize recieving part of the lift communication module
    void initializeReciever();

    //initialize sending part of the lift communication module
    void initializeSender();

    //loop waiting for the messages to come
    void acceptOrders();

    // sends messages, create the connection to appropriate lift
    void sendMessage(const char *message, const char *destination);


};

#endif
