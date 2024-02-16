#ifndef MD2_H
#define MD2_H

#include "pcx.h"

typedef struct {
    int ident;        // MD2 ファイルを識別するためのマジックナンバー
    int version;      // ファイルのバージョン。常に 8
    int skinwidth;    // ピクセル内のスキンの width
    int skinheight;   // ピクセル内のスキンの height
    int framesize;    // 1 フレームのサイズ(バイト)
    int numSkins;     // スキンの数
    int numXYZ;       // 1 フレームの頂点の数
    int numST;        // 1 フレームのテクスチャ座標の数
    int numTris;      // 1 フレームの三角形の数
    int numGLcmds;    // GL コマンドリストの数
    int numFrames;    // フレーム数
    int offsetSkins;  // スキンネームの位置のファイル先頭からのオフセット(バイト)
    int offsetST;     // テクスチャ座標リストの位置のファイル先頭からのオフセット(バイト)
    int offsetTris;   // 三角形リストの位置のファイル先頭からのオフセット(バイト)
    int offsetFrames; // フレームリストの位置のファイル先頭からのオフセット(バイト)
    int offsetGLcmds; // GL コマンドリストの位置のファイル先頭からのオフセット(バイト)
    int offsetEnd;    // ファイルサイズ
} md2modelHeader;

typedef struct {
    float pt[3];    // 頂点      
    float ln[3];    // 法線 
} md2coord;

typedef struct {
    unsigned short meshIndex[3]; // メッシュのインデクス
    unsigned short stIndex[3];   // テクスチャ座標のインデクス
} md2mesh;

typedef struct {
    float s;
    float t;
} md2stTexCoord;

typedef struct {
    short s;
    short t;
} md2stTexture;

typedef struct {
    int numPoints;            // 頂点の数
    md2coord *pointList;      // 頂点のリスト
    md2coord *currentMorph;   // 未使用
    int numTriangles;         // 三角形の数
    int numFrames;            // フレーム数
    md2mesh *triIndex;        // 三角形へのインデクス
    int numST;                // テクスチャ座標の数
    md2stTexCoord *st;        // テクスチャ座標のリスト
    int texWidth, texHeight;  // テクスチャの width / height
    char *texture;            // 未使用
    int currentFrame;         // 現在のフレーム
    int animationRange[2];    // アニメーションする範囲
    int gameFrames[2];        // 未使用
    float xscale;             // 未使用
    float yscale;             // 未使用
    float zscale;             // 未使用
    int introzoom;            // 未使用
    bool zoomstop;            // 未使用
    long zoomstoptime;        // 未使用
} md2modelData;

typedef struct {
    unsigned char v[3];             // スケールされた頂点
    unsigned char lightNormalIndex; // 法線のインデクス
} md2framePoint;

typedef struct {
    float scale[3];      // md2framePoint の v に作用させる
    float translate[3];  // md2framePoint の v に作用させる
    char name[16];       // フレームの名前
    md2framePoint fp[1]; // 頂点・法線データ
} md2frame;

class MD2
{
public:
    typedef enum {
        STAND,
        RUN,
        ATTACK,
        PAIN1,
        PAIN2,
        PAIN3,
        JUMP,
        FLIPOFF,
        SALUTE,
        TAUNT,
        WAVE,
        POINT,
        CROUCH_STAND,
        CROUCH_WALK,
        CROUCH_ATTACK,
        CROUCH_PAIN,
        CROUCH_DEATH,
        DEATH1,
        DEATH2,
        DEATH3
    } MOTION;

    MD2() : isRepeat(true) {};
    ~MD2() {};

    int load(const char *fileName, const char *textureName);

    void setRepeat(bool r) { isRepeat = r; } 
    void setFrame(int frame); 
    int getFrame(); 
    int getLastFrameNum(); 
    void setMotion(MOTION m);
    void resetTime(); 
    void setAnimationRange(int startframe, int stopframe); 

    void render(float x, float y, float z, int curframe,int nextframe,float pol);
    void animate(float x, float y, float z,float time);  

protected:
    PCX pcx; 
    md2modelData *model;
    bool isRepeat;

    long newtime,frametime,framestarttime;
};

#endif
