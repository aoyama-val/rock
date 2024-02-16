#include "common.h"
#include "enemy.h"
#include "display.h"
#include "math3d.h"
#include "gameobj.h"

#include "md2.h"

Enemy::Enemy()
{
    life = 0;
    turn = 0;
    movingCount = 0;
    deadCount = 0;
}

bool Enemy::isAlive()
{ 
    return (deadCount <= 0) && (life > 0);
}

int Enemy::loadModel(const char* md2, const char* pcx)
{
    int ret = model.load(md2, pcx);
    model.setMotion(MD2::RUN);
    return ret;
} 

void Enemy::action()
{
    float b1[3], b2[3];
    float nextDest[2];
    float toNext[2];

    // 死んだ後 新しい位置に出現
    if (deadCount == 1) {
        float u[2], v[2];
        gs.map.getBoundary(u, v);
        pos[0] = v[0] * randDouble();
        pos[1] = v[1] * randDouble();
        pos[2] = gs.map.getHeight(pos[0], pos[2]);
        model.setMotion(MD2::RUN);
        model.setRepeat(true);
    }

    if (deadCount > 0) {
        deadCount -= 1;
        return;
    }

    // 進行方向を変える
    if (movingCount <= 0) { 
        gs.map.getBoundary(b1, b2); 
        nextDest[0] = pos[0] + (1.0 + 5*randDouble());
        nextDest[1] = pos[1] + (1.0 + 5*randDouble());
        if (!gs.map.isInside(nextDest[0], nextDest[1])) {
            nextDest[0] = randDouble() * 70;
            nextDest[1] = randDouble() * 70;
        }

        toNext[0] = nextDest[0] - pos[0];
        toNext[1] = nextDest[1] - pos[1];
        float d = sqrt(toNext[0]*toNext[0]+toNext[1]*toNext[1]);
        toNext[0] /= d;
        toNext[1] /= d;
        float v = (0.1 + 1.0 * randDouble()) * 0.33;
        vel[0] = toNext[0] * v;
        vel[1] = toNext[1] * v;
        rot[1] = RAD2DEG(atan2(vel[1], vel[0]));

        movingCount = (int)(d / v);
    }
    else {
        movingCount -= 1;
    }
    pos[0] += vel[0];
    pos[1] += vel[1];
    pos[2] = gs.map.getHeight(pos[0], pos[1]);
}

void Enemy::draw()
{ 
    if (0 < deadCount && deadCount < 60)
        return;

    float matAmbient[] = {0.4,0.4,0.4,1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]+1.5);
    glRotatef(90, 1, 0, 0);
    glRotatef(rot[1], 0, 1, 0);
    glScalef(4,4,4);
    model.animate(0,0,0,100);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT1);
}

void Enemy::die()
{

    movingCount = 0;
    deadCount = 90;
    model.setMotion(MD2::DEATH1);
    model.setRepeat(false);
}

void enemiesAction(void)
{
    int i;

    for (i=0; i<MAX_ENEMY; i++) {
        if (gs.enemy[i].life > 0) {
            gs.enemy[i].action();
        }
    }

}

