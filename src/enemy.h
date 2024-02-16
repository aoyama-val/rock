#ifndef ENEMY_H
#define ENEMY_H

#include "md2.h"

class Enemy {
public:
    Enemy();
    int loadModel(const char* md2, const char* pcx);
    void action();
    void draw();
    void die();
    bool isAlive();
    float pos[3];
    float rot[3];
    float vel[3];
    int turn;
    int life;
protected:
    MD2 model;
    int movingCount;  // �i�s������ς���܂ł̃_�E���J�E���^
    int deadCount;    // ����ł���Đ�����܂ł̃_�E���J�E���^
};

void enemiesAction(void);

#endif
