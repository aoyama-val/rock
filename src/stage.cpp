//-------------------------------------------------------------------
//      ステージ
//      
//-------------------------------------------------------------------

#include "common.h"
#include "stage.h"
#include "gameobj.h"

// ステージデータを作成
// init() から呼ばれる
void loadStage()
{
    float fogColor[4] = {1,1,1,1};
    float density = 1.5;
    float start = 30;
    float end = 150;

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, density);
    glFogf(GL_FOG_START, start);
    glFogf(GL_FOG_END, end);
    gs.player->pos[0] = 34.8;
    gs.player->pos[1] = 25.1;
    gs.player->pos[2] = 12.7;
    gs.player->rot[0] = 0;
    gs.player->rot[1] = 0;
    gs.player->rot[2] = 129;
    float ex = randDouble() * 70;
    float ey = randDouble() * 70;
    putEnemy(ex, ey, gs.map.getHeight(ex, ey));
    for (int i=0; i<5; i++) {
        float x, y;
        x = randDouble()*70;
        y = randDouble()*70;
        putBox(x, y, gs.map.getHeight(x, y));
    }
    putTree(15, 57);
    putTree(10, 10);
    putTree(40, 3);
    putParticleFountain(4, 4, 5, 10);
}

void putBox(float x, float y)
{
    putBox(x, y, gs.map.getHeight(x, y));
}

void putBox(float x, float y, float z)
{
    GameObj* box = new Box(x, y, z);
    gs.objs.push_back(box);
}

void putTree(float x, float y)
{
    GameObj* tree = new Tree(x, y);
    gs.objs.push_back(tree);
}

void putParticleFountain(float x, float y, float z, int num)
{
    GameObj* pf = new ParticleFountain(x, y, z, num);
    gs.objs.push_back(pf); 
}

void putEffect(GLuint texID, float x, float y, float z)
{
    GameObj* ef = new Effect(texID, x, y, z);
    gs.objs.push_back(ef);
}

void putEnemy(float x, float y, float z)
{
    int i;

    for (i=0; i<MAX_ENEMY; i++) {
        if (gs.enemy[i].life <= 0) {
            gs.enemy[i].life = 100;
            gs.enemy[i].turn = 0;
            gs.enemy[i].pos[0] = x;
            gs.enemy[i].pos[1] = y;
            gs.enemy[i].pos[2] = z;
            gs.cntEnemy += 1;
            break;
        }
    }
}

