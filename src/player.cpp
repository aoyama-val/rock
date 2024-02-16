#include "common.h"
#include "player.h"
#include "main.h"
#include "math3d.h"
#include "gameobj.h"

Player::Player()
{
    advanceSpeed = 0.20;
    turnSpeed = 1.0;
    jumpPower = 2.0;

    act = STAND;
    wait = 0; 
}

int Player::loadModel(const char* md2, const char* pcx)
{
    int ret = model.load(md2, pcx);
    model.setMotion(MD2::RUN);
    return ret;
}

void Player::draw()
{
    float matAmbient[] = {0.4,0.4,0.4,1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]+0.7);
    
    // MD2 モデルは OpenGL 座標系なので座標系を戻す
    glRotatef(90, 1, 0, 0);

    if (gs.input() & KEY_RIGHT) {
        glRotatef(-20+rot[2], 0, 1, 0);
    }
    else if (gs.input() & KEY_LEFT) {
        glRotatef(20+rot[2], 0, 1, 0);
    }
    else {
        glRotatef(rot[2], 0, 1, 0);
    }
    glScalef(2,2,2);
    model.animate(0,0,0,100);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT1);
}

typedef enum {
    FRONT,
    BACK,
    LEFT,
    RIGHT, 
    FRONT_LEFT,
    FRONT_RIGHT,
    BACK_LEFT,
    BACK_RIGHT,
    NONE
} DIRECTION;

DIRECTION getMoveDirection(int input)
{
    if ((input & KEY_UP) && (input & KEY_DOWN))
        return NONE;

    if ((input & KEY_BUTTON4) && (input & KEY_BUTTON5))
        return NONE; 

    if (input & KEY_UP) {
        if (input & KEY_BUTTON4)
            return FRONT_LEFT;
        if (input & KEY_BUTTON5)
            return FRONT_RIGHT;
        return FRONT;
    }

    if (input & KEY_DOWN) {
        if (input & KEY_BUTTON4)
            return BACK_LEFT;
        if (input & KEY_BUTTON5)
            return BACK_RIGHT;
        return BACK;
    }

    if (input & KEY_BUTTON4)
        return LEFT;

    if (input & KEY_BUTTON5)
        return RIGHT;

    return NONE;
}

void Player::action()
{
    prevAct = act;
    prevFlying = flying;

    if (!flying && wait == 0)
        act = STAND; 

    if (pos[2] > gs.map.getHeight(pos[0], pos[1])+0.2) {
        flying = true;
    }
    else {
        flying = false;
    }

    if (prevFlying && !flying) {
        wait = 8;
        act = TOUCHDOWN;
    }

    if (wait > 0) {
        wait -= 1;
    }
    else {
        DIRECTION dir = getMoveDirection(gs.input());

        switch(dir) {
        case FRONT: 
            pos[0] += cos(DEG2RAD(rot[2])) * advanceSpeed;
            pos[1] += sin(DEG2RAD(rot[2])) * advanceSpeed;
            act = MOVE;
            break;
        case BACK: 
            pos[0] -= cos(DEG2RAD(rot[2])) * advanceSpeed;
            pos[1] -= sin(DEG2RAD(rot[2])) * advanceSpeed;
            act = MOVE;
            break;
        case LEFT:
            pos[0] += cos(DEG2RAD(rot[2]+90)) * advanceSpeed;
            pos[1] += sin(DEG2RAD(rot[2]+90)) * advanceSpeed;
            act = MOVE;
            break;
        case RIGHT:
            pos[0] += cos(DEG2RAD(rot[2]-90)) * advanceSpeed;
            pos[1] += sin(DEG2RAD(rot[2]-90)) * advanceSpeed;
            act = MOVE;
            break;
        case FRONT_LEFT:
            pos[0] += cos(DEG2RAD(rot[2]+45)) * advanceSpeed;
            pos[1] += sin(DEG2RAD(rot[2]+45)) * advanceSpeed;
            act = MOVE;
            break;
        case FRONT_RIGHT:
            pos[0] += cos(DEG2RAD(rot[2]-45)) * advanceSpeed;
            pos[1] += sin(DEG2RAD(rot[2]-45)) * advanceSpeed;
            act = MOVE;
            break;
        case BACK_LEFT:
            pos[0] += cos(DEG2RAD(rot[2]+135)) * advanceSpeed;
            pos[1] += sin(DEG2RAD(rot[2]+135)) * advanceSpeed;
            act = MOVE;
            break;
        case BACK_RIGHT:
            pos[0] += cos(DEG2RAD(rot[2]-135)) * advanceSpeed;
            pos[1] += sin(DEG2RAD(rot[2]-135)) * advanceSpeed;
            act = MOVE;
            break; 
        default:
            break;
        }

        if (gs.input() & KEY_LEFT) {
            rot[2] += turnSpeed;
            act = MOVE;
        }
        if (gs.input() & KEY_RIGHT) {
            rot[2] -= turnSpeed;
            act = MOVE;
        }
        if (gs.input() & KEY_BUTTON6) {
            rot[0] += turnSpeed;
            act = MOVE;
        }
        if (gs.input() & KEY_BUTTON7) {
            rot[0] -= turnSpeed;
            act = MOVE;
        }
        if (gs.input() & KEY_BUTTON1) {
            throwBall();
        }
        if (gs.input() & KEY_BUTTON2) {
            if (!flying)
                vel[2] = jumpPower;
        }
    }

    vel[2] += gs.gravity; 
    pos[2] += vel[2];
    if (pos[2] < gs.map.getHeight(pos[0], pos[1])) {
        pos[2] = gs.map.getHeight(pos[0], pos[1]);
        vel[2] = 0;
    }

    if (flying) {
        model.setAnimationRange(42, 42);
    }

    if (act != prevAct) {
        switch(act) {
        case MOVE:
            model.setMotion(MD2::RUN);
            break;
        case ATTACK:
            model.setMotion(MD2::ATTACK);
            break;
        case STAND:
            model.setFrame((int)(randDouble()*40));
            model.setMotion(MD2::STAND);
            break;
        case TOUCHDOWN:
            model.setMotion(MD2::CROUCH_STAND);
            break; 
        default:
            break;
        }
    } 

}

void Player::throwBall()
{
    float f[2];
    float bp[3];
    float x, y, z;

    gs.ball->getPos(bp);

    // 前に投げたボールがマップの内側にある間は次のを投げられない
    if (gs.map.isInside(bp[0], bp[1])) return;

    calc2DFrontVector(rot, f);
    x = pos[0] + f[0] * 1.5;
    y = pos[1] + f[1] * 1.5;
    z = pos[2]+1.0;

    gs.ball->setPos(x, y, z);
    gs.ball->setVel(f[0]*0.1, f[1]*0.1, 0.6);
    gs.ball->setAge(0);
    gs.ball->combo = 0;
    act = ATTACK;
    wait = 4;
}

