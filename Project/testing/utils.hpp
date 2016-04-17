/*
	utils.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/
#ifndef _UTILS_HPP
#define _UTILS_HPP
    
#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

#endif