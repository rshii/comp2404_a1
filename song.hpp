#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>
class Track;

class Song {
    std::string title;
    std::string composer;
    std::vector< std::weak_ptr< Track > > tracks;

    public:
        Song( std::string title, std::string composer )
          : title( title ),
            composer( composer )
        {
            std::cout << "make song" << std::endl;
        }
        ~Song()
        {
            std::cout << "del song" << std::endl;
        }

        std::string getTitle();
        std::string getComposer();

        void addTrack( std::shared_ptr< Track > );
        void removeTrack( std::shared_ptr< Track > );
};