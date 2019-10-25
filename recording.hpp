#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>
class Track;

class Recording {
    int year;
    std::string title;
    std::string producer;
    std::vector< std::weak_ptr < Track > > tracks;

    public:
        Recording( std::string title, std::string producer, int year )
          : title( title ),
            producer( producer ),
            year( year )
        {
          std::cout << "make recording" << std::endl;
        }
        ~Recording()
        {
          std::cout << "del recording" << std::endl;
        }

        int getYear();
        std::string getTitle();
        std::string getProducer();
        int size();

        std::weak_ptr < Track > getTrack( int i );
        bool insertTrack( int i, std::shared_ptr < Track > x, std::shared_ptr< Recording > p );
        bool removeTrack( int i );
        void removeTrack( std::shared_ptr < Track > x );
};