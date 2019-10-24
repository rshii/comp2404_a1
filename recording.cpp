#include "recording.hpp"

using std::string;
using std::shared_ptr;

int Recording::getYear(){
    return year;
}

string Recording::getProducer() {
    return producer;
}

string Recording::getTitle(){
    return title;
}

shared_ptr< Track > Recording::getTrack( int i ) {
    if ( i < 0 || i >= tracks.size() ) {
        return nullptr;
    }
    return tracks[i];
}

bool Recording::insertTrack( int i, shared_ptr< Track > x ) {
    if ( i < 0 || i > tracks.size() ) {
        return false;
    }
    tracks.insert( tracks.begin()+i, x );
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
    tracks.erase( std::remove( tracks.begin(), tracks.end(), x ), tracks.end() );
}