//-------------------------------------------------------------------
//      md2.cpp
//      Quake 2 MD2 モデル
//-------------------------------------------------------------------

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdlib.h>
#include "md2.h"

#define NUMVERTEXNORMALS 162

// 法線
float avertexnormals[NUMVERTEXNORMALS][3] = {
    {-0.525731f, 0.000000f, 0.850651f},
    {-0.442863f, 0.238856f, 0.864188f},
    {-0.295242f, 0.000000f, 0.955423f},
    {-0.309017f, 0.500000f, 0.809017f},
    {-0.162460f, 0.262866f, 0.951056f},
    {0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 0.850651f, 0.525731f},
    {-0.147621f, 0.716567f, 0.681718f},
    {0.147621f, 0.716567f, 0.681718f},
    {0.000000f, 0.525731f, 0.850651f},
    {0.309017f, 0.500000f, 0.809017f},
    {0.525731f, 0.000000f, 0.850651f},
    {0.295242f, 0.000000f, 0.955423f},
    {0.442863f, 0.238856f, 0.864188f},
    {0.162460f, 0.262866f, 0.951056f},
    {-0.681718f, 0.147621f, 0.716567f},
    {-0.809017f, 0.309017f, 0.500000f},
    {-0.587785f, 0.425325f, 0.688191f},
    {-0.850651f, 0.525731f, 0.000000f},
    {-0.864188f, 0.442863f, 0.238856f},
    {-0.716567f, 0.681718f, 0.147621f},
    {-0.688191f, 0.587785f, 0.425325f},
    {-0.500000f, 0.809017f, 0.309017f},
    {-0.238856f, 0.864188f, 0.442863f},
    {-0.425325f, 0.688191f, 0.587785f},
    {-0.716567f, 0.681718f, -0.147621f},
    {-0.500000f, 0.809017f, -0.309017f},
    {-0.525731f, 0.850651f, 0.000000f},
    {0.000000f, 0.850651f, -0.525731f},
    {-0.238856f, 0.864188f, -0.442863f},
    {0.000000f, 0.955423f, -0.295242f},
    {-0.262866f, 0.951056f, -0.162460f},
    {0.000000f, 1.000000f, 0.000000f},
    {0.000000f, 0.955423f, 0.295242f},
    {-0.262866f, 0.951056f, 0.162460f},
    {0.238856f, 0.864188f, 0.442863f},
    {0.262866f, 0.951056f, 0.162460f},
    {0.500000f, 0.809017f, 0.309017f},
    {0.238856f, 0.864188f, -0.442863f},
    {0.262866f, 0.951056f, -0.162460f},
    {0.500000f, 0.809017f, -0.309017f},
    {0.850651f, 0.525731f, 0.000000f},
    {0.716567f, 0.681718f, 0.147621f},
    {0.716567f, 0.681718f, -0.147621f},
    {0.525731f, 0.850651f, 0.000000f},
    {0.425325f, 0.688191f, 0.587785f},
    {0.864188f, 0.442863f, 0.238856f},
    {0.688191f, 0.587785f, 0.425325f},
    {0.809017f, 0.309017f, 0.500000f},
    {0.681718f, 0.147621f, 0.716567f},
    {0.587785f, 0.425325f, 0.688191f},
    {0.955423f, 0.295242f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.951056f, 0.162460f, 0.262866f},
    {0.850651f, -0.525731f, 0.000000f},
    {0.955423f, -0.295242f, 0.000000f},
    {0.864188f, -0.442863f, 0.238856f},
    {0.951056f, -0.162460f, 0.262866f},
    {0.809017f, -0.309017f, 0.500000f},
    {0.681718f, -0.147621f, 0.716567f},
    {0.850651f, 0.000000f, 0.525731f},
    {0.864188f, 0.442863f, -0.238856f},
    {0.809017f, 0.309017f, -0.500000f},
    {0.951056f, 0.162460f, -0.262866f},
    {0.525731f, 0.000000f, -0.850651f},
    {0.681718f, 0.147621f, -0.716567f},
    {0.681718f, -0.147621f, -0.716567f},
    {0.850651f, 0.000000f, -0.525731f},
    {0.809017f, -0.309017f, -0.500000f},
    {0.864188f, -0.442863f, -0.238856f},
    {0.951056f, -0.162460f, -0.262866f},
    {0.147621f, 0.716567f, -0.681718f},
    {0.309017f, 0.500000f, -0.809017f},
    {0.425325f, 0.688191f, -0.587785f},
    {0.442863f, 0.238856f, -0.864188f},
    {0.587785f, 0.425325f, -0.688191f},
    {0.688191f, 0.587785f, -0.425325f},
    {-0.147621f, 0.716567f, -0.681718f},
    {-0.309017f, 0.500000f, -0.809017f},
    {0.000000f, 0.525731f, -0.850651f},
    {-0.525731f, 0.000000f, -0.850651f},
    {-0.442863f, 0.238856f, -0.864188f},
    {-0.295242f, 0.000000f, -0.955423f},
    {-0.162460f, 0.262866f, -0.951056f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.295242f, 0.000000f, -0.955423f},
    {0.162460f, 0.262866f, -0.951056f},
    {-0.442863f, -0.238856f, -0.864188f},
    {-0.309017f, -0.500000f, -0.809017f},
    {-0.162460f, -0.262866f, -0.951056f},
    {0.000000f, -0.850651f, -0.525731f},
    {-0.147621f, -0.716567f, -0.681718f},
    {0.147621f, -0.716567f, -0.681718f},
    {0.000000f, -0.525731f, -0.850651f},
    {0.309017f, -0.500000f, -0.809017f},
    {0.442863f, -0.238856f, -0.864188f},
    {0.162460f, -0.262866f, -0.951056f},
    {0.238856f, -0.864188f, -0.442863f},
    {0.500000f, -0.809017f, -0.309017f},
    {0.425325f, -0.688191f, -0.587785f},
    {0.716567f, -0.681718f, -0.147621f},
    {0.688191f, -0.587785f, -0.425325f},
    {0.587785f, -0.425325f, -0.688191f},
    {0.000000f, -0.955423f, -0.295242f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.262866f, -0.951056f, -0.162460f},
    {0.000000f, -0.850651f, 0.525731f},
    {0.000000f, -0.955423f, 0.295242f},
    {0.238856f, -0.864188f, 0.442863f},
    {0.262866f, -0.951056f, 0.162460f},
    {0.500000f, -0.809017f, 0.309017f},
    {0.716567f, -0.681718f, 0.147621f},
    {0.525731f, -0.850651f, 0.000000f},
    {-0.238856f, -0.864188f, -0.442863f},
    {-0.500000f, -0.809017f, -0.309017f},
    {-0.262866f, -0.951056f, -0.162460f},
    {-0.850651f, -0.525731f, 0.000000f},
    {-0.716567f, -0.681718f, -0.147621f},
    {-0.716567f, -0.681718f, 0.147621f},
    {-0.525731f, -0.850651f, 0.000000f},
    {-0.500000f, -0.809017f, 0.309017f},
    {-0.238856f, -0.864188f, 0.442863f},
    {-0.262866f, -0.951056f, 0.162460f},
    {-0.864188f, -0.442863f, 0.238856f},
    {-0.809017f, -0.309017f, 0.500000f},
    {-0.688191f, -0.587785f, 0.425325f},
    {-0.681718f, -0.147621f, 0.716567f},
    {-0.442863f, -0.238856f, 0.864188f},
    {-0.587785f, -0.425325f, 0.688191f},
    {-0.309017f, -0.500000f, 0.809017f},
    {-0.147621f, -0.716567f, 0.681718f},
    {-0.425325f, -0.688191f, 0.587785f},
    {-0.162460f, -0.262866f, 0.951056f},
    {0.442863f, -0.238856f, 0.864188f},
    {0.162460f, -0.262866f, 0.951056f},
    {0.309017f, -0.500000f, 0.809017f},
    {0.147621f, -0.716567f, 0.681718f},
    {0.000000f, -0.525731f, 0.850651f},
    {0.425325f, -0.688191f, 0.587785f},
    {0.587785f, -0.425325f, 0.688191f},
    {0.688191f, -0.587785f, 0.425325f},
    {-0.955423f, 0.295242f, 0.000000f},
    {-0.951056f, 0.162460f, 0.262866f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.850651f, 0.000000f, 0.525731f},
    {-0.955423f, -0.295242f, 0.000000f},
    {-0.951056f, -0.162460f, 0.262866f},
    {-0.864188f, 0.442863f, -0.238856f},
    {-0.951056f, 0.162460f, -0.262866f},
    {-0.809017f, 0.309017f, -0.500000f},
    {-0.864188f, -0.442863f, -0.238856f},
    {-0.951056f, -0.162460f, -0.262866f},
    {-0.809017f, -0.309017f, -0.500000f},
    {-0.681718f, 0.147621f, -0.716567f},
    {-0.681718f, -0.147621f, -0.716567f},
    {-0.850651f, 0.000000f, -0.525731f},
    {-0.688191f, 0.587785f, -0.425325f},
    {-0.587785f, 0.425325f, -0.688191f},
    {-0.425325f, 0.688191f, -0.587785f},
    {-0.425325f, -0.688191f, -0.587785f},
    {-0.587785f, -0.425325f, -0.688191f},
    {-0.688191f, -0.587785f, -0.425325f},
};

void MD2::resetTime()
{
    frametime=0;
    framestarttime=0;
}

void MD2::animate(float x, float y, float z,float time)
{ 
    newtime = SDL_GetTicks();
    if(framestarttime==0) framestarttime=newtime;
    frametime=newtime-framestarttime;

    float tim=frametime/time;

    if(getFrame()+1>model->animationRange[1]) {
        render(x,y,z,getFrame(),model->animationRange[0],tim);
    } else {
        render(x,y,z,getFrame(),getFrame()+1,tim);
    }

    if((frametime/time)>=1) {
        framestarttime=newtime;
        setFrame(getFrame()+1);
        if (isRepeat) {
            if(getFrame() > model->animationRange[1])
                setFrame(model->animationRange[0]);
        }
        else {
            setAnimationRange(model->animationRange[1], model->animationRange[1]);
        }
    } 
}

void MD2::setAnimationRange(int start, int stop)
{
    model->animationRange[0] = start;
    model->animationRange[1] = stop;
}

void MD2::setFrame(int frame)
{
    model->currentFrame = frame;
}

int MD2::getFrame()
{
    return model->currentFrame;
}

int MD2::getLastFrameNum()
{
    return model->numFrames-1;
}

void MD2::setMotion(MOTION m)
{
    int start = 0, end = 0;

    switch(m) {
    case STAND:
        start = 0; end = 39;
        break;
    case RUN:
        start = 40; end = 45;
        break;
    case ATTACK:
        start = 46; end = 53;
        break;
    case PAIN1:
        start = 54; end = 57;
        break;
    case PAIN2:
        start = 58; end = 61;
        break;
    case PAIN3:
        start = 62; end = 65;
        break;
    case JUMP:
        start = 66; end = 71;
        break;
    case FLIPOFF:
        start = 72; end = 83;
        break;
    case SALUTE:
        start = 84; end = 94;
        break;
    case TAUNT:
        start = 95; end = 111;
        break;
    case WAVE:
        start = 112; end = 122;
        break;
    case POINT:
        start = 123; end = 134;
        break;
    case CROUCH_STAND:
        start = 135; end = 153;
        break;
    case CROUCH_WALK:
        start = 154; end = 159;
        break;
    case CROUCH_ATTACK:
        start = 160; end = 168;
        break;
    case CROUCH_PAIN:
        start = 169; end = 172;
        break;
    case CROUCH_DEATH:
        start = 173; end = 177;
        break;
    case DEATH1:
        start = 178; end = 183;
        break;
    case DEATH2:
        start = 184; end = 189;
        break;
    case DEATH3:
        start = 190; end = 198;
        break;
    default:
        break;
    }
    setFrame(start);
    setAnimationRange(start, end);
} 

void MD2::render(float x, float y, float z, int curframe,int nextframe,float pol)
{
    float  x1, y1, z1, x2, y2, z2;
    float nx1,ny1,nz1,nx2,ny2,nz2;
    if(pol>1) pol=1;

    md2coord *pointListPtr;
    md2coord *pointListPtr2;
    pointListPtr = &model->pointList[ model->numPoints * curframe];
    pointListPtr2 = &model->pointList[ model->numPoints * nextframe];

    glBindTexture(GL_TEXTURE_2D, pcx.texID);
    glBegin(GL_TRIANGLES);

    for(int i=0; i<model->numTriangles; i++ ) {
        glTexCoord2f(model->st[ model->triIndex[i].stIndex[0] ].s, model->st[ model->triIndex[i].stIndex[0] ].t);

        x1 = pointListPtr[ model->triIndex[i].meshIndex[0] ].pt[0];
        y1 = pointListPtr[ model->triIndex[i].meshIndex[0] ].pt[1];
        z1 = pointListPtr[ model->triIndex[i].meshIndex[0] ].pt[2];
        x2 = pointListPtr2[ model->triIndex[i].meshIndex[0] ].pt[0];
        y2 = pointListPtr2[ model->triIndex[i].meshIndex[0] ].pt[1];
        z2 = pointListPtr2[ model->triIndex[i].meshIndex[0] ].pt[2];
        nx1 = pointListPtr[ model->triIndex[i].meshIndex[0] ].ln[0];
        ny1 = pointListPtr[ model->triIndex[i].meshIndex[0] ].ln[1];
        nz1 = pointListPtr[ model->triIndex[i].meshIndex[0] ].ln[2];
        nx2 = pointListPtr2[ model->triIndex[i].meshIndex[0] ].ln[0];
        ny2 = pointListPtr2[ model->triIndex[i].meshIndex[0] ].ln[1];
        nz2 = pointListPtr2[ model->triIndex[i].meshIndex[0] ].ln[2];

        glNormal3f( x+nx1 + pol * (nx2 - nx1), y+ny1 + pol * (ny2 - ny1), z+nz1 + pol * (nz2 - nz1) );
        glVertex3f( x+x1 + pol * (x2 - x1), y+y1 + pol * (y2 - y1), z+z1 + pol * (z2 - z1) );

        glTexCoord2f( model->st[ model->triIndex[i].stIndex[1] ].s, model->st[ model->triIndex[i].stIndex[1] ].t );
        x1 = pointListPtr[ model->triIndex[i].meshIndex[1] ].pt[0];
        y1 = pointListPtr[ model->triIndex[i].meshIndex[1] ].pt[1];
        z1 = pointListPtr[ model->triIndex[i].meshIndex[1] ].pt[2];
        x2 = pointListPtr2[ model->triIndex[i].meshIndex[1] ].pt[0];
        y2 = pointListPtr2[ model->triIndex[i].meshIndex[1] ].pt[1];
        z2 = pointListPtr2[ model->triIndex[i].meshIndex[1] ].pt[2];
        nx1 = pointListPtr[ model->triIndex[i].meshIndex[1] ].ln[0];
        ny1 = pointListPtr[ model->triIndex[i].meshIndex[1] ].ln[1];
        nz1 = pointListPtr[ model->triIndex[i].meshIndex[1] ].ln[2];
        nx2 = pointListPtr2[ model->triIndex[i].meshIndex[1] ].ln[0];
        ny2 = pointListPtr2[ model->triIndex[i].meshIndex[1] ].ln[1];
        nz2 = pointListPtr2[ model->triIndex[i].meshIndex[1] ].ln[2];
        glNormal3f( x+nx1 + pol * (nx2 - nx1), y+ny1 + pol * (ny2 - ny1), z+nz1 + pol * (nz2 - nz1) );
        glVertex3f( x+x1 + pol * (x2 - x1), y+y1 + pol * (y2 - y1), z+z1 + pol * (z2 - z1) );


        glTexCoord2f( model->st[ model->triIndex[i].stIndex[2] ].s, model->st[ model->triIndex[i].stIndex[2] ].t );
        x1 = pointListPtr[ model->triIndex[i].meshIndex[2] ].pt[0];
        y1 = pointListPtr[ model->triIndex[i].meshIndex[2] ].pt[1];
        z1 = pointListPtr[ model->triIndex[i].meshIndex[2] ].pt[2];
        x2 = pointListPtr2[ model->triIndex[i].meshIndex[2] ].pt[0];
        y2 = pointListPtr2[ model->triIndex[i].meshIndex[2] ].pt[1];
        z2 = pointListPtr2[ model->triIndex[i].meshIndex[2] ].pt[2];
        nx1 = pointListPtr[ model->triIndex[i].meshIndex[2] ].ln[0];
        ny1 = pointListPtr[ model->triIndex[i].meshIndex[2] ].ln[1];
        nz1 = pointListPtr[ model->triIndex[i].meshIndex[2] ].ln[2];
        nx2 = pointListPtr2[ model->triIndex[i].meshIndex[2] ].ln[0];
        ny2 = pointListPtr2[ model->triIndex[i].meshIndex[2] ].ln[1];
        nz2 = pointListPtr2[ model->triIndex[i].meshIndex[2] ].ln[2];
        glNormal3f( x+nx1 + pol * (nx2 - nx1), y+ny1 + pol * (ny2 - ny1), z+nz1 + pol * (nz2 - nz1) );
        glVertex3f( x+x1 + pol * (x2 - x1), y+y1 + pol * (y2 - y1), z+z1 + pol * (z2 - z1) );

    }

    glEnd();
}

int MD2::load(const char *fileName, const char *textureName)
{
    FILE *fp = fopen(fileName, "rb");

    if (fp == NULL) {
        return -1;
    }

    int fileLen;
    char *buffer;
    md2modelHeader *mdh;
    md2frame *frm;
    md2coord *pointList, *pointListPtr;
    md2mesh *triIndex, *bufIndexPtr;
    md2stTexCoord *st;
    md2stTexture *stPtr;
    int i, j;

    fseek(fp, 0, SEEK_END);
    fileLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer = (char *)malloc( fileLen+1 );
    fread( buffer, sizeof( char ), fileLen, fp);

    mdh = (md2modelHeader *)buffer;

    model = (md2modelData *)malloc( sizeof( md2modelData ));

    pointList = (md2coord *)malloc( sizeof( md2coord ) * mdh->numXYZ * mdh->numFrames );
    model->pointList = pointList;
    model->numPoints = mdh->numXYZ;
    model->numFrames = mdh->numFrames;

    model->currentMorph = (md2coord *)malloc( sizeof( md2coord ) * mdh->numXYZ);

    for( j=0; j<mdh->numFrames; j++ ) {
        frm = (md2frame *)&buffer[ mdh->offsetFrames + mdh->framesize * j ];
        pointListPtr = (md2coord *)&pointList[ mdh->numXYZ * j ];
        for( i=0; i<mdh->numXYZ; i++ ) {
            pointListPtr[i].pt[0] = frm->scale[0] * frm->fp[i].v[0] + frm->translate[0];
            pointListPtr[i].pt[2] = frm->scale[1] * frm->fp[i].v[1] + frm->translate[1];
            pointListPtr[i].pt[1] = frm->scale[2] * frm->fp[i].v[2] + frm->translate[2];

            pointListPtr[i].ln[0] = avertexnormals[frm->fp[i].lightNormalIndex][0];
            pointListPtr[i].ln[1] = avertexnormals[frm->fp[i].lightNormalIndex][1];
            pointListPtr[i].ln[2] = avertexnormals[frm->fp[i].lightNormalIndex][2];

            pointListPtr[i].pt[0] /= 64;
            pointListPtr[i].pt[1] /= 64;
            pointListPtr[i].pt[2] /= 64;
        }
    }

    // PCX を読み込む
    if (pcx.load(textureName) != 0)
        return -1;

    // テクスチャ座標を計算
    st = (md2stTexCoord *)malloc( sizeof( md2stTexCoord ) * mdh->numST );
    model->numST = mdh->numST;
    model->st = st;

    stPtr = (md2stTexture *)&buffer[mdh->offsetST];
    for( i=0; i<mdh->numST; i++ ) {
        st[i].s = (float)stPtr[i].s / (float)pcx.width;
        st[i].t = (float)stPtr[i].t / (float)pcx.width;
    }

    model->zoomstop = false;

    // メッシュリスト作成
    triIndex = (md2mesh *)malloc( sizeof( md2mesh ) * mdh->numTris );
    model->numTriangles = mdh->numTris;
    model->triIndex = triIndex;

    bufIndexPtr = (md2mesh *)&buffer[mdh->offsetTris];

    for(i=0; i<mdh->numTris; i++) {
        triIndex[i].meshIndex[0] = bufIndexPtr[i].meshIndex[0];
        triIndex[i].meshIndex[1] = bufIndexPtr[i].meshIndex[1];
        triIndex[i].meshIndex[2] = bufIndexPtr[i].meshIndex[2];
        triIndex[i].stIndex[0] = bufIndexPtr[i].stIndex[0];
        triIndex[i].stIndex[1] = bufIndexPtr[i].stIndex[1];
        triIndex[i].stIndex[2] = bufIndexPtr[i].stIndex[2];
    }

    free(buffer);
    return 0;
}

