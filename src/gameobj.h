#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <SDL_opengl.h>
#include <vector>
#include "math3d.h"

class GameObj {
public:
    virtual ~GameObj() {}
    virtual void action() = 0;
    virtual void draw() = 0;
    void die();
    virtual void getPos(float *dst) const = 0;
    bool destroyable;
};

class Box : public GameObj {
public:
    Box(float x, float y, float z);
    void action();
    void draw();
    void getPos(float *dst) const;
protected:
    int life;
    float size[3];
    float pos[3];
    float rot[3];
};

class Tree : public GameObj {
public:
    Tree(float x, float y);
    void action() {}
    void draw();
    void getPos(float *dst) const;
protected:
    float pos[3];
    float size[2];
};

class Ball : public GameObj {
public:
    Ball();
    void action();
    void die();
    void draw(); 
    void setPos(float x, float y, float z);
    void getPos(float *dst) const;
    void setAge(int age_) { age = age_; }
    void setRadius(float r) { radius = r; }
    void setVel(float x, float y, float z);
    int combo;
protected:
    float radius;
    float pos[3];
    float rot;
    float vel[3]; 
    int age;
};

class Effect : public GameObj {
public:
    Effect(int dataID_, float x, float y, float z);
    void action();
    void draw();
    void getPos(float *dst) const;
protected:
    int dataID;
    int frame;
    float size[2];
    float pos[3];
    float rot[3];
};

class Particle : public GameObj {
public:
    Particle();
    void action();
    void draw();
    void getPos(float *dst) const;
protected:
    int life;
    float pos[3];
    friend class ParticleFountain;
};

class ParticleFountain : public GameObj {
public:
    ParticleFountain(float x, float y, float z, int n);
    ~ParticleFountain() {}
    void action();
    void draw();
    void getPos(float *dst) const;
    void addParticle();
    int num;
    float pos[3];
protected:
    std::vector<Particle> particle;
};


bool isCollide(float *a, float *b, float ra, float rb);
void putTree(float x, float y);
void putParticle(float x, float y, float z);
void putParticleFountain(float x, float y, float z, int num);
void putBox(float x, float y);
void putBox(float x, float y, float z);
void putEffect(GLuint texID, float x, float y, float z);

#endif
