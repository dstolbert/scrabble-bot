
#include "utils.hxx"
#include <string>
#include <algorithm>

#include <iostream>

using namespace std;

int findIndexAtEndOfSubString(string subString, string mainString) {

    int startIdx = mainString.find(subString);

    return startIdx + subString.length() - 1;
};

// Finds the substring from a given index until the first terminator is found
string getSubStrFromIndexToTerminator(int index, string str, 
    vector<char> terminators) 
{

    // Iterate from index to the next terminator value
    int endIdx = str.length();
    for (auto i=index; i<str.length(); i++) {
        for (auto j=0; j<terminators.size(); j++) {
            
            // Break when we reach a terminator
            if (terminators[j] == str[i]) {
                endIdx = i;
                break;
            }
                
        }

        // Break when endIdx has been set
        if (endIdx == i)
            break;
            
    };

    return str.substr(index, endIdx-index);
};

// Splits a string by delimiter
vector<string> splitByDelimiter(string str, string delimiter) {

    vector<string> split;

    size_t pos = 0;
    while ((pos = str.find(delimiter)) != string::npos) {
        string token = str.substr(0, pos);
        split.push_back(token);
        str.erase(0, pos + delimiter.length());
    };

    // Add the remaining str to split
    split.push_back(str);

    return split;
};

// Parse a vector of words from json string
vector<string> parseWords(string json, string key) {

    vector<string> words;

    // Parse words
    int startOfWords = findIndexAtEndOfSubString(key, json);

    // Track opening/closing of double quotes
    int openingQuote = -1;

    for (auto i=startOfWords; i<json.length(); i++) {

        // Start of word
        if (json.at(i) == '\"' && openingQuote < 0)
            openingQuote = i + 1;

        // End of word
        else if (json.at(i) == '\"') {
            words.push_back(json.substr(openingQuote, i - openingQuote));
            openingQuote = -1;
        }

        // End of dictionary
        else if (json.at(i) == ']')
            break;
    };

    return words;

};

// A helper to parse a single row from the JSON board
vector<Tile> parseRow (string json, int rowIdx) {

    vector<Tile> row;

    // A board row = [{square: " ", letter: "a"}, ...]
    auto split = splitByDelimiter(json, "},");
    for (auto i=0; i<split.size(); i++)
        row.push_back(Tile(split.at(i), rowIdx, i));

    return row;
};

// Parse the board object from the json string
vector<vector<Tile>> parseBoard (string json) {

    vector<vector<Tile>> board;

    // Parse board
    int startOfBoard = findIndexAtEndOfSubString("\"board\":", json);
    
    // Track opening/closing brackets
    int lastOpeningBracket = 0;
    int lastClosingBracket = 0;

    // Track if we last saw an opening or closing bracket
    bool lastUpdateClosing = false;
    int rowIdx = 0;

    for (auto i=startOfBoard; i<json.length(); i++) {

        // Found a new opening bracket
        if (json.at(i) == '[') {
            lastUpdateClosing = false;
            lastOpeningBracket = i;
        }            

        // Found the end of a row
        else if (json.at(i) == ']' && !lastUpdateClosing) {
            lastUpdateClosing = true;
            lastClosingBracket = i;
            board.push_back(
                parseRow(
                    json.substr(
                        // Get substring between opening/closing brackets
                        lastOpeningBracket, 
                        lastClosingBracket - lastOpeningBracket + 1
                        ),
                    rowIdx // row index
                )
            );

            rowIdx++;
        }

        // Found the end of the board
        else if (json.at(i) == ']' && lastUpdateClosing) {
            break;
        };
    };

    return board;
};

// Checks above, below, left, and right for open tiles
map<int, bool> getTileRef(vector<vector<Tile>> &board, Tile tile) {

    map<int, bool> tileRef;

    // Ensure we don't have empty vectors
    if (board.size() == 0 || board.at(0).size() == 0)
        return tileRef;
    
    int nRows = board.size();
    int nCols = board.at(0).size();

    // Init with all sides available
    tileRef[TileRef::ABOVE] = true;
    tileRef[TileRef::BELOW] = true;
    tileRef[TileRef::LEFT] = true;
    tileRef[TileRef::RIGHT] = true;

    // Check above
    if (tile.row > 0) {
        auto& aboveTile = board.at(tile.row - 1).at(tile.col);

        if (aboveTile.letter.size() > 0)
            tileRef[TileRef::ABOVE] = false;
    };

    // Check below
    if (tile.row < (nRows - 1)) {
        auto& belowTile = board.at(tile.row + 1).at(tile.col);

        if (belowTile.letter.size() > 0)
            tileRef[TileRef::BELOW] = false;
    }

    // Check left
    if (tile.col > 0) {
        auto& leftTile = board.at(tile.row).at(tile.col - 1);

        if (leftTile.letter.size() > 0)
            tileRef[TileRef::LEFT] = false;
    };

    // Check right
    if (tile.col < (nCols - 1)) {
        auto& rightTile = board.at(tile.row).at(tile.col + 1);

        if (rightTile.letter.size() > 0)
            tileRef[TileRef::RIGHT] = false;
    };

    return tileRef;
};

// Finds all tiles in hand that are available to be played on
map<Tile*, int> findAvailableTiles(vector<vector<Tile>> &board, int nTilesInHand) {

    // Create a map from Tile -> availability
    map<Tile*, int> tileMap;

    // Ensure we don't have empty vectors
    if (board.size() == 0 || board.at(0).size() == 0)
        return tileMap;

    int nRows = board.size();
    int nCols = board.at(0).size();

    // Iterate over all rows to find squares
    for (auto r=0; r<nRows; r++) {

        // Get this row
        auto& row = board.at(r);

        for (auto c=0; c<nCols; c++) {
            
            // Get this tile
            auto& tile = row.at(c);

            // If this tile already has a letter or we already checked it, it's not available
            if (tile.letter.size() > 0 || tileMap[&tile])
                continue;
            
            auto tileRef = getTileRef(board, tile);
            int nOpen = ( tileRef[TileRef::ABOVE] + tileRef[TileRef::BELOW] + 
                tileRef[TileRef::LEFT] + tileRef[TileRef::RIGHT] );

            // For simplicity, only make words orthogonally to existing words
            bool checkHorizontal = tileRef[TileRef::BELOW] || tileRef[TileRef::ABOVE];
            bool checkVertical = tileRef[TileRef::LEFT] || tileRef[TileRef::RIGHT];
            
            /* 
                For simplicity, only consider a tile open if it touches on just
                one side. (i.e. don't try to make more than one word at a time)            
            */
            
            if ((tile.square.find("st") != string::npos && tile.letter.size() == 0) || nOpen == 3) {

                // These are our "starting tiles" flag with a 2
                tileMap[&tile] = 2;

                if (checkVertical) {
                    // Check how many tiles above we can play
                    for (auto i=1; i<=nTilesInHand; i++) {

                        // Dont check after the end of the board
                        if ((r - i) < 0)
                            break;

                        auto& aboveTile = board.at(r - i).at(c);
                        auto aboveTileRef = getTileRef(board, aboveTile);
                        nOpen = ( aboveTileRef[TileRef::ABOVE] + aboveTileRef[TileRef::BELOW] + 
                            aboveTileRef[TileRef::LEFT] + aboveTileRef[TileRef::RIGHT] );

                        // Only keep if all 4 sides are open, stop checking otherwise
                        if (nOpen == 4)
                            tileMap[&aboveTile] = true;
                        else
                            break;
                    };

                    // Check how many tiles below we can play
                    for (auto i=1; i<=nTilesInHand; i++) {

                        // Dont check after the end of the board
                        if ((r + i) >= nRows)
                            break;

                        auto& belowTile = board.at(r + i).at(c);
                        auto belowTileRef = getTileRef(board, belowTile);
                        nOpen = ( belowTileRef[TileRef::ABOVE] + belowTileRef[TileRef::BELOW] + 
                            belowTileRef[TileRef::LEFT] + belowTileRef[TileRef::RIGHT] );

                        // Only keep if all 4 sides are open, stop checking otherwise
                        if (nOpen == 4)
                            tileMap[&belowTile] = true;
                        else
                            break;
                    };
                }
                
                if (checkHorizontal) {
                    // Check how many tiles to the right we can play
                    for (auto i=1; i<=nTilesInHand; i++) {

                        // Dont check after the end of the board
                        if ((c + i) >= nCols)
                            break;

                        auto& rightTile = board.at(r).at(c + i);
                        auto rightTileRef = getTileRef(board, rightTile);
                        nOpen = ( rightTileRef[TileRef::ABOVE] + rightTileRef[TileRef::BELOW] + 
                            rightTileRef[TileRef::LEFT] + rightTileRef[TileRef::RIGHT] );

                        // Only keep if all 4 sides are open, stop checking otherwise
                        if (nOpen == 4)
                            tileMap[&rightTile] = true;
                        else
                            break;
                    };

                    // Check how many tiles to the left we can play
                    for (auto i=1; i<=nTilesInHand; i++) {

                        // Dont check after the end of the board
                        if ((c - i) < 0)
                            break;

                        auto& leftTile = board.at(r).at(c - i);
                        auto leftTileRef = getTileRef(board, leftTile);
                        nOpen = ( leftTileRef[TileRef::ABOVE] + leftTileRef[TileRef::BELOW] + 
                            leftTileRef[TileRef::LEFT] + leftTileRef[TileRef::RIGHT] );

                        // Only keep if all 4 sides are open, stop checking otherwise
                        if (nOpen == 4)
                            tileMap[&leftTile] = true;
                        else
                            break;
                    };
                }

            }            
        }
    };

    return tileMap;
};

// A big dirty function to check if the word is valid and score it
// NOTE: This function will only check validity along the primary axis of the word
// it will also only check one tile passed the given tiles!
int scoreWord(vector<Tile> tiles, vector<string> &dictionary, 
    vector<vector<Tile>> &board, map<string, int> letterScores) {

    if (tiles.size() == 0 || board.size() == 0 || board.at(0).size() == 0)
        return -1;

    // Get nCols and nRows from board
    int nRows = board.size();
    int nCols = board.at(0).size();

    // Check if we are increasing in row or col order
    bool isRowIncreasing = tiles.size() > 1 && tiles.at(0).row != tiles.at(1).row;

    if (isRowIncreasing) {

        // Append the tiles before/after the word
        if (tiles.at(0).row - 1 >= 0) {
            auto& firstTile = tiles.at(0);

            // Grab the previous tile to check if it has a letter
            auto& prevTile = board.at(firstTile.row - 1).at(firstTile.col);

            if (prevTile.letter.size() == 0) {
                // Insert the first tile prior to the word into the vector
                tiles.insert(tiles.begin(), prevTile);
            }            
        }

        if (tiles.at(tiles.size() - 1).row + 1 < nRows) {
            auto& lastTile = tiles.at(tiles.size() - 1);

            // Grab the next tile to check if it has a letter
            auto& nextTile = board.at(lastTile.row + 1).at(lastTile.col);

            if (nextTile.letter.size() == 0) {
                // Append the tile to the end of the tiles vector
                tiles.push_back(nextTile);
            }           
        };


    } else {

        // Append the tiles before/after the word
        if (tiles.at(0).col - 1 >= 0) {
            auto& firstTile = tiles.at(0);

            // Grab the previous tile to check if it has a letter
            auto& prevTile = board.at(firstTile.row).at(firstTile.col - 1);

            if (prevTile.letter.size() == 0) {
                // Insert the first tile prior to the word into the vector
                tiles.insert(tiles.begin(), prevTile);
            }            
        }

        if (tiles.at(tiles.size() - 1).col + 1 < nCols) {
            auto& lastTile = tiles.at(tiles.size() - 1);

            // Grab the next tile to check if it has a letter
            auto& nextTile = board.at(lastTile.row).at(lastTile.col + 1);

            if (nextTile.letter.size() == 0) {
                // Append the tile to the end of the tiles vector
                tiles.push_back(nextTile);
            }           
        };
    }

    // Get the letters of the word
    string word;
    for (const auto& tile: tiles)
        word = word + tile.letter;

    // Check if the word is in the dictionary and score the word
    if (find(dictionary.begin(), dictionary.end(), word) != dictionary.end()) {

        // Use this for double/triple words after scoring everything
        int multiplier = 1;
        int score = 0;

        for (auto& tile: tiles) {
            int letterScore = letterScores[tile.letter];

            // Handle tile bonuses
            if (tile.square.find("dl") != string::npos)
                score += (letterScore * 2);
            else if (tile.square.find("tl") != string::npos)
                score += (letterScore * 3);
            else if (tile.square.find("dw") != string::npos) {
                score += letterScore;
                multiplier *= 2;
            }
            else if (tile.square.find("tw") != string::npos) {
                score += letterScore;
                multiplier *= 3;
            }
            else 
                score += letterScore;
            
        }

        return score * multiplier;
    };

    return -1;
};


// Gets random word
vector<string> getRandomWord(vector<string> letters, int seed) {

    int permIdx = 0;
    do {
        
        if (permIdx == seed)
            return vector<string> (letters.begin(), letters.end());

        permIdx++;

    } while(next_permutation(letters.begin(), letters.end()));

    return letters;
}