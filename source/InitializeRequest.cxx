
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
                        // Parse from from the last opening bracket to the closing bracket we just found
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

    // Print board for confirmation
    cout << "BOARD: \n";
    for (auto i=0; i<board.size(); i++) {
        cout << "ROW: ";
        for (auto j=0; j<board.at(i).size(); j++) {
            cout << "{ square: " << board.at(i).at(j).square << ", letter: " << board.at(i).at(j).letter << " }, ";
        }
        cout << "\n";
    };



};