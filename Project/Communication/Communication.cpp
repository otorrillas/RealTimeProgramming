#include "Communication.hpp"


using namespace std;

    int yes = 1;


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
try{
    int dollar_sign = packet.find("$");
    int slash = packet.find("/");
    int percent = packet.find("%");
    int open_mark = packet.find("<");
    int close_mark = packet.find(">");

    string packet_len = packet.substr(dollar_sign+1, slash-(dollar_sign+1));
    string from = packet.substr(slash+1, percent-(slash+1) );
    string destination = packet.substr(percent+1, open_mark-(percent+1) );
    string message = packet.substr(open_mark+1, close_mark-(open_mark+1) );

    if(type == "from"){return from;}
    if(type == "destination"){return destination;}
    if(type == "message"){return message;}
}catch(const out_of_range &oor){cout << "Substring Error"<<endl;}
}

int Communication::recieveAll(int socket){

    int n;
    memset(buf,'\0', sizeof(buf));

   if((n = recv( socket, buf, sizeof(buf), 0 ))== 0){

        perror("No data recieved");
        return n = 0;

    } else {

        string packet(buf);
        //cout << "PO pierwszym recv:" << packet << endl;

    // searching for the packet length which is placed beetween $ and /

        int dollar_sign = packet.find("$");
        int slash = packet.find("/");

        if(dollar_sign != -1 && slash != -1 && dollar_sign < slash){


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
        try{
            try{
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

                    if(dollar_sign != -1 && slash != -1 && dollar_sign < slash){

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

                        }else{break;}
                    }

                } total = 0;
                }catch(const invalid_argument &ia){cout << "Stoi Error"<<endl;}
              }catch(const out_of_range &oor){cout << "Substring Error"<<endl;}
            } else { perror("Recieved garbage data"); }

            //typedef vector<string>::const_iterator VecIterator;
           //int i = 1;
            //for (VecIterator iter = received_messages.begin(); iter != received_messages.end(); iter++){
            //cout << i << ": " <<*iter << endl;
            //i++;
            //}

            if(n == 0) { return n = 0; }
            if(n == -1) { return n =-1; }
            if(n > 0) { return n;}
        }
    }

int Communication::sendAll( int s, const char *message, const char *from, const char *destination)
{

    const char *buf = marshal(message,from, destination).c_str();
    unsigned int len = strlen(buf);

    int total = 0;
    int bytesleft = len;
    int n;


    while( total <  len ) {

        n = send( s, buf, bytesleft, 0 );
        if( n == - 1 ) { break; }
        total += n;
        bytesleft -= n;
    }

    len = total;

    return n ==- 1 ?- 1: 0;
}








