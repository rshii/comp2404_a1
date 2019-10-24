#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>
class Playlist;
class Recording;
class Song;

class Track {
    std::string url;
    std::vector< std::weak_ptr< Playlist > > playlists;
    std::vector< std::weak_ptr< Recording > > recordings;
    std::weak_ptr< Song > song;

    public:
        Track(std::shared_ptr< Song > song)
          : url( "temp.mp3" ),
            song(song)
        {
            std::cout << "make track" << std::endl;
        }
        ~Track()
        {
            std::cout << "del track" << std::endl;
        }

        std::string getURL();

        std::weak_ptr< Song > getSong();
        void linkPlaylist( std::shared_ptr< Playlist > );
        void linkRecording( std::shared_ptr< Recording > );
        void delinkPlaylist( std::shared_ptr< Playlist >  );
        void delinkRecording( std::shared_ptr< Recording > );
        std::vector< std::weak_ptr< Playlist > >  getLinkedPlaylists();
        std::vector< std::weak_ptr< Recording > >  getLinkedRecordings();
};