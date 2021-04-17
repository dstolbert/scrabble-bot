
#include "Tile.hxx"
#include "PlayerInfo.hxx"

#include <string>
#include <vector>

using namespace std;

struct MoveRequest {

    // Construct from raw string
    MoveRequest(string json);

    vector<vector<Tile>> board;
    int tilesLeft;

    // All players in game
    vector<PlayerInfo> players;

    // Letters in my hand
    string letters;

    // My index in the players array
    int playerIndex;

};