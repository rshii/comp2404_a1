#ifndef TRACK
#define TRACK

#include <string>
#include <vector>

class Track {
    int ID;
    int song;
    int recording;
    std::string url;

    public:
        Track( int ID, int song, int recording = -1 )
          : ID( ID ),
            song( song ),
            recording( recording ),
            url( "temp.mp3" )
        {}

        void getID();
        void getSong();
        void getRecording();
        void getURL();
};
#endif