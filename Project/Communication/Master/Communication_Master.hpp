#ifndef Communication_master_hpp
#define Communication_master_hpp

using namespace std;

class Communication_Master : public Communication{

public:


    void initializeSender();

    void initializeReciever();

    void updateConnectedLifts();

    void updateDownLifts(int sock_num);

    bool checkLiftAvailability(const char *destination);

    const char * getLiftIp(string lift_id);

    void acceptReplies();

    void sendOrders(const char *message, const char *destination);



};

#endif
