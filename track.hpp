#ifndef TRACK
#define TRACK

#include <string>
#include <vector>

class Track {
    int ID;
    int song;
    std::string url;

    public:
        Track( int ID, int song )
          : ID( ID ),
            song( song ),
            url( "temp.mp3" )
        {}

        void getID();
        void getSong();
        void getURL();
};
#endif