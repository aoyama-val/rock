#ifndef CONF_H
#define CONF_H

#include <SDL.h>

class Config{
public:
    void reset();
    void resetKeyAssign();
    int load(const char* filename);

    int useJoystick;
    int fullscreen;
    int sound;
    int music;
    int soundVolume;
    int musicVolume;
    int joyLeft;    // ジョイスティックの閾値
    int joyRight;
    int joyUp;
    int joyDown;
    SDLKey assignedKeyToUP; // 上ボタンを押した事になるキー
    SDLKey assignedKeyToLEFT;
    SDLKey assignedKeyToDOWN;
    SDLKey assignedKeyToRIGHT;
    SDLKey assignedKeyToBUTTON[10];
};


#endif
