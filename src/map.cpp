#include <stdio.h>
#include <SDL_opengl.h>
#include "map.h"

int Map::load(const char* filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "cannot open %s\n", filename);
        return -1;
    }
    fscanf(fp, "SCALE %f\n", &scale);
    fscanf(fp, "NUMX %d\n", &numx);
    fscanf(fp, "NUMY %d\n", &numy);

    for (int j=0; j<numx+1; j++) {
        for (int i=0; i<numy+1; i++) {
            verteces[i][j][0] = i * scale;
            verteces[i][j][1] = j * scale;
            fscanf(fp, "%f", &verteces[i][j][2]);
        }
    } 

    fclose(fp);

    return 0;
}

float Map::getHeight(float x, float y)
{
    float dx, dy;
    float dz1, dz2;
    float h;

    int ti = -1, tj = -1;

    for (int i=0; i<numx; i++) {
        if (verteces[i][0][0] < x && x < verteces[i+1][0][0]) {
            ti = i;
            break;
        }
    }
    for (int j=0; j<numy; j++) {
        if (verteces[0][j][1] < y && y < verteces[0][j+1][1]) {
            tj = j;
            break;
        }
    }

    if (ti == -1 || tj == -1) return 0.0;

    dx = x-verteces[ti][tj][0];
    dy = y-verteces[ti][tj][1];

    if (dx > dy) {
        dz1 = verteces[ti][tj][2] - verteces[ti+1][tj][2];
        dz2 = verteces[ti+1][tj+1][2] - verteces[ti+1][tj][2];
        dx = verteces[ti+1][tj][0] - x;
        h = verteces[ti+1][tj][2] + dz1*dx/scale + dz2*dy/scale;
    }
    else {
        dz1 = verteces[ti+1][tj+1][2] - verteces[ti][tj+1][2];
        dz2 = verteces[ti][tj][2] - verteces[ti][tj+1][2];
        dy = verteces[ti][tj+1][1] - y;
        h = verteces[ti][tj+1][2] + dz1*dx/scale + dz2*dy/scale;
    }
    
    return h;
}

void Map::getGravity(float x, float y, float *gra)
{
    float dx, dy;
    float dz1, dz2;

    int ti = -1, tj = -1;

    for (int i=0; i<numx; i++) {
        if (verteces[i][0][0] < x && x < verteces[i+1][0][0]) {
            ti = i;
            break;
        }
    }
    for (int j=0; j<numy; j++) {
        if (verteces[0][j][1] < y && y < verteces[0][j+1][1]) {
            tj = j;
            break;
        }
    }

    if (ti == -1 || tj == -1) {
        gra[0] = 0;
        gra[1] = 0;
        gra[2] = 0;
        return;
    }

    dx = x-verteces[ti][tj][0];
    dy = y-verteces[ti][tj][1];

    if (dx > dy) {
        dz1 = verteces[ti][tj][2] - verteces[ti+1][tj][2];
        dz2 = verteces[ti+1][tj+1][2] - verteces[ti+1][tj][2];
        gra[0] = dz1;
        gra[1] = -dz2;
        gra[2] = gra[0] * dz2 + gra[1] * dz1;
        return;
    }
    else {
        dz1 = verteces[ti+1][tj+1][2] - verteces[ti][tj+1][2];
        dz2 = verteces[ti][tj][2] - verteces[ti][tj+1][2];
        gra[0] = -dz1;
        gra[1] = dz2;
        gra[2] = gra[0] * dz2 + gra[1] * dz1;
        return;
    }
}

void Map::draw()
{ 
    glCallList(dlistID);
}

void Map::createDisplayList()
{
    dlistID = glGenLists(1);
    glNewList(dlistID, GL_COMPILE);
    glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, texID); 
    glBegin(GL_TRIANGLES);
    for (int i=0; i<numx; i++) {
        for (int j=0; j<numy; j++) {
            glTexCoord2f(i/(float)numx, j/(float)numy); glVertex3fv(verteces[i][j]);
            glTexCoord2f((i+1)/(float)numx, j/(float)numy); glVertex3fv(verteces[i+1][j]);
            glTexCoord2f((i+1)/(float)numx, (j+1)/(float)numy); glVertex3fv(verteces[i+1][j+1]);

            glTexCoord2f((i  )/(float)numx, (j  )/(float)numy); glVertex3fv(verteces[i][j]);
            glTexCoord2f((i+1)/(float)numx, (j+1)/(float)numy); glVertex3fv(verteces[i+1][j+1]);
            glTexCoord2f((i  )/(float)numx, (j+1)/(float)numy); glVertex3fv(verteces[i][j+1]);
        }
    }
    glEnd();
    glEndList();
}

void Map::deleteDisplayList()
{
    glDeleteLists(dlistID, 1); 
}

bool Map::isInside(float x, float y)
{
    float u[2];
    float v[2];

    getBoundary(u, v);

    if (u[0] < x && x < v[0]
     && u[1] < y && y < v[1])
        return true;
    else
        return false;
}

void Map::getBoundary(float *min, float *max)
{
    min[0] = 0;
    min[1] = 0;
    max[0] = numx * scale;
    max[1] = numy * scale;
}

