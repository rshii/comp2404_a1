#ifndef ITUNES_INTERFACE
#define ITUNES_INTERFACE

#include "interface.hpp"

class ItunesInterface: public Interface {

    std::vector<std::string> loggingContainerVec;
    enum class ENUM_LOG {
        OFF,
        COMMAND_ONLY,
        OUTPUT_ONLY,
        FULL
    };
    ENUM_LOG loggingMode = ENUM_LOG::OFF;

    void addSong(std::vector<std::string> args);
    void showSong(std::vector<std::string> args);
    void deleteSong(std::vector<std::string> args);

    void dot_help();
    void dot_read(std::vector<std::string> args);
    void dot_log(std::vector<std::string> args);

    void dot_trim(std::vector<std::string> args);
    void dot_startsWith(std::vector<std::string> args);
    void dot_endsWith(std::vector<std::string> args);
    void dot_toTitleCase(std::vector<std::string> args);

    public:
        std::vector<std::string> parseCommands(std::string arg);
        void executeCommands(std::vector<std::string>);
};

#endif