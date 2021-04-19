
#include "Game.hxx"
#include "utils.hxx"

#include <iostream>

// Constructor
Game::Game() {}

// Parse request object to initialize game
string Game::update(Request req) {

    std::cout << "updating game\n";

    // Parse initialization request
    auto init = InitializeRequest(req.body);

    // Update game
    board = init.board;
    dictionary = init.words;
    letters = init.letters;

    // Create JSON response body
    return "{ \"status\": \"Alpha-Master-Scrabble-Bot is ready\" }";

};


// Generate the next scrabble move
string Game::makeMove(Request req) {

    std::cout << "making a move \n";

    // Parse move request
    auto moveReq = MoveRequest(req.body);

    // Find all available squares to start a word
    auto tileMap = findAvailableTiles(board);

    // For each square, try every possible combination of letters and compute score

    // Keep track of the highest scoring combination

    // Once we have tried every combination or run out of time, return move

    return "";
};

