
#pragma once

#include <string>

using namespace std;

struct Request {
    
    // Constructor    
    Request(char* buffer);

    // Full request
    string raw;

    // Header only
    string header;

    // Body only
    string body;

    // Method of the HTTP request
    string method;

    // Is this a Scrabble init request?
    bool isInitializationRequest;

};