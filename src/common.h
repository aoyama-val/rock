#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <list>

#include "util.h"
#include "player.h"
#include "enemy.h"
#include "conf.h"
#include "map.h"


#ifdef GLOBAL_DEFINE
#define GLOBAL
#else
#define GLOBAL extern
#endif

//-------------------------------------------------------------------
//      デバッグ用マクロ
//-------------------------------------------------------------------

#define here()          dprintf("%s: %d passed\n", __FILE__, __LINE__)
#define printD(expr)    dprintf("%s: %d: " #expr "=%d\n", __FILE__, __LINE__, expr)
#define printF(expr)    dprintf("%s: %d: " #expr "=%f\n", __FILE__, __LINE__, expr)

//-------------------------------------------------------------------
//      入力状態
//-------------------------------------------------------------------

#define KEY_UP          0x01
#define KEY_LEFT        0x02
#define KEY_DOWN        0x04
#define KEY_RIGHT       0x08
#define KEY_BUTTON0     0x10
#define KEY_BUTTON1     0x20
#define KEY_BUTTON2     0x40
#define KEY_BUTTON3     0x80
#define KEY_BUTTON4     0x100
#define KEY_BUTTON5     0x200
#define KEY_BUTTON6     0x400
#define KEY_BUTTON7     0x800
#define KEY_BUTTON8     0x1000
#define KEY_BUTTON9     0x2000

#define BASIC_BUTTONS   (KEY_BUTTON0 | KEY_BUTTON1 | KEY_BUTTON2 | KEY_BUTTON3)
#define LR_BUTTONS      (KEY_BUTTON4 | KEY_BUTTON5 | KEY_BUTTON6 | KEY_BUTTON7)
#define ANY_BUTTONS     (BASIC_BUTTONS | LR_BUTTONS | KEY_BUTTON8 | KEY_BUTTON9)
#define AXES            (KEY_UP | KEY_LEFT | KEY_DOWN | KEY_RIGHT)
#define ANY_INPUTS      (ANY_BUTTONS | AXES)

//-------------------------------------------------------------------
//      グローバルオブジェクトのクラス
//-------------------------------------------------------------------

class Sys {
public:
    Sys();
    ~Sys();
    void init();
    void quit();    // 正常終了はこのメソッドを通して行う
    int getInput();
    void screenshot();
    void toggleFullscreen();
    void handleEvent();

    bool debug;
    int FPS;
    int MSPF;
    int winWidth, winHeight;
protected:
    void initSDL();
    int initJoystick();

    unsigned int randomSeed;
    SDL_Surface *screen;
    SDL_Joystick *joy;
};

class Camera;
class GameObj;
class Ball;

class GameState {
public:
    typedef enum {
        OPENING,
        GAME,
        SYS_QUIT 
    } SCENE;

    GameState();
    ~GameState();

    void init();
    void updateInput();
    int input();

    void setScene(SCENE s);
    SCENE getScene();

    Map map;

    Player* player;
    Camera *camera;

    float gravity; 

#define MAX_ENEMY 10
    Enemy enemy[MAX_ENEMY];
    int cntEnemy;

    Ball* ball;

    std::list<GameObj*> objs;
    std::list<GameObj*> deadObjs;
    
    int score;

    int frame;
    bool pause;

protected: 
    SCENE scene_;
    int input_; 
};

//-------------------------------------------------------------------
//      グローバルオブジェクト
//-------------------------------------------------------------------

GLOBAL Sys sys;
GLOBAL Config config; 
GLOBAL GameState gs;


#endif
