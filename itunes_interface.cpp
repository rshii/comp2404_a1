#include "itunes_interface.hpp"
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

vector<string> ItunesInterface::parseCommands(string arg) {
    int indexOfInterest = 0;
    vector<string> v;

    ItunesInterface::appendCommandLog(arg);

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
                cout << ERROR_OPEN_DOUBLE_QUOTE << endl;
                ItunesInterface::appendOutputLog(ERROR_OPEN_DOUBLE_QUOTE);
                return vector<string> ();
            }
            else if (indexOfInterest == i + 1) {
                cout << ERROR_EMPTY_DOUBLE_QUOTE << endl;
                ItunesInterface::appendOutputLog(ERROR_EMPTY_DOUBLE_QUOTE);
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
                //  enables parsing of comments
                else if (arg[j] == '/' && j < arg.length() - 1 && arg[j+1] == '/') {
                        if (i != j) {
                            v.push_back(arg.substr(i,j-i));
                        }
                        v.push_back(arg.substr(j,arg.length() - j));
                        return v;
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
    if (v.back().length() > 1 && v.back().substr(0,2) == "//" && readingMode == ENUM_READ::IDLE) {
        // extract and log, only used for cmdline comments, his instructions are unclear and this can be a case
        // will be processessed as a command output
        ItunesInterface::appendOutputLog(v.back());
        v.pop_back();
    }
    if (v.size() == 0) {
        return;
    }
    string cmd = v[0];
    if (cmd == "add") {
        ItunesInterface::add(v);
    }
    else if (cmd == "show") {
        ItunesInterface::show(v);
    }
    else if (cmd == "delete") {
        ItunesInterface::del(v);
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

void ItunesInterface::add(vector<string> args) {
    cout << "PARSED COMMAND:" << endl;
    if (args.size() < 2) {
        cout << "ERROR: more arguments required for add cmd" << endl;
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        cout << *it << endl;
    }
}

void ItunesInterface::show(vector<string> args) {
    cout << "PARSED COMMAND:" << endl;
    if (args.size() < 2) {
        cout << "ERROR: more arguments required for show cmd" << endl;
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        cout << *it << endl;
    }
}

void ItunesInterface::del(vector<string> args) {
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
void ItunesInterface::dot_read(vector<string> args) {

}

void ItunesInterface::dot_log(vector<string> args) {

}

void ItunesInterface::dot_trim(vector<string> args) {

}

void ItunesInterface::dot_startsWith(vector<string> args) {

}

void ItunesInterface::dot_endsWith(vector<string> args) {

}

void ItunesInterface::dot_toTitleCase(vector<string> args) {

}

void ItunesInterface::appendCommandLog(string arg) {
    if (loggingMode == ENUM_LOG::COMMAND_ONLY || loggingMode == ENUM_LOG::FULL) {
        loggingContainerVec.push_back(arg);
    }
}

void ItunesInterface::appendOutputLog(string arg) {
    if (loggingMode == ENUM_LOG::OUTPUT_ONLY || loggingMode == ENUM_LOG::FULL) {
            loggingContainerVec.push_back(arg);
    }
}