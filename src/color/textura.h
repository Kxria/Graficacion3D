#pragma once
#include <stdint.h>

typedef struct _texturaUV {
    float u;
    float v;
}TexturaUV;

typedef struct _textura {
    int width;
    int height;
    uint32_t *pixeles;
}Textura;

void swapUV(TexturaUV *tuv1, TexturaUV *tuv2);