
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

int factorial(int vecSize) {

    int nf = 1;
    for(int i = 1; i <= vecSize; ++i) {
        nf *= i;
    }

    return nf;

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

    // Keep track of the highest scoring combination
    findScoreableWord(moveReq, tileMap);

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

// Find a scoreable word of a certain length
void Game::findScoreableWord(MoveRequest &moveReq, map<Tile *, int> &tileMap) {

    // For each square, try every possible combination of letters and compute score
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

                // Can only play above if tile below has a letter
                if (board.at(startTile->row + 1).at(startTile->col).letter.size() <= 3)
                    break;
                    

                if (tileMap[&aboveTile] > 0)
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

                // Can only play below if tile above has a letter
                if (board.at(startTile->row - 1).at(startTile->col).letter.size() <= 3)
                    break;
                    

                if (tileMap[&belowTile] > 0)
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

                // Can only play right if tile left has a letter
                if (board.at(startTile->row).at(startTile->col - 1).letter.size() <= 3)
                    break;                    

                if (tileMap[&rightTile] > 0)
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

                // Can only play left if tile right has a letter
                if (board.at(startTile->row).at(startTile->col + 1).letter.size() <= 3)
                    break;
                   

                if (tileMap[&leftTile] > 0)
                    leftLetters++;
                else
                    break;
            }

            int score = -1;
            vector<Tile> newTiles;
            int numPerms = factorial(moveReq.letters.size());

            // Ugly fix to move our tiles up/down/left/right
            int vFactor = 0;
            int hFactor = 0;

            if (aboveLetters >= belowLetters && aboveLetters >= rightLetters && aboveLetters >= leftLetters) {
                startTile->row -= aboveLetters;
                vFactor = 1;
            }
                
            else if (belowLetters > aboveLetters && belowLetters > rightLetters && belowLetters > leftLetters) {
                vFactor = 1;
            }
               
            else if (leftLetters >= belowLetters && leftLetters >= rightLetters && leftLetters >= aboveLetters) {
                startTile->col -= leftLetters;
                hFactor = 1;
            }
                
            else if (rightLetters > belowLetters && rightLetters > aboveLetters && rightLetters > leftLetters) {
                hFactor = 1;
            }
                

            for (auto n=0; n<numPerms; n++) {

                // Make random word from above starting from longest to shortest
                auto word = getRandomWord(moveReq.letters, n);

                // For each sub string of the word, get score
                for (int subSize=1; subSize<aboveLetters; subSize++) {

                    for (int subStart=0; (subStart + subSize)<aboveLetters; subStart+=subSize) {
                        vector<string> subWord(word.begin() + subStart, word.begin() + subStart + subSize);

                        // Add the random word to the board and check the score
                        for (auto i=0; i<subWord.size(); i++) {
                            auto t = Tile();
                            t.col = startTile->col + hFactor*i;
                            t.row = startTile->row + vFactor*i;
                            t.letter = subWord.at(i);
                            t.square = board.at(t.row).at(t.col).square;
                            newTiles.push_back(t);
                        };

                        score = scoreWord(newTiles, dictionary, board, letters);

                        if (score > 0) {

                            // Add word to board
                            cout << "word: ";
                            for (auto& tile: newTiles) {

                                cout << tile.letter;

                                board.at(tile.row).at(tile.col) = tile;
                            }
                            cout << "\n";

                            return;
                        }
                        else
                            newTiles = {};
                    }
                }
            };
        }
    }
}
