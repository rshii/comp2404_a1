#include "recording.hpp"
#include "track.hpp"
#include "song.hpp"

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

void Recording::delinkTrack( std::shared_ptr < Track > x ) {
    for (auto it = tracks.begin(); it != tracks.end(); ++it ) {
        auto w = (*it).lock();
        if (w==x) {
            tracks.erase(it--);
        }
    }
}

void Recording::purge( std::shared_ptr < Recording > x ) {
    for (auto it = tracks.begin(); it != tracks.end(); ++it ) {
        (*it).lock()->delinkRecording( x );
    }
}

std::ostream &operator<<( std::ostream &output, const Recording &x ){ 
            output << "Title: " << x.title << endl;
            output << "Artist: " << x.artist << endl;
            output << "Producer: " << x.producer << endl;
            output << "Year: " << x.year << endl;
            for ( int i = 0; i < x.tracks.size(); ++i ) {
                output << "Track " << (i+1) << ": " << x.tracks[i].lock()->getSong().lock()->getTitle() << endl;
            }
            return output;            
}