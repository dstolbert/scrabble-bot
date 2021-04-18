
#pragma once

#include "Tile.hxx"
#include "Letter.hxx"

#include <string>
#include <vector>
#include <map>

struct InitializeRequest
{
    /* Request object sent during the scrabble initialization */
    InitializeRequest(string json);

    vector<vector<Tile>> board;
    vector<string> words;
    map<string, int> letters;

};
