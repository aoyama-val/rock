#include "common.h"
#include "gameobj.h"
#include "main.h"
#include "math3d.h"
#include "display.h"
#include "camera.h"

void GameObj::die()
{
    gs.deadObjs.push_back(this);
}

bool isCollide(float *a, float *b, float ra, float rb)
{
    if (distance3fv(a, b) < ra + rb)
        return true;
    else
        return false;
}

//-------------------------------------------------------------------
//  Box
//-------------------------------------------------------------------

Box::Box(float x, float y, float z)
{
    destroyable = true;
    life = 1;
    size[0] = 1.5;
    size[1] = 1.5;
    size[2] = 1.5;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    rot[0] = 0;
    rot[1] = 0;
    rot[2] = 0;
}

void Box::getPos(float *dst) const
{
    dst[0] = pos[0];
    dst[1] = pos[1];
    dst[2] = pos[2];
}

void Box::action()
{
}

void Box::draw()
{
    glPushMatrix(); 
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(rot[0], 1, 0, 0);
    glRotatef(rot[1], 0, 1, 0);
    glRotatef(rot[2], 0, 0, 1);
    glScalef(size[0], size[1], size[2]);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, boxID);
    glColor3f(1,1,1);
    glCallList(boxList);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix(); 
}

//-------------------------------------------------------------------
//  Tree
//-------------------------------------------------------------------

Tree::Tree(float x, float y)
{
    destroyable = false;
    pos[0] = x;
    pos[1] = y;
    pos[2] = gs.map.getHeight(x,y);
    size[0] = 1.5;
    size[1] = 9.0;
}

void Tree::draw()
{ 
    float camRot[3];

    glEnable(GL_TEXTURE_2D);
    glColor4f(1,1,1,1);

    gs.camera->getRot(camRot);

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(camRot[2]-90, 0, 0, 1);
    putTexture(treeID, size[0], size[1], 0, 0, 1, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void Tree::getPos(float *dst) const
{
    dst[0] = pos[0];
    dst[1] = pos[1];
    dst[2] = pos[2];
}

//-------------------------------------------------------------------
//  Particle
//-------------------------------------------------------------------

Particle::Particle()
{ 
    destroyable = false;
}

void Particle::action()
{
}

void Particle::draw()
{ 
    glColor4f(0.5, 0.0, 1.0, life/50.0);

    beginBillboard2(pos);
    putTexture(particleID, 0.5, 1.0, 0, 0, 1, 1);
    endBillboard2();

    glDisable(GL_TEXTURE_2D);
}

void Particle::getPos(float *dst) const
{
    dst[0] = pos[0];
    dst[1] = pos[1];
    dst[2] = pos[2];
}


ParticleFountain::ParticleFountain(float x, float y, float z, int n)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    num = n;

    Particle p;
    particle.assign(n, p);

    for (int i=0; i<n; i++)
        particle[i].life = 0;
}

void ParticleFountain::draw()
{
    for (int i=0; i<num; i++) {
        if (particle[i].life > 0) {
            particle[i].draw();
        }
    }
}

void ParticleFountain::action()
{
    for (int j=0; j<num; j++) {
        particle[j].life -= 1;
        if (particle[j].life <= 0) {
            particle[j].life = (int)(100 * randDouble());
            particle[j].pos[0] = pos[0] + 3*randDouble();
            particle[j].pos[1] = pos[1] + 3*randDouble();
            particle[j].pos[2] = pos[2] + 3*randDouble();
            continue;
        }
        particle[j].pos[0] += 0.1 * sin(2*M_PI*particle[j].life/240.0);
        particle[j].pos[1] += 0.1 * cos(2*M_PI*particle[j].life/240.0);
        particle[j].pos[2] += 0.066;
    }
}

void ParticleFountain::getPos(float *dst) const
{
    dst[0] = pos[0];
    dst[1] = pos[1];
    dst[2] = pos[2];
}

void ParticleFountain::addParticle()
{
    int i;

    for (i=0; i<num; i++) {
        if (particle[i].life <= 0) {
            particle[i].life = (int)(100 * randDouble());
            particle[i].pos[0] = pos[0] + 3*randDouble();
            particle[i].pos[1] = pos[1] + 3*randDouble();
            particle[i].pos[2] = pos[2] + 3*randDouble();
            break;
        }
    }
}

//-------------------------------------------------------------------
//  Ball
//-------------------------------------------------------------------

Ball::Ball()
{
    radius = 0.6;
    rot = 0;
    vel[0] = 0;
    vel[1] = 0;
    vel[2] = 0;

    die();
}

void Ball::setPos(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void Ball::action()
{
    float gra[3];
    float h;

    h = gs.map.getHeight(pos[0], pos[1]);

    // íàÇ…ïÇÇ¢ÇƒÇ¢ÇÈä‘ÇÕóéâ∫
    if (pos[2] > h + radius + 0.2) {
        vel[2] += -0.18;
        pos[2] += vel[2];
        if (pos[2] < h)
            pos[2] = h + radius + 0.1;
    }
    else {  // ì]Ç™ÇÈ
        gs.map.getGravity(pos[0], pos[1], gra);
        normalize(gra);
        gra[0] *= 0.09;
        gra[1] *= 0.09;
        vel[0] = vel[0] * 0.9 + gra[0];
        vel[1] = vel[1] * 0.9 + gra[1];
        vel[2] = 0;
        pos[0] += vel[0];
        pos[1] += vel[1];
        pos[2] = gs.map.getHeight(pos[0], pos[1]) + radius + 0.1;
    }

    rot -= 20 * sqrt(vel[0]*vel[0]+vel[1]*vel[1]+vel[2]*vel[2]);

    std::list<GameObj*>::iterator it, begin,end;
    begin = gs.objs.begin();
    end = gs.objs.end();

    // î†Ç∆ÇÃè’ìÀîªíË
    for (it=begin; it != end; ++it) {
        if ((*it)->destroyable) {
            float tgt[3];
            (*it)->getPos(tgt);
            if (isCollide(tgt, pos, 1.5, radius)) {
                (*it)->die();
                putEffect(explodeID, tgt[0], tgt[1], tgt[2]-1.4);
                float x = randDouble() * 70;
                float y = randDouble() * 70;
                float z = gs.map.getHeight(x, y);
                putBox(x, y, z);
                combo += 1;
                gs.score += static_cast<int>(age*age+500)*combo;
                drawComboCount = 120;
                if (combo >= 2) {
                    putBox(randDouble()*70, randDouble()*70);
                }
            }
        }
    }

    // ìGÇ∆ÇÃè’ìÀîªíË
    for (int i=0; i<MAX_ENEMY; i++) {
        if (gs.enemy[i].isAlive()) {
            if (isCollide(gs.enemy[i].pos, pos, 1.0, 1.0)) {
                combo += 1;
                gs.score += age*age*4*combo;
                putEffect(explodeID, gs.enemy[i].pos[0], gs.enemy[i].pos[1], gs.enemy[i].pos[2]-0.35);
                gs.enemy[i].die();
                if (combo >= 2) {
                    putBox(randDouble()*70, randDouble()*70);
                }
            }
        }
    }

    age += 1;
    if (age >= 240)
        die();
}

void Ball::die()
{
    age = 0;
    pos[0] = -100;
    pos[1] = -100;
    pos[2] = -100;
}

void Ball::draw()
{
    if (!gs.map.isInside(pos[0], pos[1]))
        return;
    glPushMatrix(); 
    glTranslatef(pos[0], pos[1], pos[2]+0.4);

    // êiçsï˚å¸Ç∆êÇíºÇÃé≤Ç…âÒì]
    glRotatef(rot, vel[1], -vel[0], 0);

    // àÍíËéûä‘ÇΩÇ¬Ç∆ìßñæÇ…Ç»Ç¡ÇƒéÄÇ 
    if (age > 200) {
        glColor4f(1,1,1,1-(age-200)/40.0);
    }
    else {
        glColor4f(1,1,1,1);
    }

    glCallList(ballList);
    glPopMatrix(); 
}

void Ball::getPos(float *dst) const
{
    dst[0] = pos[0];
    dst[1] = pos[1];
    dst[2] = pos[2];
}

void Ball::setVel(float x, float y, float z)
{
    vel[0] = x;
    vel[1] = y;
    vel[2] = z;
}

//-------------------------------------------------------------------
//  Effect
//-------------------------------------------------------------------

Effect::Effect(int dataID_, float x, float y, float z) : dataID(dataID_)
{
    destroyable = false;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    size[0] = 5.0;
    size[1] = 5.0;
    frame = 0;
}

void Effect::action()
{
    frame += 1;
    if (frame >= 48) {
        frame = 0;
        die();
    }
}

void Effect::draw()
{
    float u1, v1, u2, v2;
    u1 = (frame%8)/8.0;
    u2 = (frame%8+1)/8.0;
    v1 = (7-frame/8)/8.0;
    v2 = (8-frame/8)/8.0;
    beginBillboard2(pos);
    glColor3f(1,1,1);
    putTexture(explodeID, size[0], size[1], u1, v1, u2, v2);
    endBillboard2();
}

void Effect::getPos(float *dst) const
{
    dst[0] = pos[0];
    dst[1] = pos[1];
    dst[2] = pos[2];
}

