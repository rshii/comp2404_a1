#include "user.hpp"
#include "playlist.hpp"

using namespace std;

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

void User::removePlaylist( string playlistName ) {
    for ( auto it = playlists.begin(); it != playlists.end(); it++ ) {
        if ( ( *it )->getName() == playlistName ) {
            playlists.erase(it);
            break;
        }
    }
}

void User::clearReferences( shared_ptr< User > u ) {
    for (auto it = playlists.begin(); it != playlists.end(); ++it ) {
        (*it)->killFromSuper((*it));
    }
    playlists.clear();
}

void User::printPlaylists() {
    cout << "Owned playlists:" << endl;
    for (auto it = playlists.begin(); it != playlists.end(); ++it ) {
        cout << "- " << (*it)->getName() << endl;
    }
}

std::ostream &operator<<( std::ostream &output, const User &x ){ 
            output << "Name: " << x.name << endl;
            output << "Owned playlists:" << endl;
            for (auto it = x.playlists.begin(); it != x.playlists.end(); ++it ) {
                output << "- " << (*it)->getName() << endl;
            }
            return output;            
}