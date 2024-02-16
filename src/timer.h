#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class FPSTimer { 
public:
    void init();
    void wait(Uint32 time);
protected:
    Uint32 last;
};

#endif
