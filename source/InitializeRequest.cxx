
#include "utils.hxx"
#include "InitializeRequest.hxx"

#include <iostream>
#include <string>

map<string, int> parseLetters(string json) {
    
    map<string, int> letters;
    
    // Parse letter scores
    vector<char> terminators = {']'};
    int startOfLetters = findIndexAtEndOfSubString("\"letters\":", json);
    string letterStr = getSubStrFromIndexToTerminator(startOfLetters, json, terminators);
    vector<string> letterMap = splitByDelimiter(letterStr, ",");

    for (auto i=0; i<letterMap.size(); i++) {

        int startOfLetter = -1;
        string letter;
        int val;

        for (auto j=0; j<letterMap.at(i).size(); j++) {

            // Find opening quote of letter string
            if (letterMap.at(i).at(j) == '\"' && startOfLetter < 0)
                startOfLetter = j + 1;
            
            // Find closing quote of letter string
            else if (letterMap.at(i).at(j) == '\"')
                letter = letterMap.at(i).substr(startOfLetter, j - startOfLetter);
 
            // Find numeric val
            else if (letterMap.at(i).at(j) == ':' && startOfLetter >= 0) {
                val = stoi(letterMap.at(i).substr(j + 1, letterMap.at(i).size() - j));
                break;
            }
        };

        letters[letter] = val;
    };

    return letters;
}

InitializeRequest::InitializeRequest(string json) {
    
    board = parseBoard(json);
    words = parseWords(json, "\"words\":");
    letters = parseLetters(json);

};