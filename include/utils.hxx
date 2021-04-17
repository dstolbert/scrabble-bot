
#include <string>
#include <vector>

using namespace std;

// Shared functions between objects

// Gets the index at the end of a subtring
int findIndexAtEndOfSubString(string subString, string mainString);

// Finds the substring from a given index until the first terminator is found
string getSubStrFromIndexToTerminator(int index, string str, vector<char> terminators);