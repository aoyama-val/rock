#include "common.h"
#include "camera.h" 
#include "gameobj.h"
#include <time.h>

//-------------------------------------------------------------------
//      Sys
//-------------------------------------------------------------------

Sys::Sys()
{
    debug = false;
    FPS = 30;
    MSPF = 1000/FPS;
    winWidth = 640;
    winHeight = 480;

    screen = NULL;
    joy = NULL;
}

Sys::~Sys()
{ 
} 

void Sys::init()
{
    randomSeed = (unsigned)time(NULL);
    srand(randomSeed);
    initSDL();
    if (config.useJoystick) {
        if (initJoystick()) {
            eprintf("failed to init joystick\n");
            config.useJoystick = false;
        }
    }

}

void Sys::quit()
{
    if (SDL_WasInit(SDL_INIT_JOYSTICK))
        SDL_JoystickClose(sys.joy);
    SDL_Quit();
    exit(0);
}

void Sys::initSDL()
{
    int videoflags;
    const SDL_VideoInfo *videoInfo;

    SDL_Init(SDL_INIT_VIDEO);
    videoInfo = SDL_GetVideoInfo();

    videoflags = SDL_OPENGL | SDL_HWPALETTE;
    if (videoInfo->hw_available)
        videoflags |= SDL_HWSURFACE;
    else
        videoflags |= SDL_SWSURFACE;
    if (videoInfo->blit_hw)
        videoflags |= SDL_HWACCEL;

    if (config.fullscreen) {
        videoflags |= SDL_FULLSCREEN;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    sys.screen = SDL_SetVideoMode(sys.winWidth, sys.winHeight, 24, videoflags);

    SDL_WM_SetCaption("THE ROCK 'N' ROLL GAME", "THE ROCK 'N' ROLL GAME");
} 

int Sys::initJoystick()
{
    if (joy == NULL) {
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
        joy = SDL_JoystickOpen(0);
        if (joy != NULL)
            return 0;
        else
            return -1;
    } 
    else {
        return 0;
    }
}

void Sys::handleEvent()
{
    SDL_Event ev;
    
    while(SDL_PollEvent(&ev)) {
        switch(ev.type) {
        case SDL_KEYDOWN:
            switch(ev.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit();
                break; 
            case SDLK_n:
                gs.pause = !gs.pause;
                break;
            case SDLK_v:
                sys.screenshot();
                break;
            case SDLK_F1:
                sys.toggleFullscreen();
                break;
            default:
                break;
            }
            break;
        case SDL_ACTIVEEVENT:
            if (ev.active.state == SDL_APPINPUTFOCUS) {
                if (!ev.active.gain) { // 入力フォーカスを失ったら PAUSE にする
                    gs.pause = true;
                }
            }
            break; 
        case SDL_QUIT:
            quit();
            break;
        default:
            break;
        }
    }
}

int Sys::getInput()
{
    int key = 0;

    Sint16 axis;
    Uint8 *keystate; 

    if (config.useJoystick) {
        key = 0;

        axis = SDL_JoystickGetAxis(joy, 0);
        if (axis <= config.joyLeft)
            key |= KEY_LEFT;
        else if (axis == config.joyRight)
            key |= KEY_RIGHT;

        axis = SDL_JoystickGetAxis(joy, 1);
        if (axis <= config.joyUp)
            key |= KEY_UP;
        else if (axis == config.joyDown)
            key = KEY_DOWN; 

        // ジョイスティックのボタン番号と KEY_BUTTON は一致させていないので
        // 0 1 2 3 がずれてる
        if (SDL_JoystickGetButton(joy, 0) == SDL_PRESSED) key |= KEY_BUTTON2;
        if (SDL_JoystickGetButton(joy, 1) == SDL_PRESSED) key |= KEY_BUTTON3;
        if (SDL_JoystickGetButton(joy, 2) == SDL_PRESSED) key |= KEY_BUTTON1;
        if (SDL_JoystickGetButton(joy, 3) == SDL_PRESSED) key |= KEY_BUTTON0;
        if (SDL_JoystickGetButton(joy, 4) == SDL_PRESSED) key |= KEY_BUTTON4;
        if (SDL_JoystickGetButton(joy, 5) == SDL_PRESSED) key |= KEY_BUTTON5;
        if (SDL_JoystickGetButton(joy, 6) == SDL_PRESSED) key |= KEY_BUTTON6;
        if (SDL_JoystickGetButton(joy, 7) == SDL_PRESSED) key |= KEY_BUTTON7;
        if (SDL_JoystickGetButton(joy, 8) == SDL_PRESSED) key |= KEY_BUTTON8;
        if (SDL_JoystickGetButton(joy, 9) == SDL_PRESSED) key |= KEY_BUTTON9;
    } 

    keystate = SDL_GetKeyState(NULL);
    if (keystate[config.assignedKeyToUP]) key |= KEY_UP;
    if (keystate[config.assignedKeyToLEFT]) key |= KEY_LEFT;
    if (keystate[config.assignedKeyToDOWN]) key |= KEY_DOWN;
    if (keystate[config.assignedKeyToRIGHT]) key |= KEY_RIGHT;
    if (keystate[config.assignedKeyToBUTTON[0]]) key |= KEY_BUTTON0;
    if (keystate[config.assignedKeyToBUTTON[1]]) key |= KEY_BUTTON1;
    if (keystate[config.assignedKeyToBUTTON[2]]) key |= KEY_BUTTON2;
    if (keystate[config.assignedKeyToBUTTON[3]]) key |= KEY_BUTTON3;
    if (keystate[config.assignedKeyToBUTTON[4]]) key |= KEY_BUTTON4;
    if (keystate[config.assignedKeyToBUTTON[5]]) key |= KEY_BUTTON5;
    if (keystate[config.assignedKeyToBUTTON[6]]) key |= KEY_BUTTON6;
    if (keystate[config.assignedKeyToBUTTON[7]]) key |= KEY_BUTTON7;
    if (keystate[config.assignedKeyToBUTTON[8]]) key |= KEY_BUTTON8;
    if (keystate[config.assignedKeyToBUTTON[9]]) key |= KEY_BUTTON9;

    if (((key & KEY_UP) && (key & KEY_DOWN)) || ((key & KEY_LEFT) && (key & KEY_RIGHT))) {
        key &= ~AXES;
    } 

    return key;
}

// ファイル名を指定してスクリーンショットをとる
int screenshotAs(SDL_Surface *screen, char *filename)
{
    SDL_Surface *temp;
    unsigned char *pixels;
    int i;
    
    if (!(screen->flags & SDL_OPENGL))
    {
        SDL_SaveBMP(screen, filename);
        return 0;
    }
        
    temp = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 24,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    0x000000FF, 0x0000FF00, 0x00FF0000, 0
#else
    0x00FF0000, 0x0000FF00, 0x000000FF, 0
#endif
    );
    if (temp == NULL)
        return -1;

    pixels = (unsigned char*)malloc(3 * screen->w * screen->h);
    if (pixels == NULL)
    {
        SDL_FreeSurface(temp);
        return -1;
    }

    glReadPixels(0, 0, screen->w, screen->h, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    for (i=0; i<screen->h; i++)
        memcpy(((char *) temp->pixels) + temp->pitch * i, pixels + 3*screen->w * (screen->h-i-1), screen->w*3);
    free(pixels);

    SDL_SaveBMP(temp, filename);
    SDL_FreeSurface(temp);

    return 0;
}

// 連番ファイル名でスクリーンショットをとる
void Sys::screenshot()
{
    int i = 0;
    char buf[256]; 
    FILE *fp;

    while (1) {
        sprintf(buf, "s_shot/s_shot%03d.bmp", i); 
        fp = fopen(buf, "r");
        if (fp != NULL) {
            fclose(fp);
            i++;
            continue;
        } 
        screenshotAs(screen, buf);
        break;
    }
}

void Sys::toggleFullscreen()
{
    if (SDL_WM_ToggleFullScreen(screen))
        config.fullscreen = !config.fullscreen;
}


//-------------------------------------------------------------------
//      GameState
//-------------------------------------------------------------------

GameState::GameState()
{ 
    gravity = -0.145;

    cntEnemy = 0;

    score = 0;

    frame = 0;
    pause = false;
    input_ = 0;
}

GameState::~GameState()
{
    delete player;
    delete camera;
    delete ball;
}

void GameState::init()
{
    map.load("data/heightmap.dat");
    player = new Player();
    ball = new Ball();
    camera = new Camera();
}

void GameState::updateInput()
{
    input_ = sys.getInput();
}

int GameState::input()
{
    return input_;
}

GameState::SCENE GameState::getScene()
{
    return scene_;
}

void GameState::setScene(SCENE s)
{
    scene_ = s;
}
