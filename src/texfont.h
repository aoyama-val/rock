#ifndef TEXFONT_H
#define TEXFONT_H

#include <SDL_opengl.h>

class TexFont {
public:
    TexFont() : sizex(1.0), sizey(1.0) {}

    ~TexFont() {
        deleteFont();
    }

    int load(const char* filename);
    void deleteFont();

    void print(int x, int y, char type, const char *fmt, ...);

    void setSize(float x, float y) {
        sizex = x;
        sizey = y;
    };

    GLuint texID;
    int base;
    float sizex, sizey;
protected:
};

#endif
