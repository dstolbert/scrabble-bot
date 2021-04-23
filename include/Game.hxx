
#pragma once

#include "Request.hxx"
#include "InitializeRequest.hxx"
#include "MoveRequest.hxx"

#include <string>
#include <map>
#include <vector>

using namespace std;


// A singleton to hold all game data
class Game {

    public:

        // Init
        Game();

        // Update game when initialization request comes in
        string update(Request req);
        
        // Generate next move as a JSON formatted string
        string makeMove(Request req);

    private:
        // Timeout (ms)
        int timeToProcess = 1000;

        // Current state of the board
        vector<vector<Tile>> board;

        // All valid words
        vector<string> dictionary;

        // Map letters to points
        map<string, int> letters;

};
