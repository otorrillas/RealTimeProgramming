#include "Communication.hpp"
#include "Communication_Master.hpp"

using namespace std;



void Communication_Master::initializeSender(){

    if(( sock_ord = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {
        perror( "socket" );
        exit( 1 );
    }

     if( setsockopt( sock_ord, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof( int ) ) == - 1 ) {
            perror( "setsockopt error" );
            exit( 1 );
        }

        ifr.ifr_addr.sa_family = AF_INET;
        strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
        ioctl(sock_ord, SIOCGIFADDR, &ifr);

        my_addr.sin_family = AF_INET;
        inet_aton(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), &my_addr.sin_addr);
        my_addr.sin_port = htons(MYPORT_MASTER);
        memset( &( my_addr.sin_zero ), '\0', 8 );

        their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons(MYPORT_LIFT);
        inet_aton("129.241.187.145", &their_addr.sin_addr);
        memset( &( their_addr.sin_zero ), '\0', 8 );

}

void Communication_Master::initializeReciever(){

        FD_ZERO( & head_set );
        FD_ZERO( & read_fds );

        if(( listener = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {

            perror( "socket error" );
            exit( 1 );
        }

        if( setsockopt( listener, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof( int ) ) == - 1 ) {
            perror( "setsockopt error" );
            exit( 1 );
        }

        ifr.ifr_addr.sa_family = AF_INET;
        strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
        ioctl(listener, SIOCGIFADDR, &ifr);

        my_addr.sin_family = AF_INET;
        inet_aton(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), &my_addr.sin_addr);
        my_addr.sin_port = htons(MYPORT_MASTER);
        memset( &( my_addr.sin_zero ), '\0', 8 );


        if( bind( listener,( struct sockaddr * ) & my_addr, sizeof( struct sockaddr ) ) == - 1 ) {
            perror( "bind error recv" );
            exit( 1 );
        }

    }

void Communication_Master::updateConnectedLifts() {

        char line_buf[255];
        vector<string> lift;

        sockets.insert(pair<string,int>(inet_ntoa(their_addr.sin_addr),socky));

        lift_file.open("Connected_lifts.txt", ios::in);

        while(!lift_file.eof()){

            lift_file.getline(line_buf, sizeof(line_buf));
            string line(line_buf);
            cout<< "SEARCHING LIFT ADDRESS: " << inet_ntoa( their_addr.sin_addr )<< endl;

            if(line.find( inet_ntoa( their_addr.sin_addr )) == -1){
                    lift.push_back(line+'\n');}
            else{
                    lift.push_back(line + " CONNECTED" + " socket: "+ to_string(socky)+'\n');

            }
        }
            lift_file.close();

            lift_file.open("Connected_lifts.txt",ios::out);
            vector<string>::iterator iter;

            for(iter=lift.begin(); iter!=lift.end();iter++){

                    lift_file << *iter;
            }

            lift_file.close();

}

void Communication_Master::updateDownLifts(int sock_num) {


    char line_buf[255];
    vector<string> lift;

    lift_file.open("Connected_lifts.txt", ios::in);

    while(!lift_file.eof()){

        lift_file.getline(line_buf, sizeof(line_buf));
        string line(line_buf);
        cout<< "ERASINIG: SEARCHING LIFT ON SOCKET: " << sock_num<< endl;

        if(line.find("socket: " + to_string(sock_num)) == -1){
            lift.push_back(line+'\n');
        }
    }
    lift_file.close();

    lift_file.open("Connected_lifts.txt",ios::out);
    vector<string>::iterator iter;

    for(iter=lift.begin(); iter!=lift.end();iter++){

        lift_file << *iter;
        lift_file.close();

    }
    lift_file.close();

}

map<string,string> Communication_Master::checkAvailableLifts(){

    char line_buf[255];
    string liftid;
    string liftip;
    lift_list.clear();

    lift_file.open("Connected_lifts.txt", ios::in);

    while(!lift_file.eof()){

    try{
        lift_file.getline(line_buf, sizeof(line_buf));
        string line(line_buf);

        if(!line.empty()){
            liftid = line.substr(0,line.find(" ", 0));
            liftip = line.substr(line.find(" ", 5)+1, (line.find(" ", 10)-line.find(" ", 5)));

        }

        }catch (const out_of_range& oor) { cout << " EMPTY LINE " << endl; }


        lift_list[liftid] = liftip;
    }
    lift_file.close();

    return lift_list;

}

const char * Communication_Master::getLiftIp(string lift_id){

    try{

        return lift_list[lift_id].c_str();

    }catch(const out_of_range& oor) { cout << " LIFT ID NOT FOUND! " << endl; }


}


void Communication_Master::acceptReplies(){

        if( listen( listener, BACKLOG ) == - 1 ) {
                perror( "listen error" );
                exit( 1 );
            }

        FD_SET( listener, & head_set );
        fdmax = listener;


    while( 1 ) {

        read_fds = head_set;
        if( select( fdmax + 1, & read_fds, NULL, NULL, NULL ) == - 1 ) {
            perror( "select" );
            exit( 1 );
        }


        for(int i = 0; i <= fdmax; i++ ) {

            if( FD_ISSET( i, & read_fds ) ) {

                if( i == listener ) {

                        sin_size = sizeof( struct sockaddr_in );


                    if(( newfd = accept( listener,( struct sockaddr * ) & their_addr, & sin_size ) ) == - 1 ) {
                        perror( "accept error" );

                    } else {
                        FD_SET( newfd, & head_set );

                            if( newfd > fdmax ) {fdmax = newfd;}

                                printf( "selectserver: new connection from %s on "
                                "socket %d\n", inet_ntoa( their_addr.sin_addr ), newfd );
                                }
                 } else {

                    if(( numbytes = recieveAll( i) ) <= 0 ) {


                        if( numbytes == 0 ) {
                            printf( "selectserver: socket %d hung up\n", i );
                           updateDownLifts(i);

                        }else {
                            perror( "recv" );
                        }
                            close( i );
                            FD_CLR( i, & head_set );

                        } else {


                            string packet(buf);
                            cout << packet << endl;

                            saveMessage("Master_messages.txt");

                            if(unmarshal(packet, "message") == "HELLO" ){

                                connected_lifts_num++;
                                string lift_num = "Lift" + to_string(connected_lifts_num);
                                lift_file.open("Connected_lifts.txt", ios::out);
                                lift_file << lift_num + " " + unmarshal(packet,"from") + '\n';
                                lift_file.close();

                                }
                            }

                        }
                    }
                }
            }
        }

void Communication_Master::sendOrders(const char *message, const char *destination){

        char line_buf[255];

        int exist = 0;

        inet_aton(destination, &their_addr.sin_addr);

        lift_file.open("Connected_lifts.txt", ios::in);



        while(!lift_file.eof()){

            lift_file.getline(line_buf, sizeof(line_buf));
            string line(line_buf);

            if(line.find(string(inet_ntoa(their_addr.sin_addr))+" CONNECTED") != -1){
                exist = 1;
            }
        }

        lift_file.close();

        cout << "exist = " << exist << endl;

        if(exist == 0){

             if(( socky = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {

                    perror( "socket error" );


                }

            if( connect( socky,( struct sockaddr * ) & their_addr, sizeof( struct sockaddr ) ) == - 1 ) {
                perror( "connect error" );
            }

            updateConnectedLifts();

        }
          map<string,int>::iterator iterat = sockets.find(inet_ntoa(their_addr.sin_addr));

          int sock = iterat->second;

          sendAll(sock, message, inet_ntoa(my_addr.sin_addr), destination);


    }





