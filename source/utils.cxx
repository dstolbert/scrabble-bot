
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

        // Break when endIdx has beens et
        if (endIdx == i)
            break;
            
    };

    return str.substr(index, endIdx-index);
};