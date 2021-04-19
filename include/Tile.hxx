
#pragma once

#include <string>

using namespace std;

// A single tile on the board
struct Tile 
{
    // Constructor
    Tile(string json, int row = -1, int col = -1);

    string square;
    string letter;
    
    // Position in board
    int row;
    int col;

};

