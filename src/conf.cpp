#include "common.h"
#include "conf.h"

void Config::reset()
{
    sound = 1;
    music = 1;
    useJoystick = 0;
    fullscreen = 0;
    musicVolume = 128;
    soundVolume = 128;
    joyLeft = -16383;
    joyRight = 16383;
    joyUp = -16383;
    joyDown = 16383;
}


void Config::resetKeyAssign()
{
    assignedKeyToUP    = SDLK_w;
    assignedKeyToLEFT  = SDLK_a;
    assignedKeyToDOWN  = SDLK_s;
    assignedKeyToRIGHT = SDLK_d;
    assignedKeyToBUTTON[0] = SDLK_i;
    assignedKeyToBUTTON[1] = SDLK_j;
    assignedKeyToBUTTON[2] = SDLK_k;
    assignedKeyToBUTTON[3] = SDLK_l;
    assignedKeyToBUTTON[4] = SDLK_q;
    assignedKeyToBUTTON[5] = SDLK_e;
    assignedKeyToBUTTON[6] = SDLK_z;
    assignedKeyToBUTTON[7] = SDLK_c;
    assignedKeyToBUTTON[8] = SDLK_v;
    assignedKeyToBUTTON[9] = SDLK_b;
}

int Config::load(const char* filename)
{
    char buf[1024];
    char buf2[1024];
    int n;
    FILE* fp = fopen(filename, "r");

    if (fp == NULL)
        return -1;

    while(fgets(buf, sizeof(buf), fp)) {
        if (startswith(buf, "joystick"))
            useJoystick = true;
        else if (startswith(buf, "nojoystick"))
            useJoystick = false;
        else if (startswith(buf, "fullscreen"))
            fullscreen = true;
        else if (startswith(buf, "nofullscreen"))
            fullscreen = false; 
        else if (startswith(buf, "joyLeft")) {
            sscanf(buf, "%s %d", buf2, &n);
            joyLeft = n;
        }
        else if (startswith(buf, "joyRight")) {
            sscanf(buf, "%s %d", buf2, &n);
            joyRight = n;
        }
        else if (startswith(buf, "joyUp")) {
            sscanf(buf, "%s %d", buf2, &n);
            joyUp = n;
        }
        else if (startswith(buf, "joyDown")) {
            sscanf(buf, "%s %d", buf2, &n);
            joyDown = n;
        }
    }

    return 0; 
}
