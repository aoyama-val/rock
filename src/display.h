#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "texfont.h"

GLOBAL TexFont font;

// テクスチャ
GLOBAL GLuint topID, bottomID, frontID, backID, leftID, rightID;
GLOBAL GLuint treeID;
GLOBAL GLuint particleID;
GLOBAL GLuint ballID;
GLOBAL GLuint boxID;
GLOBAL GLuint explodeID; 
GLOBAL GLuint enemyID;

// ディスプレイリスト
GLOBAL GLuint ballList;
GLOBAL GLuint boxList;
GLOBAL GLuint skyBoxList;

extern int drawComboCount;

void draw();
void drawEnemies();
void drawScore();
void drawPosRot();
void drawFPS();
void set2DMode();
void beginBillboard2(float *pos);
void endBillboard2();
void putTexture(GLuint texID, float x, float y, float u1, float v1, float u2, float v2);
void printString(const char *string, void* fnt, int left, int bottom);
void printStringRight(const char *string, void *fnt, int right, int bottom);
int getStringWidth(void* fnt, const char *string);

#endif
