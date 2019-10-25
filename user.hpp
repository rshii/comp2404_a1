#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <vector>
class Playlist;

class User {
    // a playlist can only exist as owned by a single user, can used shared_ptr
    std::string name;
    std::vector< std::shared_ptr< Playlist > > playlists;

    public:
        User( std::string name )
          : name( name )
        {
            std::cout << "make user" << std::endl;
        }

        ~User() 
        {
            std::cout << "del user" << std::endl;
        }

        std::string getName();
        std::weak_ptr< Playlist > getPlaylist( std::string playlistName );
        void printPlaylists();
        void makePlaylist( std::string playlistName, std::shared_ptr< User > user );
        void removePlaylist ( std::string playlistName ); 
        void clearReferences( std::shared_ptr< User > );

        friend std::ostream &operator<<( std::ostream &output, const User &x );
};