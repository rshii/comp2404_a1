#include "loophandler.hpp"
#include "itunes_interface.hpp"

int main() {
    ItunesInterface itunes;
    Loophandler loop(&itunes);
    loop.run();
    return 0;
}