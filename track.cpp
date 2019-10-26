#include "track.hpp"
#include "recording.hpp"
#include "playlist.hpp"
#include "song.hpp"

using namespace std;

string Track::getURL() {
    return url;
}

weak_ptr< Song > Track::getSong() {
    return song;
}

void Track::linkPlaylist( shared_ptr< Playlist > x ) {
    playlists.push_back( x );
}

void Track::linkRecording( shared_ptr< Recording > x ) {
    recordings.push_back( x );
}

void Track::purgeTrack( std::shared_ptr< Track > x ) {
    song.lock()->delinkTrack( x );
    for (auto it = recordings.begin(); it != recordings.end(); ++it ) {
        (*it).lock()->delinkTrack( x );
    }
    for (auto it = playlists.begin(); it != playlists.end(); ++it ) {
        (*it).lock()->delinkTrack( x );
    }
}

std::ostream &operator<<( std::ostream &output, const Track &x ){
    output << "Song: " << x.song.lock()->getTitle() << endl;

    if (x.playlists.size() != 0) {
        output << "<Associated with below playlists>" << endl;
        for (auto it = x.playlists.begin(); it != x.playlists.end(); ++it ) {
            output << (*it).lock()->getName() << endl;
        }
    }
    if (x.recordings.size() != 0) {
        output << "<Associated with below recorings>" << endl;
        for (auto it = x.recordings.begin(); it != x.recordings.end(); ++it ) {
            output <<  (*it).lock()->getTitle() << endl;
        }
    }
    return output;
}