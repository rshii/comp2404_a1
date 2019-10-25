#pragma once

#include "interface.hpp"
#include <memory>
#include <unordered_map>

class Track;
class Recording;
class User;
class Song;
class Playlist;

class ItunesInterface: public Interface {

    std::vector<std::string> loggingContainerVec;
    std::unordered_map< int, std::shared_ptr< Track > > tracks;
    std::unordered_map< int, std::shared_ptr< Recording > > recordings;
    std::unordered_map< std::string, std::shared_ptr< User > > users;
    std::unordered_map< int, std::shared_ptr< Song > > songs;

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