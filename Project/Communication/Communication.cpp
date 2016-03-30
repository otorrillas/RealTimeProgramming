#include "Communication.hpp"


using namespace std;

    int yes = 1;


void Communication::initialize_reciever(){

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

        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons( MYPORT_SEND);
        my_addr.sin_addr.s_addr = INADDR_ANY;
        memset( &( my_addr.sin_zero ), '\0', 8 );


        if( bind( listener,( struct sockaddr * ) & my_addr, sizeof( struct sockaddr ) ) == - 1 ) {
            perror( "bind error" );
            exit( 1 );
        }

    }

void Communication::initialize_sender(){

    if(( sock_ord = socket( AF_INET, SOCK_STREAM, 0 ) ) == - 1 ) {
        perror( "socket" );
        exit( 1 );
    }

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons( MYPORT_RECV );
    their_addr.sin_addr.s_addr = INADDR_ANY;
    memset( &( their_addr.sin_zero ), '\0', 8 );

    if( connect( sock_ord,( struct sockaddr * ) & their_addr, sizeof( struct sockaddr ) ) == - 1 ) {
        perror( "connect error" );
        exit( 1 );
    }

}

string Communication::marshal(const char *message, const char *from,const char *destination){

    string packet;

        int from_len = strlen(from);
        int message_len = strlen(message);
        int destination_len = strlen(destination);

        ostringstream convert;
        convert << from_len+message_len + destination_len;

        int pack_len  = strlen(convert.str().c_str());

        ostringstream ostr_packet_len;

        ostr_packet_len << from_len + message_len + destination_len + pack_len + 5;

        packet.append("$");
        packet.append(ostr_packet_len.str());
        packet.append("/");
        packet.append(from);
        packet.append("%");
        packet.append(destination);
        packet.append("<");
        packet.append(message);
        packet.append(">");

    return packet;
}

string Communication::unmarshal(string packet, string type){

    int dollar_sign = packet.find("$");
    int slash = packet.find("/");
    int percent = packet.find("%");
    int open_mark = packet.find("<");
    int close_mark = packet.find(">");

    string packet_len = packet.substr(dollar_sign, slash );
    string from = packet.substr(slash, percent );
    string destination = packet.substr(percent, open_mark );
    string message = packet.substr(open_mark, close_mark );

    if(type == "from"){return from;}
    if(type == "destination"){return destination;}
    if(type == "message"){return message;}

}

int Communication::recieve_all(int socket){

    int n;
    memset(buf,'\0', sizeof(buf));

   if((n = recv( socket, buf, sizeof(buf), 0 ))== 0){

        perror("No data recieved");
        return n = 0;

    } else {

        string packet(buf);

    // searching for the packet length which is placed beetween $ and /

        int dollar_sign = packet.find("$");
        int slash = packet.find("/");

        if(dollar_sign != -1 && slash != -1){


            if(dollar_sign > 0){

                packet.erase(0, dollar_sign);
                memset(buf,'\0', sizeof(buf));
                strcpy(buf, packet.c_str());

                dollar_sign = packet.find("$");
                slash = packet.find("/");
            }

                string packet_len = packet.substr(dollar_sign+1, slash-(dollar_sign+1) );

        // checking if recieved full packet or a part of it -> comparing bufer length with packet size
        // if not calls recieve in a loop untill gets the whole packet

                int total = strlen(buf);
                int pack_len = stoi(packet_len);

                if(total!= pack_len){

                    underflow_packet_proc = false;
                    overflow_packet_proc = false;

                } else {

                    received_messages.push_back(string(buf));
                    underflow_packet_proc = true;
                    overflow_packet_proc = true;

                    }

            while(underflow_packet_proc == false || overflow_packet_proc == false){

                while(total < pack_len){

                    int left = pack_len-total;
                    memset(buf_temp,'\0', sizeof(buf_temp));
                    n = recv( socket, buf_temp, left, 0 );
                    if(n == 0){break;}

                    left -= n;
                    total +=n;

                    strcat(buf, buf_temp);
                    memset(buf_temp,'\0', sizeof(buf_temp));

                    if(total >= pack_len){

                        underflow_packet_proc = true;
                        received_messages.push_back(string(buf));
                    }
                }

                while(total > pack_len){

                    if(dollar_sign != -1 && slash != -1){

                        packet_len = packet.substr(dollar_sign+1, slash-(dollar_sign+1) );
                        pack_len = stoi(packet_len);
                        string first_packet = packet.substr(0, pack_len);
                        received_messages.push_back(first_packet);
                        packet.erase(0,pack_len);
                        total -= strlen(first_packet.c_str());

                        packet_len = packet.substr(dollar_sign+1, slash-(dollar_sign+1) );
                        pack_len = stoi(packet_len);

                        if(total <= pack_len){

                            memset(buf,'\0', sizeof(buf));
                            strcpy(buf, packet.c_str());
                            if(total == pack_len){received_messages.push_back(string(buf));}

                            overflow_packet_proc = true;

                            }

                        if(total == pack_len){

                            underflow_packet_proc = true;
                            overflow_packet_proc = true;

                            }

                        }
                    }

                } total = 0;

            } else { perror("Recieved garbage data"); }

            if(n == 0) { return n = 0; }
            if(n == -1) { return n =-1; }
            if(n > 0) { return n;}
        }
    }

int Communication::send_all( int s, const char *message, const char *from, const char *destination)
{

    const char *buf = marshal(message,from, destination).c_str();
    unsigned int len = strlen(buf);

    int total = 0; // ile bajtów już wysłaliśmy
    int bytesleft = len; // ile jeszcze zostało do wysłania
    int n;


    while( total <  len ) {

        n = send( s, buf + total, bytesleft, 0 );
        if( n == - 1 ) { break; }
        total += n;
        bytesleft -= n;
    }

    len = total; // zwróć ilość faktycznie wysłanych bajtów

    return n ==- 1 ?- 1: 0; // zwróć -1 w przypadku błędu, 0, gdy się powiedzie
}

 void Communication::send_message(const char *message, const char *destination){

    inet_aton(destination, &their_addr.sin_addr);
    send_all( sock_ord, message, inet_ntoa(my_addr.sin_addr), destination);

    }

