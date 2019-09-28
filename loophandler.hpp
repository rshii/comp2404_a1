#ifndef LOOPHANDLER_H
#define LOOPHANDLER_H

#include "interface.hpp"

class Loophandler {
    Interface * dataHandler;

    public:
        Loophandler(Interface * interface) {
            dataHandler = interface;
        }
        
        void run();
};
#endif