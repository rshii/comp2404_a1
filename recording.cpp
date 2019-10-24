#include "recording.hpp"
#include "track.hpp"

using std::string;
using std::shared_ptr;
using std::weak_ptr;

int Recording::getYear(){
    return year;
}

string Recording::getProducer() {
    return producer;
}

string Recording::getTitle(){
    return title;
}

int Recording::size(){
    return tracks.size();
}

weak_ptr< Track > Recording::getTrack( int i ) {
    if ( i < 0 || i >= tracks.size() ) {
        return weak_ptr< Track >();
    }
    return tracks[i];
}

bool Recording::insertTrack( int i, shared_ptr< Track > x, shared_ptr< Recording > pl) {
    if ( i < 0 || i > tracks.size() ) {
        return false;
    }
    weak_ptr< Track > w = x;
    tracks.insert( tracks.begin()+i, w);
    x->linkRecording(pl);
    return true;
}

bool Recording::removeTrack( int i ) {
    if ( i < 0 || i >= tracks.size() ) {
        return false;
    }
    tracks.erase( tracks.begin()+i );
    return true;
}

void Recording::removeTrack( shared_ptr< Track > x ) {
    for (auto it = tracks.begin(); it != tracks.end(); ++it ) {
        auto temp = (*it).lock();
        if (temp == x) {
            tracks.erase(it--);
        }
    }
}