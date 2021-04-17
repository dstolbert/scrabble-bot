
#include "Tile.hxx"
#include "utils.hxx"

#include <string>
#include <vector>
#include <iostream>

// New tile constructor
Tile::Tile(string json) {

    vector<char> terminators = {',', '}'};

    // parse json here
    auto startOfSquare = findIndexAtEndOfSubString("\"square\": ", json);
    square = getSubStrFromIndexToTerminator(startOfSquare, json, terminators);

    auto startOfLetter = findIndexAtEndOfSubString("\"letter\": ", json);
    letter =  getSubStrFromIndexToTerminator(startOfLetter, json, terminators);

};