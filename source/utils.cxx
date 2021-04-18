
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

// Parse a vector of words from json string
vector<string> parseWords(string json, string key) {

    vector<string> words;

    // Parse words
    int startOfWords = findIndexAtEndOfSubString(key, json);

    // Track opening/closing of double quotes
    int openingQuote = -1;

    for (auto i=startOfWords; i<json.length(); i++) {

        // Start of word
        if (json.at(i) == '\"' && openingQuote < 0)
            openingQuote = i + 1;

        // End of word
        else if (json.at(i) == '\"') {
            words.push_back(json.substr(openingQuote, i - openingQuote));
            openingQuote = -1;
        }

        // End of dictionary
        else if (json.at(i) == ']')
            break;
    };

    return words;

};

// A helper to parse a single row from the JSON board
vector<Tile> parseRow (string json) {

    vector<Tile> row;

    // A board row = [{square: " ", letter: "a"}, ...]
    auto split = splitByDelimiter(json, "},");
    for (auto i=0; i<split.size(); i++)
        row.push_back(Tile(split.at(i)));

    return row;
};

// Parse the board object from the json string
vector<vector<Tile>> parseBoard (string json) {

    vector<vector<Tile>> board;

    // Parse board
    int startOfBoard = findIndexAtEndOfSubString("\"board\":", json);
    
    // Track opening/closing brackets
    int lastOpeningBracket = 0;
    int lastClosingBracket = 0;

    // Track if we last saw an opening or closing bracket
    bool lastUpdateClosing = false;

    for (auto i=startOfBoard; i<json.length(); i++) {

        // Found a new opening bracket
        if (json.at(i) == '[') {
            lastUpdateClosing = false;
            lastOpeningBracket = i;
        }            

        // Found the end of a row
        else if (json.at(i) == ']' && !lastUpdateClosing) {
            lastUpdateClosing = true;
            lastClosingBracket = i;
            board.push_back(
                parseRow(
                    json.substr(
                        // Get substring between opening/closing brackets
                        lastOpeningBracket, 
                        lastClosingBracket - lastOpeningBracket + 1
            )));
        }

        // Found the end of the board
        else if (json.at(i) == ']' && lastUpdateClosing) {
            break;
        };
    };

    return board;
};