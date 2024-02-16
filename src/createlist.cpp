//-------------------------------------------------------------------
//      createlist
//      OpenGL ディスプレイリストを作成
//-------------------------------------------------------------------

#include "common.h"
#include "display.h"
#include "createlist.h"

GLuint createBallList()
{
    GLUquadricObj *qobj;
    GLuint index;

    index = glGenLists(1);
    qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GL_SMOOTH);
    gluQuadricTexture(qobj, GL_TRUE);
    glNewList(index, GL_COMPILE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ballID);
        gluSphere(qobj, 1.0, 6, 6);
        glDisable(GL_TEXTURE_2D);
    glEndList();
    gluDeleteQuadric(qobj);
    return index;
}

static const int boxEdge[][4] = {
    {0,1,2,3},
    {3,2,6,7},
    {5,4,7,6},
    {4,5,1,0},
    {1,5,6,2},
    {0,3,7,4}
};

static const float boxVertex[][3] = {
    {-0.5, -0.5, 0.0},
    { 0.5, -0.5, 0.0},
    { 0.5, -0.5, 1.0},
    {-0.5, -0.5, 1.0},
    {-0.5,  0.5, 0.0},
    { 0.5,  0.5, 0.0},
    { 0.5,  0.5, 1.0},
    {-0.5,  0.5, 1.0}
};

GLuint createBoxList(float t)
{
    GLuint index;
    int i;

    index = glGenLists(1);
    glNewList(index, GL_COMPILE);
        glBegin(GL_QUADS);
        for (i=0; i<6; i++) {
            glTexCoord2f(0,0); glVertex3fv(boxVertex[boxEdge[i][0]]);
            glTexCoord2f(t,0); glVertex3fv(boxVertex[boxEdge[i][1]]);
            glTexCoord2f(t,t); glVertex3fv(boxVertex[boxEdge[i][2]]);
            glTexCoord2f(0,t); glVertex3fv(boxVertex[boxEdge[i][3]]);
        }
        glEnd();
    glEndList();

    return index;
} 

GLuint createSkyBoxList()
{
    GLuint index;

    float topZ = 50;
    float bottomZ = 0;
    float frontY = 50;
    float backY = -50;
    float rightX = 50;
    float leftX = -50;

    index = glGenLists(1);
    glNewList(index, GL_COMPILE);
        glPushMatrix();
        glTranslatef(0, 0, -20);

        glColor4f(1,1,1,1); 
        glDisable(GL_DEPTH_TEST); 

        //top
        glBindTexture(GL_TEXTURE_2D, topID);
        glBegin(GL_QUADS);
            glTexCoord2f(0,1); glVertex3f(leftX, backY, topZ);
            glTexCoord2f(0,0); glVertex3f(leftX, frontY, topZ);
            glTexCoord2f(1,0); glVertex3f(rightX, frontY, topZ);
            glTexCoord2f(1,1); glVertex3f(rightX, backY, topZ); 
        glEnd();

        // front
        glBindTexture(GL_TEXTURE_2D, frontID);
        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(leftX, frontY, bottomZ);
            glTexCoord2f(1,0); glVertex3f(rightX, frontY, bottomZ);
            glTexCoord2f(1,1); glVertex3f(rightX, frontY, topZ);
            glTexCoord2f(0,1); glVertex3f(leftX, frontY, topZ); 
        glEnd();

        // right
        glBindTexture(GL_TEXTURE_2D, leftID);
        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(rightX, frontY, bottomZ);
            glTexCoord2f(1,0); glVertex3f(rightX, backY, bottomZ);
            glTexCoord2f(1,1); glVertex3f(rightX, backY, topZ);
            glTexCoord2f(0,1); glVertex3f(rightX, frontY, topZ);
        glEnd();

        // left
        glBindTexture(GL_TEXTURE_2D, rightID);
        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(leftX, backY, bottomZ);
            glTexCoord2f(1,0); glVertex3f(leftX, frontY, bottomZ);
            glTexCoord2f(1,1); glVertex3f(leftX, frontY, topZ);
            glTexCoord2f(0,1); glVertex3f(leftX, backY, topZ);
        glEnd();

        // back
        glBindTexture(GL_TEXTURE_2D, backID);
        glBegin(GL_QUADS);
            glTexCoord2f(1,0); glVertex3f(leftX ,backY, bottomZ);
            glTexCoord2f(1,1); glVertex3f(leftX ,backY, topZ);
            glTexCoord2f(0,1); glVertex3f(rightX ,backY, topZ);
            glTexCoord2f(0,0); glVertex3f(rightX ,backY, bottomZ);

        glEnd();

        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
    glEndList();

    return index;
}
