
#include "Game.hxx"

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

    return "";
};