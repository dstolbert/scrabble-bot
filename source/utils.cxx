
#include "utils.hxx"
#include <string>
#include <algorithm>

#include <iostream>

using namespace std;

int findIndexAtEndOfSubString(string subString, string mainString) {

    int startIdx = mainString.find(subString);

    return startIdx + subString.length() - 1;
};

// Finds the substring from a given index until the first terminator is found
string getSubStrFromIndexToTerminator(int index, string str, 
    vector<char> terminators) 
{

    // Iterate from index to the next terminator value
    int endIdx = str.length();
    for (auto i=index; i<str.length(); i++) {
        for (auto j=0; j<terminators.size(); j++) {
            
            // Break when we reach a terminator
            if (terminators[j] == str[i]) {
                endIdx = i;
                break;
            }
                
        }

        // Break when endIdx has been set
        if (endIdx == i)
            break;
            
    };

    return str.substr(index, endIdx-index);
};

// Splits a string by delimiter
vector<string> splitByDelimiter(string str, string delimiter) {

    vector<string> split;

    size_t pos = 0;
    while ((pos = str.find(delimiter)) != string::npos) {
        string token = str.substr(0, pos);
        split.push_back(token);
        str.erase(0, pos + delimiter.length());
    };

    // Add the remaining str to split
    split.push_back(str);

    return split;
};