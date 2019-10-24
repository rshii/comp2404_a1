#include "track.hpp"
#include "recording.hpp"
#include "playlist.hpp"
#include "song.hpp"

using std::remove;
using std::weak_ptr;
using std::shared_ptr;
using std::string;
using std::vector;

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

void Track::delinkPlaylist( shared_ptr< Playlist >  x ) {
    for (auto it = playlists.begin(); it != playlists.end(); ++it ){
        auto w = (*it).lock();
        if (w == x) {
            playlists.erase(it);
            return;
        }
    }
}

void Track::delinkRecording( shared_ptr< Recording > x ) {
    for (auto it = recordings.begin(); it != recordings.end(); ++it ){
        auto w = (*it).lock();
        if (w == x) {
            recordings.erase(it);
            return;
        }
    }
}

void Track::purgeFromPlaylistsRecordingsSong( shared_ptr< Track > x ) {
    for (auto it = recordings.begin(); it != recordings.end(); it++ ) {
        (*it).lock()->removeTrack( x );
    }
    for (auto it = playlists.begin(); it != playlists.end(); it++ ) {
        (*it).lock()->removeTrack( x );
    }
    song.lock()->removeTrack( x );
}