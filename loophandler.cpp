#include "loophandler.hpp"
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

void Loophandler::run () {
    string input;
    while (true) {
        cout << endl;
        cout << "Enter command: " << endl;
        getline(cin,input);
        if (input == ".quit") {
            break;
        }
        dataHandler->executeCommands(
            dataHandler->parseCommands(input)
        );
    }
}