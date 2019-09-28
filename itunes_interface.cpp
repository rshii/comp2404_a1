#include "itunes_interface.hpp"
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

vector<string> ItunesInterface::parseCommands(string arg) {
    vector<string> v = ItunesInterface::normalize(arg);

    for (vector<string>::iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << endl;
    }

    // cout << normalized << endl;
    return v;
}

int ItunesInterface::executeCommands(string s) {
    return 0;
}

void ItunesInterface::dot_read(vector<string> args) {
}

void ItunesInterface::dot_add(vector<string> args) {

}

void ItunesInterface::dot_help(vector<string> args) {

}

void ItunesInterface::dot_log(vector<string> args) {

}


// Delimits args into vector
// Handles bad inputs and trims whitespace
vector<string> ItunesInterface::normalize(string arg) {
    int indexOfInterest = 0;
    vector<string> v;

    for (int i = 0; i < arg.length(); i++) {
        // quote to quote traversal
        if (arg[i] == '"') {
            indexOfInterest = i;
            for (int j = i; j < arg.length(); j++) {
                if (arg[j] == '"' && j!=i) {
                    indexOfInterest = j;
                    break;
                }
            }
            if (indexOfInterest == i) {
                cout << "ERROR: Unenclosed double quotes";
                return vector<string> ();
            }
            else if (indexOfInterest == i + 1) {
                cout << "ERROR: Empty double quoted argument";
                return vector<string> ();
            }
            else {
                v.push_back(arg.substr(i+1, indexOfInterest - i - 1));
                i = indexOfInterest;
            }
        }
        // ignoring of spaces outsides quotes traversal
        else if (arg[i] == ' ') {
            for (int j = i; j < arg.length(); j++) {
                if (arg[j] == ' ') {
                    indexOfInterest = j;
                }
                else {
                    break;
                }
            }
            i = indexOfInterest;
        }
        // unquoted unspaced character traversal
        else {
            for (int j = i; j < arg.length(); j++) {
                if (arg[j] == ' ' || arg[j] == '"') {
                    break;
                }
                else {
                    indexOfInterest = j;
                }
            }
            v.push_back(arg.substr(i, indexOfInterest - i + 1));
            i = indexOfInterest;
        }
    }

    return v;
}


bool ItunesInterface::startsWith(string arg) {
    return true;
}
bool ItunesInterface::endWith(string arg) {
    return true;
}
std::string ItunesInterface::toTitleCase(string arg) {
    return "";
}