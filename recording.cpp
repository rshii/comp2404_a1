#include "recording.hpp"

using std::string;

int Recording::getID() {
    return ID;
}
string Recording::getProducer() {
    return producer;
}
string Recording::getTitle(){
    return title;
}
int Recording::getYear(){
    return year;
}