#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>
class Track;
class User;

class Playlist {
    std::string name;
    std::vector< std::weak_ptr< Track > > tracks;
    std::weak_ptr< User > user;

    public:
        Playlist( std::string name, std::shared_ptr< User > user )
          : name( name ),
            user( user )
        {
          std:: cout << "make play" << std::endl;
        }
        ~Playlist(){
          std::cout << "del play" << std::endl;
        }

        std::string getName();
        std::weak_ptr< User > getUser();
        std::weak_ptr< Track > getTrack( int i );
        int size();
        bool insertTrack( int i, std::shared_ptr< Track > x, std::shared_ptr< Playlist > p );
        bool removeTrack( int i );
        void removeTrack( std::shared_ptr< Track > x );
};