
#include "utils.hxx"
#include "InitializeRequest.hxx"

#include <iostream>
#include <string>

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

// Parse the words (dictionary) from json string
vector<string> parseWords(string json) {

    vector<string> words;

    // Parse words
    int startOfWords = findIndexAtEndOfSubString("\"words\":", json);

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

map<string, int> parseLetters(string json) {
    
    map<string, int> letters;
    
    // Parse letter scores
    vector<char> terminators = {'}'};
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
    words = parseWords(json);
    letters = parseLetters(json);

};