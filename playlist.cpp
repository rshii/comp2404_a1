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

void Playlist::appendTrack( shared_ptr< Track > x, shared_ptr< Playlist > pl) {
    weak_ptr< Track > w = x;
    tracks.push_back( w );
    x->linkPlaylist( pl );
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