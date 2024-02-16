#ifndef MD2_H
#define MD2_H

#include "pcx.h"

typedef struct {
    int ident;        // MD2 �t�@�C�������ʂ��邽�߂̃}�W�b�N�i���o�[
    int version;      // �t�@�C���̃o�[�W�����B��� 8
    int skinwidth;    // �s�N�Z�����̃X�L���� width
    int skinheight;   // �s�N�Z�����̃X�L���� height
    int framesize;    // 1 �t���[���̃T�C�Y(�o�C�g)
    int numSkins;     // �X�L���̐�
    int numXYZ;       // 1 �t���[���̒��_�̐�
    int numST;        // 1 �t���[���̃e�N�X�`�����W�̐�
    int numTris;      // 1 �t���[���̎O�p�`�̐�
    int numGLcmds;    // GL �R�}���h���X�g�̐�
    int numFrames;    // �t���[����
    int offsetSkins;  // �X�L���l�[���̈ʒu�̃t�@�C���擪����̃I�t�Z�b�g(�o�C�g)
    int offsetST;     // �e�N�X�`�����W���X�g�̈ʒu�̃t�@�C���擪����̃I�t�Z�b�g(�o�C�g)
    int offsetTris;   // �O�p�`���X�g�̈ʒu�̃t�@�C���擪����̃I�t�Z�b�g(�o�C�g)
    int offsetFrames; // �t���[�����X�g�̈ʒu�̃t�@�C���擪����̃I�t�Z�b�g(�o�C�g)
    int offsetGLcmds; // GL �R�}���h���X�g�̈ʒu�̃t�@�C���擪����̃I�t�Z�b�g(�o�C�g)
    int offsetEnd;    // �t�@�C���T�C�Y
} md2modelHeader;

typedef struct {
    float pt[3];    // ���_      
    float ln[3];    // �@�� 
} md2coord;

typedef struct {
    unsigned short meshIndex[3]; // ���b�V���̃C���f�N�X
    unsigned short stIndex[3];   // �e�N�X�`�����W�̃C���f�N�X
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
    int numPoints;            // ���_�̐�
    md2coord *pointList;      // ���_�̃��X�g
    md2coord *currentMorph;   // ���g�p
    int numTriangles;         // �O�p�`�̐�
    int numFrames;            // �t���[����
    md2mesh *triIndex;        // �O�p�`�ւ̃C���f�N�X
    int numST;                // �e�N�X�`�����W�̐�
    md2stTexCoord *st;        // �e�N�X�`�����W�̃��X�g
    int texWidth, texHeight;  // �e�N�X�`���� width / height
    char *texture;            // ���g�p
    int currentFrame;         // ���݂̃t���[��
    int animationRange[2];    // �A�j���[�V��������͈�
    int gameFrames[2];        // ���g�p
    float xscale;             // ���g�p
    float yscale;             // ���g�p
    float zscale;             // ���g�p
    int introzoom;            // ���g�p
    bool zoomstop;            // ���g�p
    long zoomstoptime;        // ���g�p
} md2modelData;

typedef struct {
    unsigned char v[3];             // �X�P�[�����ꂽ���_
    unsigned char lightNormalIndex; // �@���̃C���f�N�X
} md2framePoint;

typedef struct {
    float scale[3];      // md2framePoint �� v �ɍ�p������
    float translate[3];  // md2framePoint �� v �ɍ�p������
    char name[16];       // �t���[���̖��O
    md2framePoint fp[1]; // ���_�E�@���f�[�^
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
