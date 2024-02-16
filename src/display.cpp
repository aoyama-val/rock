#include "common.h"
#include "display.h"
#include <GL/glut.h>
#include "math3d.h"
#include "camera.h"
#include "gameobj.h"

// �R���{�����Ɖ��t���[���\�����邩�̃_�E���J�E���^
int drawComboCount;

//-------------------------------------------------------------------
//      �`��R�[�h
//-------------------------------------------------------------------

// �`�惁�C��
void draw()
{ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    glEnable(GL_DEPTH_TEST); 

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 150.0);

    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

    // ���W�n�� x:�E y:�� z:�� �ɂ��� 
    glRotatef(-90, 1, 0, 0); 

    // ���ׂĂ��v���C���[�̎����̊p�x�Ɣ��Ό����ɉ�]
    gs.camera->glRotate();

    glCallList(skyBoxList);

    // �`�悷�邷�ׂĂ̕��̂��ړ������鎖�ɂ���āA
    // �v���C���[����������Ă���悤�Ɍ�����
    gs.camera->glTranslate();

    gs.map.draw();
    gs.player->draw(); 
    drawEnemies(); 
    gs.ball->draw();

    for (std::list<GameObj*>::iterator it = gs.objs.begin();
            it != gs.objs.end(); ++it) {
        (*it)->draw();
    }

    // �ȉ� 2D �̕`�� ------------------------------------------
    set2DMode();

    drawScore();
    drawPosRot();
    drawFPS();

    if (gs.pause) {
        glColor4f(0,0,0,0.5);
        glRectf(0, 0, 640, 480);
        glColor4f(1,1,1,1);
        font.setSize(3.7, 3.7);
        font.print(195, 215,  0, "PAUSE");
    }
} 

void drawEnemies()
{
    int i;

    for (i=0; i<MAX_ENEMY; i++) {
        if (gs.enemy[i].life > 0) {
            gs.enemy[i].draw();
        }
    }
}

void drawScore()
{
    static int drawingScore = 0;
    char buf[256];

    glColor3f(1,1,1);

    if (drawingScore < gs.score)
        drawingScore += 299;
    if (drawingScore >= gs.score)
        drawingScore = gs.score;

    sprintf(buf, "Score: %8d", drawingScore);
    printStringRight(buf, GLUT_BITMAP_HELVETICA_18, 630, 460);

    if (gs.ball->combo > 1 && drawComboCount > 0) {
        printString("Combo:", GLUT_BITMAP_HELVETICA_18, 510, 440);
        sprintf(buf, "%d", gs.ball->combo);
        printStringRight(buf, GLUT_BITMAP_HELVETICA_18, 630, 440);
        drawComboCount -= 1;
    } 
}

void drawPosRot()
{ 
    char buf[256];
    const int PARAM_POSX = 570;

    glColor3f(1,1,1);

    sprintf(buf, "x=%2.1f", gs.player->pos[0]);
    printString(buf, GLUT_BITMAP_HELVETICA_18, PARAM_POSX, 80);
    sprintf(buf, "y=%2.1f", gs.player->pos[1]);
    printString(buf, GLUT_BITMAP_HELVETICA_18, PARAM_POSX, 65);
    sprintf(buf, "z=%2.1f", gs.player->pos[2]);
    printString(buf, GLUT_BITMAP_HELVETICA_18, PARAM_POSX, 50);
    sprintf(buf, "rx=%2.1f", gs.player->rot[0]);
    printString(buf, GLUT_BITMAP_HELVETICA_18, PARAM_POSX, 35);
    sprintf(buf, "ry=%2.1f", gs.player->rot[1]);
    printString(buf, GLUT_BITMAP_HELVETICA_18, PARAM_POSX, 20);
    sprintf(buf, "rz=%2.1f", gs.player->rot[2]);
    printString(buf, GLUT_BITMAP_HELVETICA_18, PARAM_POSX, 5);
} 

void drawFPS()
{ 
    static Uint32 lastTick;
    Uint32 tick = SDL_GetTicks();

    float fps = 1000.0/(tick-lastTick);
    char buf[256];
    sprintf(buf, "fps: %3.1f", fps);
    printString(buf, GLUT_BITMAP_HELVETICA_12, 3, 3);

    lastTick = tick; 
}

//-------------------------------------------------------------------
//      �⏕�֐�
//-------------------------------------------------------------------

void set2DMode()
{ 
    glDisable(GL_TEXTURE_2D); 

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, 640, 0, 480, -1, 1); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
}

// ��ɑO�ʂ������郂�[�h
void beginBillboard2(float *pos)
{
    float diff[3];
    float deg;
    float cam[3];

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    gs.camera->getPos(cam);
    diff[0] = pos[0] - cam[0];
    diff[1] = pos[1] - cam[1];
    diff[2] = pos[2] - cam[2];
    deg = RAD2DEG(atan2(diff[1], diff[0])) - 90;
    glRotatef(deg, 0.0, 0.0, 1.0);
    deg = RAD2DEG(atan2(diff[2], sqrt(diff[0]*diff[0]+diff[1]*diff[1])));
    glRotatef(deg, 1.0, 0.0, 0.0);
}

// �r���{�[�h�������[�h���畜�A
void endBillboard2()
{
    glPopMatrix();
}

// texID �̃e�N�X�`����\������`��
void putTexture(GLuint texID, float x, float y, float u1, float v1, float u2, float v2)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);
    glBegin(GL_QUADS);
    glTexCoord2f(u1,v1); glVertex3f(-x, 0, 0);
    glTexCoord2f(u2,v1); glVertex3f(+x, 0, 0);
    glTexCoord2f(u2,v2); glVertex3f(+x, 0, y);
    glTexCoord2f(u1,v2); glVertex3f(-x, 0, y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//-------------------------------------------------------------------
//      glut ���g����������\��
//-------------------------------------------------------------------

// ������\��
void printString(const char *string, void* fnt, int left, int bottom)
{
    glRasterPos2i(left, bottom);
    while(*string) {
        glutBitmapCharacter(fnt, *string);
        string++;
    }
} 

// right-aligned ������\��
void printStringRight(const char *string, void *fnt, int right, int bottom)
{
    glRasterPos2i(right-getStringWidth(fnt, string), bottom);
    while(*string) {
        glutBitmapCharacter(fnt, *string);
        string++;
    }
} 

// string�̉�ʏ�̕��̃h�b�g����Ԃ�
int getStringWidth(void* fnt, const char *string)
{
    int len = 0;

    while(*string) {
        len += glutBitmapWidth(fnt, *string);
        string++;
    }

    return len;
}

