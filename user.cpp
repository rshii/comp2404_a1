#include "user.hpp"
#include "playlist.hpp"

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::remove;
using std::weak_ptr;
using std::vector;

string User::getName() {
    return name;
}

weak_ptr< Playlist > User::getPlaylist( string playlistName ) {
    for ( auto it = playlists.begin(); it != playlists.end(); it++ ) {
        if ( ( *it )->getName() == playlistName ) {
            return *it;
        }
    }
    return weak_ptr< Playlist >();
}

void User::makePlaylist( string playlistName, shared_ptr< User > user ) {
    playlists.push_back( make_shared< Playlist > (playlistName, user));
}

void User::removePlaylist ( string playlistName ) {
    for ( auto it = playlists.begin(); it != playlists.end(); it++ ) {
        if ( ( *it )->getName() == playlistName ) {
            playlists.erase(it);
            break;
        }
    }
}