#include "song.hpp"

using std::string;

int Song::getID() {
    return songID;
}
string Song::getTitle() {
    return title;
}
string Song::getComposer() {
    return composer;
}