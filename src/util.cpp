//-------------------------------------------------------------------
//      util.cpp
//      補助関数群
//-------------------------------------------------------------------

#include <stdarg.h>
#include "common.h"
#include "util.h"
#include "display.h"
#include "main.h"

#ifdef DEBUG
static FILE *debugFp = NULL;
#endif
static FILE *errorFp = NULL;

double randDouble(void)
{
    return rand()/(double)RAND_MAX;
}

// bufのはじめの部分がindexに一致する → その次の文字へのポインタ
// しない → NULL 
const char* startswith(const char *buf, const char *index)
{
    int len = strlen(index);

    if (strncmp(buf, index, len) == 0)
        return buf + len;
    else
        return NULL;
} 

void printfv(FILE* stream, float *f, int n)
{
    int i;

    for (i=0; i<n; i++) {
        fprintf(stream, "%f ", f[n]);
    }
    fprintf(stream, "\n");
}

// デバッグメッセージ用printf
void dprintf(const char *format, ...)
{
#if DEBUG
    va_list argpt;

    if (debugFp == NULL) {
        debugFp = fopen("debuglog.txt", "w"); 
    }

    va_start(argpt, format);
    vfprintf(stderr, format, argpt);
    vfprintf(debugFp, format, argpt);
    va_end(argpt);
#else
    (void)format;
#endif
}

// エラーメッセージ用printf
void eprintf(const char *format, ...)
{
    va_list argpt;

    if (errorFp == NULL) {
        errorFp = fopen("errorlog.txt", "w"); 
    }

    va_start(argpt, format);
    vfprintf(stderr, format, argpt);
    vfprintf(errorFp, format, argpt);
    fflush(errorFp); 
    va_end(argpt);
}

// OpenGL描画エラーの有無をチェック
int OpenGLErrorCheck(void)
{
    GLenum errCode;
    const GLubyte *errString;

    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        eprintf("OpenGL Error: %s\n", errString);
        return -1;
    }
    return 0;
}
