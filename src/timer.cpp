#include "timer.h"

void FPSTimer::init()
{
    last = SDL_GetTicks();
}

void FPSTimer::wait(Uint32 time)
{
    Uint32 now = SDL_GetTicks();

    if (now - last < time)
        SDL_Delay(time-(now-last));
    last = SDL_GetTicks();
}

