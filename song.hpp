#ifndef SONG
#define SONG

#include <string>

class Song {
    int ID;
    std::string title;
    std::string composer;

    public:
        Song( int ID, std::string title, std::string composer )
          : ID( ID ),
            title( title ),
            composer( composer )
        {}

        int getID();
        std::string getTitle();
        std::string getComposer();
};
#endif