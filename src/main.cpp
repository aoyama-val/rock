//-------------------------------------------------------------------
//      main
//      main 関数・ゲームメインループ
//-------------------------------------------------------------------

// 座標系は x:右 y:奥 z:上 の右手系

#define GLOBAL_DEFINE

#include "common.h"
#include "main.h"
#include "display.h"
#include "timer.h"
#include "createlist.h"
#include "gameobj.h"
#include "util.h"
#include "conf.h"
#include "player.h"
#include "texture.h"
#include "camera.h"
#include "stage.h"

// glut 3.7 で起きる浮動小数点演算エラーを回避するため 
// http://www.opengl.org/documentation/specs/glut/glut_faq.html#Q39
#ifdef __BORLANDC__
#include <float.h> 
#endif

int main(int argc, char* argv[])
{
// glut 3.7 で起きる浮動小数点演算エラーを回避するため
#ifdef __BORLANDC__
    _control87(MCW_EM,MCW_EM);
#endif

    parseArgs(argc, argv);

    init();

    gameStart();

    return 0;
} 

// ゲームメインループ
void gameStart()
{ 
    loadModels();
    loadTextures(); 
    createDisplayLists();
    loadStage();

    opening(); 
    gs.setScene(GameState::GAME);

    FPSTimer timer;
    timer.init();

    while (gs.getScene() == GameState::GAME) {
        sys.handleEvent();
        if (!gs.pause) {
            gs.updateInput();
            action();
            gs.camera->move(gs.player->pos, gs.player->rot);
        }
        draw();
        SDL_GL_SwapBuffers();
        OpenGLErrorCheck();
        timer.wait(sys.MSPF);
        clearDeadObjs();
    }

    deleteTextures();
    deleteDisplayLists();
}

// ゲームを 1 フレーム進める
void action()
{ 
    gs.player->action();
    enemiesAction();
    gs.ball->action();

    for (std::list<GameObj*>::iterator it = gs.objs.begin();
            it != gs.objs.end(); ++it) {
        (*it)->action();
    }

    gs.frame += 1;
}

// 初期化
void init()
{ 
    config.reset();
    config.resetKeyAssign();
    config.load("rock.cfg");

    sys.init();

    initGL();

    if (font.load("texture/font.tga")) {
        eprintf("failed to load texture/font.tga\n");
    }
    gs.init(); 
}

void initGL()
{
    glViewport(0, 0, sys.winWidth, sys.winHeight);
    glClearColor(0.0,0.0,0.0,0.0);	

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);

    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

    float lightAmbient[] = {1.0,1.0,1.0,1.0};
    float lightDiffuse[] = {1.0,1.0,1.0,1.0};
    float lightPosition[] = {70.0, 0.0, 50.0, 1.0};
    float lightSpecular[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular); 
}

void opening()
{
    float a;
    int frame = 0;

    gs.setScene(GameState::OPENING);

    gs.camera->move(gs.player->pos, gs.player->rot);

#ifdef WIN32
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapBuffers();
#endif
    glEnable(GL_SCISSOR_TEST);

    while(frame < 15) {
        a = frame / 10.0;
        if (frame < 12) {
            int x = (int)((600-400*a)*sys.winWidth/640.0); 
            int y = (int)((-400*a+400)*sys.winHeight/480.0); 
            int w = (int)((200+40*a)*sys.winWidth/640.0); 
            int h = (int)((200+40*a)*sys.winHeight/480.0);
            clamp<int>(&x, 0, sys.winWidth - 1);
            clamp<int>(&y, 0, sys.winHeight - 1);
            clamp<int>(&w, 0, sys.winWidth - x - 1);
            clamp<int>(&h, 0, sys.winHeight - y - 1);
            glScissor(x, y, w, h);
        }

        draw();
        if (frame >= 12) {
            glDisable(GL_SCISSOR_TEST);
            font.setSize(3.7, 3.7);
            font.print(195, 215, 0, "START");
        }
        glColor4f(0.0,0.0,0.0,1.0-a);
        glRectf(0, 0, 640, 480);
        SDL_GL_SwapBuffers();
        SDL_Delay(100);
        frame += 1;
    }
    SDL_Delay(100);
    glDisable(GL_SCISSOR_TEST);
}

void loadTextures()
{
    int err = 0;
    unsigned int tmp;
#define TEXDIR "texture/"

    err += createTextureFromTGA(TEXDIR "plane.tga", &tmp);
    gs.map.setTexture(tmp);

    err += createTextureFromTGA(TEXDIR "Top.tga", &topID);
    err += createTextureFromTGA(TEXDIR "Bottom.tga", &bottomID);
    err += createTextureFromTGA(TEXDIR "Front.tga", &frontID);
    err += createTextureFromTGA(TEXDIR "Back.tga", &backID);
    err += createTextureFromTGA(TEXDIR "Left.tga", &leftID);
    err += createTextureFromTGA(TEXDIR "Right.tga", &rightID);

    err += createTextureFromTGA(TEXDIR "tree.tga", &treeID); 
    err += createTextureFromTGA(TEXDIR "particle.tga", &particleID);
    err += createTextureFromTGA(TEXDIR "ball.tga", &ballID);
    err += createTextureFromTGA(TEXDIR "box.tga", &boxID);
    err += createTextureFromTGA(TEXDIR "explode1.tga", &explodeID);

    if (err < 0) {
        eprintf("failed to load textures\n");
        exit(0);
    } 
#undef TEXDIR
}

void deleteTextures()
{
    glDeleteTextures(1, &topID);
    glDeleteTextures(1, &bottomID);
    glDeleteTextures(1, &frontID);
    glDeleteTextures(1, &backID);
    glDeleteTextures(1, &leftID);
    glDeleteTextures(1, &rightID);
    glDeleteTextures(1, &treeID);
    glDeleteTextures(1, &particleID);
    glDeleteTextures(1, &ballID);
    glDeleteTextures(1, &boxID);
    glDeleteTextures(1, &explodeID); 
}

void createDisplayLists()
{
    gs.map.createDisplayList();
    ballList = createBallList();
    boxList = createBoxList(1.0);
    skyBoxList = createSkyBoxList();
}

void deleteDisplayLists()
{
    gs.map.deleteDisplayList();
    glDeleteLists(ballList, 1);
    glDeleteLists(boxList, 1);
    glDeleteLists(skyBoxList, 1);
}

void loadModels()
{
    if (gs.player->loadModel("model/yoshi.md2", "model/yoshi.pcx") < 0) {
        eprintf("failed to load player model\n");
        exit(0);
    }

    if (gs.enemy[0].loadModel("model/samhain.md2", "model/samhain.pcx") < 0) {
        eprintf("failed to load enemy model\n");
        exit(0);
    }

}

void clearDeadObjs()
{
    for (std::list<GameObj*>::iterator it = gs.deadObjs.begin();
            it != gs.deadObjs.end(); ++it) {
        gs.objs.remove(*it);
        delete *it;
    } 
    gs.deadObjs.clear();
}

void parseArgs(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
}

