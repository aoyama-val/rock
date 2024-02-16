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
    int joyLeft;    // �W���C�X�e�B�b�N��臒l
    int joyRight;
    int joyUp;
    int joyDown;
    SDLKey assignedKeyToUP; // ��{�^�������������ɂȂ�L�[
    SDLKey assignedKeyToLEFT;
    SDLKey assignedKeyToDOWN;
    SDLKey assignedKeyToRIGHT;
    SDLKey assignedKeyToBUTTON[10];
};


#endif
