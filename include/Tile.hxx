
#pragma once

#include <string>

using namespace std;

// A single tile on the board
struct Tile 
{
    // Constructor
    Tile(string json);

    string square;
    string letter;

};

