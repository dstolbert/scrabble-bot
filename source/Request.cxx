
#include "Request.hxx"
#include "utils.hxx"

#include <string>
#include <iostream>

using namespace std;

Request::Request(char* buffer) {

    // Parse buffer to string for my sanity
    string parsed(buffer);

    // Extract header
    int firstIdx = -1;
    int lastIdx = parsed.length();

    // Find http method (only checks for POST, hopefully Clay doesn't switch it up on me)
    int methodEndIdx = findIndexAtEndOfSubString("POST ", parsed);
    method = parsed.substr(0, methodEndIdx);

    // Find content length
    int contentLengthIdx = findIndexAtEndOfSubString("Content-Length: ", parsed);
    auto contentLengthStr = getSubStrFromIndexToTerminator(contentLengthIdx, parsed.substr(contentLengthIdx), vector<char>{'\n'});

    try {
        contentLength = stoi(contentLengthStr);
    } catch (...) {
        contentLength = 110000;
    };

    // Find if this is an initialization request or a make move request
    string routeStr = parsed.substr(

        // Route starts after the method declaration + a space char
        methodEndIdx + 1,

        // Route ends when we get to HTTP
        parsed.find(" HTTP/1.1") - methodEndIdx
    );

    if (routeStr.find("initialize") != string::npos)
        isInitializationRequest = true;
    else
        isInitializationRequest = false;

    // Split between header/body
    for (auto i=0; i<parsed.length(); i++) {
        const auto val = parsed[i];

        // Find first opening bracket
        if (firstIdx == -1 && val == '{') {
            firstIdx = i;
        }

        // Find last closing bracket
        else if (firstIdx >= 0 && val == '}') {
            lastIdx = i;
        };
    };

    raw = parsed;
    header = parsed.substr(0, firstIdx + 1);
    body = parsed.substr(firstIdx, lastIdx - firstIdx + 1);

};