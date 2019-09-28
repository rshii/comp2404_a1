#include "itunes_interface.hpp"
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

vector<string> ItunesInterface::parseCommands(string arg) {
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
                cout << "ERROR: Unenclosed double quotes" << endl;
                return vector<string> ();
            }
            else if (indexOfInterest == i + 1) {
                cout << "ERROR: Empty double quoted argument" << endl;
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

void ItunesInterface::executeCommands(vector<string> v) {
    if (v.size() == 0) {
        cout << "ERROR: Nothing to execute" << endl;
        return;
    }
    string cmd = v[0];
    if (cmd == "add") {
        ItunesInterface::addSong(v);
    }
    else if (cmd == "show") {
        ItunesInterface::showSong(v);
    }
    else if (cmd == "delete") {
        ItunesInterface::deleteSong(v);
    }
    else if (cmd == ".help") {
        ItunesInterface::dot_help();
    }
    else if (cmd == ".read") {
        ItunesInterface::dot_read(v);
    }
    else if (cmd == ".log") {
        ItunesInterface::dot_log(v);
    }
    else if (cmd == ".trim") {
        ItunesInterface::dot_trim(v);
    }
    else if (cmd == ".startsWith") {
        ItunesInterface::dot_startsWith(v);
    }
    else if (cmd == ".endsWith") {
        ItunesInterface::dot_endsWith(v);
    }
    else if (cmd == ".toTitleCase"){
        ItunesInterface::dot_toTitleCase(v);
    }
    else {
        cout << "ERROR: Invalid command: " << cmd << endl;
    }
}

void ItunesInterface::addSong(std::vector<std::string> args) {
    cout << "PARSED COMMAND:" << endl;
    if (args.size() < 2) {
        cout << "ERROR: more arguments required for add cmd" << endl;
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        cout << *it << endl;
    }
}

void ItunesInterface::showSong(std::vector<std::string> args) {
    cout << "PARSED COMMAND:" << endl;
    if (args.size() < 2) {
        cout << "ERROR: more arguments required for show cmd" << endl;
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        cout << *it << endl;
    }
}

void ItunesInterface::deleteSong(std::vector<std::string> args) {
    cout << "PARSED COMMAND:" << endl;
    if (args.size() < 2) {
        cout << "ERROR: more arguments required for delete cmd" << endl;
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        cout << *it << endl;
    }
}

void ItunesInterface::dot_help() {

}
void ItunesInterface::dot_read(std::vector<std::string> args) {

}

void ItunesInterface::dot_log(std::vector<std::string> args) {

}

void ItunesInterface::dot_trim(std::vector<std::string> args) {

}

void ItunesInterface::dot_startsWith(std::vector<std::string> args) {

}

void ItunesInterface::dot_endsWith(std::vector<std::string> args) {

}

void ItunesInterface::dot_toTitleCase(std::vector<std::string> args) {

}