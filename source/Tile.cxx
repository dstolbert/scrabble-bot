
#include "Tile.hxx"
#include "utils.hxx"

#include <string>
#include <vector>
#include <iostream>

// New tile constructor from json
Tile::Tile(string json, int _row, int _col) {

    row = _row;
    col = _col;

    vector<char> terminators = {',', '}'};

    // parse json here
    auto startOfSquare = findIndexAtEndOfSubString("\"square\": ", json);
    square = getSubStrFromIndexToTerminator(startOfSquare, json, terminators);

    auto startOfLetter = findIndexAtEndOfSubString("\"letter\": ", json);
    string rawLetter = getSubStrFromIndexToTerminator(startOfLetter, json, terminators);

    // Remove any non-alpha chars from letter
    for (auto& l: rawLetter) {
        if (isalpha(l))
            letter += l;
    }
};

// Null tile constructor
Tile::Tile() {};

// Comparisons (for making map)
bool operator<(const Tile &a, const Tile &b) {
    return a.row < b.row && (a.col < b.col);
};