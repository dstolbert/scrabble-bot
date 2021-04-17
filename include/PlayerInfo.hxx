
#include <string>

using namespace std;

// Info about each player
struct PlayerInfo {

    PlayerInfo(string json);

    int tilesInHand;
    int points;

};
