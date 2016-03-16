#ifndef Communication_master_hpp
#define Communication_master_hpp

using namespace std;

class Communication_Master : public Communication{

public:

    Communication_Master();

    Communication_Master(string sender);

    void accept_replies();


};

#endif
