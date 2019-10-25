#include "recording.hpp"
#include "track.hpp"

using namespace std;

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

std::ostream &operator<<( std::ostream &output, const Recording &x ){ 
            output << "Title: " << x.title << endl;
            output << "Artist: " << x.artist << endl;
            output << "Producer: " << x.producer << endl;
            output << "Year: " << x.year << endl;
            return output;            
}