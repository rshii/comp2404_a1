#include "playlist.hpp"
#include "track.hpp"

using std::shared_ptr;
using std::string;
using std::remove;
using std::weak_ptr;
using std::vector;

string Playlist::getName() {
    return name;
}

int Playlist::size(){
    return tracks.size();
}

weak_ptr< User > Playlist::getUser() {
    return user;
}

weak_ptr< Track > Playlist::getTrack( int i ) {
    if ( i < 0 || i >= tracks.size() ) {
        return weak_ptr < Track > () ;
    }
    return tracks[i];
}

bool Playlist::insertTrack( int i, shared_ptr< Track > x, shared_ptr< Playlist > pl) {
    if ( i < 0 || i > tracks.size() ) {
        return false;
    }
    weak_ptr< Track > w = x;
    tracks.insert( tracks.begin()+i, w);
    x->linkPlaylist(pl);
    return true;
}

bool Playlist::removeTrack( int i ) {
    if ( i < 0 || i >= tracks.size() ) {
        return false;
    }
    tracks.erase( tracks.begin()+i );
    return true;
}

void Playlist::removeTrack( shared_ptr< Track > x ) {
    for (auto it = tracks.begin(); it != tracks.end(); ++it ) {
        auto temp = (*it).lock();
        if (temp == x) {
            tracks.erase(it--);
        }
    }
}

void Playlist::killFromSuper( shared_ptr< Playlist > x ) {
    for (auto it = tracks.begin(); it != tracks.end(); ++it ) {
        (*it).lock()->delinkPlaylist( x );
    }
}