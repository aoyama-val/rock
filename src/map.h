#ifndef MAP_H
#define MAP_H

class Map {
public:
    void draw();
    int load(const char* filename);
    void setTexture(unsigned int texID_) { texID = texID_; }
    void createDisplayList();
    void deleteDisplayList();
    float getHeight(float x, float y);
    void getGravity(float x, float y, float *gra);
    bool isInside(float x, float y);
    void getBoundary(float *min, float *max);
protected:
    unsigned int texID;
    unsigned int dlistID;
    static const int MAX_NUMX = 16;
    static const int MAX_NUMY = 16;
    float scale;
    int numx, numy;
    float verteces[MAX_NUMX][MAX_NUMY][3];
};

#endif
