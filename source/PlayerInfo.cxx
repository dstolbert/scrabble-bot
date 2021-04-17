
#include "PlayerInfo.hxx"
#include "utils.hxx"

#include <string>
#include <vector>

using namespace std;

PlayerInfo::PlayerInfo(string json) {

    vector<char> terminators = {',', '}'};

    // parse json here
    int tilesIndex = findIndexAtEndOfSubString("\"tilesInHand\": ", json);
    tilesInHand = stoi(getSubStrFromIndexToTerminator(tilesIndex, json, terminators));

    int pointsIndex = findIndexAtEndOfSubString("\"points\": ", json);
    points = stoi(getSubStrFromIndexToTerminator(pointsIndex, json, terminators));

};