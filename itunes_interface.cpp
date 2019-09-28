#include "itunes_interface.hpp"
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

vector<string> ItunesInterface::parseCommands(string s) {
    string ret = ItunesInterface::normalize(s);
    cout << ret << endl;
    vector<string> v;
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

string ItunesInterface::normalize(string arg) {
    bool quoting = false;
    bool spaced = false;
    string normalized = "";
    int initIndex = 0;

    if (arg.length() == 0) {
        cout << "ERROR: arg is empty";
        return "";
    }

    if (arg[0] == ' ') {
        spaced = true;
        initIndex = 1;
    }

    for (int i = initIndex; i < arg.length(); i++) {
        if (!quoting && arg[i] == ' ' && spaced) {
            continue;
        }
        else if (!quoting && arg[i] == ' ' && !spaced) {
            normalized += arg[i];
            spaced = true;
        }
        else if (quoting && arg[i] == '"') {
            normalized += arg[i];
            quoting = false;
            spaced = false;
        }
        else if (!quoting && arg[i] == '"' && !spaced) {
            normalized += ' ';
            normalized += arg[i];
            quoting = true;
            spaced = true;
        }
        else if (!quoting && arg[i] == '"') {
            normalized += arg[i];
            quoting = true;
            spaced = false;
        }
        else {
            normalized += arg[i];
            spaced = false;
        }
    }

    if (quoting) {
        cout << "ERROR: unenclosed quotes";
        return "";
    }
    
    return normalized;
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