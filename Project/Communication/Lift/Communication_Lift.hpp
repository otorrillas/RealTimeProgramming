#ifndef Communication_lift_hpp
#define Communication_lift_hpp

using namespace std;

class Communication_Lift : public Communication{

    public:

    void initializeReciever();

    void initializeSender();

    void acceptOrders();

    void sendMessage(const char *message, const char *destination);


};

#endif
