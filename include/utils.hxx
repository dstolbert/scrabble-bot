
#pragma once

#include "Tile.hxx"
#include <string>
#include <vector>

using namespace std;

// Shared functions between objects

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