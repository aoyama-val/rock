//-------------------------------------------------------------------
//      pcx.cpp
//      PCX テクスチャローダ
//-------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <SDL_opengl.h>
#include "pcx.h"

// ファイルの先頭
typedef struct {
    short id[2];        // マジックナンバー。常に 0a 05 01 08
    short offset[2];
    short size[2];
} pcxHeader;

int PCX::load(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
        return -1;

    int w, h;
    int filesize;
    int imgBufferPtr;
    pcxHeader *pcxPtr;
    unsigned char* buf;
    unsigned char *imgBuffer, *pcxBufferPtr, *paletteBuffer; 
    GLubyte *imgData;

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buf = (unsigned char *)malloc(filesize+1);
    fread(buf, sizeof(char), filesize, fp);

    pcxPtr = (pcxHeader *)buf;
    w = pcxPtr->size[0] - pcxPtr->offset[0] + 1;
    h = pcxPtr->size[1] - pcxPtr->offset[1] + 1;

    imgBuffer = (unsigned char *)malloc(w*h);
    imgBufferPtr = 0;

    pcxBufferPtr = &buf[128];

    // 圧縮を展開
    while(imgBufferPtr < w * h) {
        if(*pcxBufferPtr > 0xbf) {
            int repeat = *pcxBufferPtr++ & 0x3f;
            for(int i=0; i<repeat; i++ ) {
                imgBuffer[imgBufferPtr++] = *pcxBufferPtr;
            } 
        } else {
            imgBuffer[imgBufferPtr++] = *pcxBufferPtr;
        }
        pcxBufferPtr++;
    }

    paletteBuffer = &buf[filesize-768];

    // glTexImage2D に渡すデータを作成
    imgData = (unsigned char *)malloc(w* h* 3);
    for (int j=0; j < h; j++) {
        for (int i=0; i < w; i++) {
            imgData[3*(j*w+i)+0] = paletteBuffer[3*imgBuffer[j*w+i]+0];
            imgData[3*(j*w+i)+1] = paletteBuffer[3*imgBuffer[j*w+i]+1];
            imgData[3*(j*w+i)+2] = paletteBuffer[3*imgBuffer[j*w+i]+2];
        }
    }

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h,0, GL_RGB, GL_UNSIGNED_BYTE, imgData);

    // メンバ変数セット
    width = w;
    height = h;
    bpp = 24;

    free(buf);
    free(imgBuffer);
    free(imgData); 

    return 0;
}
