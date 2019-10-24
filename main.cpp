// #include "loophandler.hpp"
// #include "itunes_interface.hpp"

// int main() {
//     ItunesInterface itunes;
//     Loophandler loop(&itunes);
//     loop.run();
//     return 0;
// }

#include "user.hpp"
#include "playlist.hpp"
#include "track.hpp"
#include "song.hpp"
#include <memory>
#include <vector>
#include <iostream>

using namespace std;

int main() {
    auto user = make_shared< User >("ted");
    user->makePlaylist("play1", user);
    user->makePlaylist("play2", user);
    user->makePlaylist("play3", user);

    auto song = make_shared< Song >("title", "composer");
    auto track = make_shared< Track >(song);
    //nullcheck
    {
        auto pl = user->getPlaylist("play1").lock();
        if (pl) {
            pl->insertTrack(0, track, pl);
        }
    }
    // auto track = make_shared< Track >(song);

    // user->getPlaylist("play1").lock()->insertTrack(0, track);
    // track->linkPlaylist(user->getPlaylist("play1").lock());


    // cout << "del below" << endl;
    // user->removePlaylist("play11");
    // user->insertPlaylist( play );
    cout << "deadmain" << endl;
}