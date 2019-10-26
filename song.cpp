#include "song.hpp"
#include "track.hpp"

using namespace std;

string Song::getTitle() {
    return title;
}
string Song::getComposer() {
    return composer;
}

void Song::addTrack( shared_ptr< Track > x ) {
    tracks.push_back(x);
}

void Song::delinkTrack( std::shared_ptr < Track > x ) {
    for (auto it = tracks.begin(); it != tracks.end(); ++it ) {
        auto w = (*it).lock();
        if (w==x) {
            tracks.erase(it--);
        }
    }
}

vector< weak_ptr< Track > > Song::getTracks() {
    return tracks;
}

std::ostream &operator<<( std::ostream &output, const Song &x ){ 
            output << "Song title: " << x.title << endl;
            output << "Song Composer: " << x.composer << endl;
            return output;            
}