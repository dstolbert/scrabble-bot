
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


InitializeRequest::InitializeRequest(string json) {
    
    board = parseBoard(json);

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

    cout << "WORDS: \n";
    for (auto i=0; i<words.size(); i++) {
        cout << words.at(i) << "\n";
    }

};