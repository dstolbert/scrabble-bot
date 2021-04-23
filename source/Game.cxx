
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

    // Update game
    board = moveReq.board;

    // Find all available squares to start a word
    auto tileMap = findAvailableTiles(board, moveReq.letters.size());

    // For each square, try every possible combination of letters and compute score
    vector<Tile> word;
    for (auto& it : tileMap) {

        // Find first available starting tile
        if (it.second == 2) {

            auto startTile = it.first;
            auto nRows = board.size();
            auto nCols = board.at(0).size();

            // Check how many letters we can play above start
            int aboveLetters = 0;
            for (auto i=1; i < moveReq.letters.size(); i++) {

                if (startTile->row - i < 0)
                    break;

                auto& aboveTile = board.at(startTile->row - i).at(startTile->col);

                if (tileMap[&aboveTile]  > 0)
                    aboveLetters++;
                else
                    break;
            }

            // Check how many letters we can play below start
            int belowLetters = 0;
            for (auto i=1; i < moveReq.letters.size(); i++) {

                if (startTile->row + i >= nRows)
                    break;

                auto& belowTile = board.at(startTile->row + i).at(startTile->col);

                if (tileMap[&belowTile]  > 0)
                    belowLetters++;
                else
                    break;
            }

            // Check how many letters we can play right of start
            int rightLetters = 0;
            for (auto i=1; i < moveReq.letters.size(); i++) {

                if (startTile->col + i >= nCols)
                    break;

                auto& rightTile = board.at(startTile->row).at(startTile->col + i);

                if (tileMap[&rightTile]  > 0)
                    rightLetters++;
                else
                    break;
            }

            // Check how many letters we can play left of start
            int leftLetters = 0;
            for (auto i=1; i < moveReq.letters.size(); i++) {

                if (startTile->col - i < 0)
                    break;

                auto& leftTile = board.at(startTile->row).at(startTile->col - i);

                if (tileMap[&leftTile]  > 0)
                    leftLetters++;
                else
                    break;
            }

            int score = -1;

            int nPerms = 100;
            vector<Tile> newTiles;
            for (auto n=0; n<nPerms; n++) {
                // Make random word from above starting from longest to shortest
                auto word = getRandomWord(moveReq.letters, 3, 0);

                // Add the random word to the board and check the score
                for (auto i=0; i<word.size(); i++) {
                    auto t = Tile();
                    t.col = startTile->col;
                    t.row = startTile->row - 1;
                    t.letter = word.at(i);
                    t.square = board.at(t.row).at(t.col).square;
                };

                score = scoreWord(newTiles, dictionary, board, letters);

                if (score > 0)
                    break;
            };          

            cout << "score: " << to_string(score);

            for (auto w: newTiles)
                cout << w.letter + " ";

            cout << endl;

            break;
        }
    }

    // Keep track of the highest scoring combination

    // Once we have tried every combination or run out of time, return move
    string ret = "[";

    // Go through every row
    for (auto r=0; r<board.size(); r++)  {
        ret += "[";

        // Go through every item in row
        for (auto c=0; c<board.at(r).size(); c++) {

            auto& tile = board.at(r).at(c);

            // Add tile
            string tileStr = "{sqaure: " + tile.square;

            // Add letter
            tileStr += ", letter: " + tile.letter;

            // TMP, flag if this square is available to be played or not
            tileStr += " , available: " + to_string(tileMap[&tile]);

            // Close item
            tileStr += "}";

            // Add comma if this is not the last tile in the row
            if (c < board.at(r).size() - 1)
                tileStr += ", ";
            
            // Add tile to to our return string
            ret += tileStr;
        };

        // End row
        ret += "]";

        // Add comma if this is not the last row in the board
        if (r < board.size() - 1)
            ret += ",";
    };

    ret += "]";

    return ret;
};

