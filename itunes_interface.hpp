#ifndef ITUNES_INTERFACE
#define ITUNES_INTERFACE

#include "interface.hpp"

class ItunesInterface: public Interface {

    std::vector<std::string> loggingContainerVec;

    void dot_read(std::vector<std::string> args);
    void dot_add(std::vector<std::string> args);
    void dot_help(std::vector<std::string> args);
    void dot_log(std::vector<std::string> args);

    std::string normalize(std::string arg);
    bool startsWith(std::string arg);
    bool endWith(std::string arg);
    std::string toTitleCase(std::string arg);

    public:
        std::vector<std::string> parseCommands(std::string s);
        int executeCommands(std::string  s);
};

#endif