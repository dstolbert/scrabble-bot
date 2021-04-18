
#include "utils.hxx"
#include "MoveRequest.hxx"


#include <string>
#include <iostream>

using namespace std;

MoveRequest::MoveRequest(string json) {

    // For now, just grab the board and the tiles in hand
    board = parseBoard(json);
    letters = parseWords(json, "\"letters\":");

};