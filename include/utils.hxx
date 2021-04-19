
#pragma once

#include "Tile.hxx"
#include <string>
#include <vector>
#include <map>

using namespace std;

// Enum to reference the tile surrounding a give tile
enum TileRef
{
    ABOVE=0,
    BELOW=1,
    LEFT=2,
    RIGHT=3
};

/* 
    Shared functions between objects
*/

// Gets the index at the end of a subtring
int findIndexAtEndOfSubString(string subString, string mainString);

// Finds the substring from a given index until the first terminator is found
string getSubStrFromIndexToTerminator(int index, string str, vector<char> terminators);

// Splits a string by delimiter
vector<string> splitByDelimiter(string str, string delimiter);

// Parse board object from json
vector<vector<Tile>> parseBoard(string json);

// Parse a vector of words from a json string
vector<string> parseWords(string json, string key); 

// Find all available tiles to play on in a board
map<Tile, bool> findAvailableTiles(vector<vector<Tile>> &board);

// Finds the number of open tiles around a give tile
map<int, bool> getTileRef(vector<vector<Tile>> &board, Tile tile);

// Finds the score of the provided word, if invalid -1
int scoreWord(vector<Tile> tiles, vector<string> &dictionary, vector<vector<Tile>> &board);