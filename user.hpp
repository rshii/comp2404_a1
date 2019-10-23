#ifndef USER
#define USER

#include "playlist.hpp"
#include <string>
#include <vector>

class User {
    std::string ID;
    std::string name;

    public:
        User( std::string ID, std::string name )
          : ID( ID ),
            name( name )
        {}
        std::vector<Playlist> playlists;
};
#endif