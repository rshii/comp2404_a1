#include "loophandler.hpp"
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

void Loophandler::run () {
    string input;
    while (input != "quit") {
        cout << "Enter command: ";
        getline(cin,input);
        dataHandler->parseCommands(input);
        cout << endl;
    }
}