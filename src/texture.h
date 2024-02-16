#ifndef TEXTURE_H
#define TEXTURE_H

unsigned char *loadTGA(const char* filename, int* width, int* height);
int createTextureFromTGA(char *filename, GLuint *texID);

#endif
