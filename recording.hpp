#pragma once

#include <string>
#include <memory>
#include <vector>
class Track;

class Recording {
    int year;
    std::string title;
    std::string producer;
    std::vector< std::shared_ptr < Track > > tracks;

    public:
        Recording( std::string title, std::string producer, int year )
          : title( title ),
            producer( producer ),
            year( year )
        {}

        int getYear();
        std::string getTitle();
        std::string getProducer();

        std::shared_ptr < Track > getTrack( int i );
        bool insertTrack( int i, std::shared_ptr < Track > x );
        bool removeTrack( int i );
        void removeTrack( std::shared_ptr < Track > x );
};