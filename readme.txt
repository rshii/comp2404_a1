README

Code authored by: Rishi Sharma 101011443

DECOMPRESS ZIP

unzip mytunes.zip
cd comp2404_a1


TO COMPILE
either use:

source sourceMe

or

g++ main.cpp itunes_interface.cpp loophandler.cpp -std=c++11

NOTICE: interface differs from what professor provided due to
        really terrible specifications
        for example: there is no delete -t track_id
        reason: there is never any reference of track_id
        anywhere else


TO RUN TEST SCRIPT AND LOG TO FILE
./a.out
.read demoScript
.log save log
.quit
vim log
