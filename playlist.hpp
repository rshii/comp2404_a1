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

        int size();
        std::string getName();
        std::weak_ptr< User > getUser();
        std::weak_ptr< Track > getTrack( int i );
        void appendTrack( std::shared_ptr< Track > x, std::shared_ptr< Playlist > p );
        void delinkTrack( std::shared_ptr < Track > x );
        void purge( std::shared_ptr < Playlist >);

        friend std::ostream &operator<<( std::ostream &output, const Playlist &x );
};