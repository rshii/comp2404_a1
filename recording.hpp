#ifndef RECORDING
#define RECORDING

#include <string>
#include <vector>

class Recording {
    int ID;
    int year;
    std::string title;
    std::string producer;
    std::vector<int> tracks;

    public:
        Recording( int ID, std::string title, std::string producer, int year )
          : ID( ID ),
            title( title ),
            producer( producer ),
            year( year )
        {}

        int getID();
        int getYear();
        std::string getTitle();
        std::string getProducer();

        bool insert( int );
        bool remove( int );
};
#endif