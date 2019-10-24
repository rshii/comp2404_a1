#include "song.hpp"
#include "track.hpp"

using std::shared_ptr;
using std::string;
using std::weak_ptr;

string Song::getTitle() {
    return title;
}
string Song::getComposer() {
    return composer;
}

void Song::addTrack( shared_ptr< Track > x ) {
    tracks.push_back(x);
}

void Song::removeTrack( shared_ptr< Track > x ) {
    for (auto it = tracks.begin(); it != tracks.end(); ++it ) {
        auto w = (*it).lock();
        if (w==x) {
            tracks.erase(it);
            return;
        }
    }
}