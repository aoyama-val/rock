#ifndef PLAYER_H
#define PLAYER_H

#include "md2.h"

class Player {
public:
    Player();
    void draw();
    void action();
    void attack();
    void throwBall();

    int loadModel(const char* md2, const char* pcx);

    float pos[3];
    float rot[3];
    float vel[3];

    float advanceSpeed; 
    float turnSpeed;
    float jumpPower;

    typedef enum {
        MOVE,
        ATTACK,
        DAMAGED,
        TOUCHDOWN,
        STAND
    } ACTION;
    ACTION act, prevAct;

    bool flying, prevFlying;

    int wait;   // 現在の act があと何フレーム続くかを示すダウンカウンタ

    MD2 model;
};

#endif
