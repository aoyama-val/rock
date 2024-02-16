//-------------------------------------------------------------------
//      texfont
//      テクスチャによるフォントクラス
//-------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "texfont.h"
#include "texture.h"

int TexFont::load(const char* filename)
{
    unsigned char* data;
    int w, h;
    float cx, cy;

    glGenTextures(1, &texID);         
    glBindTexture(GL_TEXTURE_2D, texID);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = loadTGA(filename, &w, &h);
    if (data == 0)
        return -1;

    glGenTextures(1, &texID);             
    glBindTexture(GL_TEXTURE_2D, texID); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);

    base = glGenLists(256);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    for (int i=0; i<256; i++) {
        cx = (float)(i%16)/16.0;
        cy = (float)(i/16)/16.0;

        glNewList(base+i, GL_COMPILE);
        glBegin(GL_QUADS);
            glTexCoord2f(cx          , 1.0 - cy - 0.0625); glVertex2f(0, 0);
            glTexCoord2f(cx + 0.0625f, 1.0 - cy - 0.0625); glVertex2f(16,0);                               
            glTexCoord2f(cx + 0.0625f, 1.0 - cy - 0.001f); glVertex2f(16, 16);                                  
            glTexCoord2f(cx          , 1.0 - cy - 0.001f); glVertex2f(0, 16);                         
        glEnd();
        glTranslatef(12, 0, 0);
        glEndList();
    }

    return 0;
}

void TexFont::deleteFont()
{
    glDeleteLists(base, 256);
    glDeleteTextures(1, &texID);
}

void TexFont::print(int x, int y, char type, const char *fmt, ...)
{
    char text[1024];
    va_list ap;

    va_start(ap, fmt);                               
    vsprintf(text, fmt, ap);                        
    va_end(ap);                                    

    GLboolean isTex2D = glIsEnabled(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glListBase(base-32+(128*type));
    glScalef(sizex, sizey, 1.0);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopMatrix(); 

    if (!isTex2D)
        glDisable(GL_TEXTURE_2D);
}

