#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>

class Interface {
    public:
        virtual std::vector<std::string> parseCommands (std::string s) = 0;
        virtual void executeCommands (std::vector<std::string> s) = 0;
};

#endif