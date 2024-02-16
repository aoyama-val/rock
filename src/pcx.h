#ifndef PCX_H
#define PCX_H

class PCX
{
public:
    PCX() {};
    ~PCX() {};

    int load(const char *filename);

    int bpp;
    int width;
    int height;
    unsigned int texID; 
};

#endif

