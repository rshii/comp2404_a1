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
    enum class ENUM_READ {
        READING,
        IDLE
    };
    ENUM_LOG loggingMode = ENUM_LOG::OFF;
    ENUM_READ readingMode = ENUM_READ::IDLE;

    const std::string ERROR_OPEN_DOUBLE_QUOTE = "ERROR: Unenclosed double quotes";
    const std::string ERROR_EMPTY_DOUBLE_QUOTE = "ERROR: Empty double quoted argument";

    void add(std::vector<std::string> args);
    void show(std::vector<std::string> args);
    void del(std::vector<std::string> args);

    void dot_help();
    void dot_read(std::vector<std::string> args);
    void dot_log(std::vector<std::string> args);
    void appendCommandLog(std::string arg);
    void appendOutputLog(std::string arg);

    void dot_trim(std::vector<std::string> args);
    void dot_startsWith(std::vector<std::string> args);
    void dot_endsWith(std::vector<std::string> args);
    void dot_toTitleCase(std::vector<std::string> args);

    public:
        std::vector<std::string> parseCommands(std::string arg);
        void executeCommands(std::vector<std::string>);
};

#endif