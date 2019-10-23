#ifndef PLAYLIST
#define PLAYLIST

#include <string>
#include <vector>

class Playlist {
    std::string name;
    std::vector<int> tracks;

    public:
        Playlist( std::string name )
          : name( name )
        {}

        bool insert( int );
        bool remove( int );
};
#endif