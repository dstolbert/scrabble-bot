
#pragma once

#include <string>

using namespace std;

// A single tile on the board
struct Tile 
{
    // Constructor
    Tile(string json, int _row = -1, int _col = -1);
    Tile();

    string square;
    string letter;
    
    // Position in board
    int row;
    int col;

};

// Comparison operator
bool operator<(const Tile &a, const Tile &b);

