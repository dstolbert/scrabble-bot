
#pragma once

#include <string>

using namespace std;

// A single letter that can be played
struct Letter {

    Letter(string json);

    int value;
    int n;
    int left;

};