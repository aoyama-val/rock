//-------------------------------------------------------------------
//      texture.cpp
//      テクスチャローダ
//-------------------------------------------------------------------

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdlib.h>

#include "texture.h"

// TGA ファイルを読み込み、RGBA のデータとして返す
unsigned char *loadTGA(const char* filename, int* width, int* height)
{
    unsigned char rep;
    unsigned char* data;
    unsigned char* buf;
    unsigned char* ptr;
    unsigned char info[18];
    int w, h, bpp, size;
    int i, j, k, l;
    FILE* fp;

    fp = fopen(filename, "rb");

    if (fp == NULL)
        return 0;

    fread(info, 1, 18, fp);

    w = info[12] + info[13] * 256;
    h = info[14] + info[15] * 256;

    switch(info[16]) {
    case 32:
        bpp = 4;
        break;
    case 24: 
        bpp = 3;
        break;
    default:
        fclose(fp);
        return 0;
    }

    size = w * h * bpp;

    buf = (unsigned char*)malloc(size);
    data = (unsigned char*)malloc(w*h*4);

    fseek(fp, info[0], SEEK_CUR);

    switch(info[2]) {
    case 2:     // 非圧縮
        fread(buf, 1, size, fp);
        break;
    case 10:    // RLE 圧縮
        ptr = buf; 
        i = 0;
        while(i < size) {
            fread(&rep, 1, 1, fp);
            if (rep & 0x80) {
                rep ^= 0x80;
                fread(ptr, 1, bpp, fp);
                ptr += bpp;
                for (j=0; j<rep*bpp; j++) {
                    *ptr = *(ptr - bpp);
                    ptr++;
                }
                i += bpp * (rep+1);
            }
            else {
                k = bpp * (rep+1);
                fread(ptr, 1, k, fp);
                ptr += k;
                i += k;
            }
        }
        break;
    default:
        fclose(fp);
        free(buf);
        free(data);
        return 0;
    }

    // BGR -> RGB
    for (i=0, j=0; i<size; i+= bpp, j+=4) {
        data[j] = buf[i+2];
        data[j+1] = buf[i+1];
        data[j+2] = buf[i];
        if (bpp == 4) 
            data[j+3] = buf[i+3];
        else
            data[j+3] = 255;
    }

    // 上下反転
    if (!info[17] & 0x20) {
        k = w * 4;
        for (j=0; j<h/2; j++) {
            for (i=0; i<w*4; i++) {
                l = data[j*k+i];
                data[j*k+i] = data[(h-j-1)*k+i];
                data[(h-j-1)*k+i] = l;
            }
        }
    }

    fclose(fp);
    free(buf);
    *width = w;
    *height = h;

    return data;
}

int createTextureFromTGA(char *filename, GLuint *texID)
{
    unsigned char* data;
    int w, h;

    data = loadTGA(filename, &w, &h);
    if (data == NULL) {
        return -1;
    }
    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_2D, *texID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

    free(data);

    return 0; 
}
