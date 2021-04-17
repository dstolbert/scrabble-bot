
#include "Letter.hxx"
#include "utils.hxx"

#include <string>
#include <iostream>

using namespace std;

Letter::Letter(string json) {

    // Parse json here, assume that we are given everything after "letter": ... 

    // We know that the Letter json values end whenever a comma or closing bracket show up!
    vector<char> terminators = {',', '}'};

    // find value
    int startOfValue = findIndexAtEndOfSubString("\"value\": ", json);
    value = stoi(getSubStrFromIndexToTerminator(startOfValue, json, terminators));

    // find n
    int startOfN = findIndexAtEndOfSubString("\"n\": ", json);
    n = stoi(getSubStrFromIndexToTerminator(startOfN, json, terminators));

    // find left
    int startOfLeft = findIndexAtEndOfSubString("\"left\": ", json);
    left = stoi(getSubStrFromIndexToTerminator(startOfLeft, json, terminators));

};