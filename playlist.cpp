#include "playlist.hpp"
#include "track.hpp"
#include "user.hpp"
#include "song.hpp"

using namespace std;

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

vector< weak_ptr< Track > > Playlist::getAllTracks() {
    return tracks;
}

void Playlist::appendTrack( shared_ptr< Track > x, shared_ptr< Playlist > pl) {
    weak_ptr< Track > w = x;
    tracks.push_back( w );
    x->linkPlaylist( pl );
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

std::ostream &operator<<( std::ostream &output, const Playlist &x ){ 
            output << "Name: " << x.name << endl;
            output << "Owned by: " << x.user.lock()->getName() << endl;
            output << "<Tracks added>" << endl;
            for (auto it = x.tracks.begin(); it != x.tracks.end(); ++it ){
                output << (*it).lock()->getSong().lock()->getTitle() << endl;
            }
            return output;            
}