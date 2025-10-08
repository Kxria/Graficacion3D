#pragma once

#include <stdint.h>
#include <SDL3/SDL.h>
#include "color/colores.h"
#include "draw/figuras.h"
// #include "HUD/boton.h"

typedef struct _estado {
    uint32_t *color_buffer;
    SDL_Event evento;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int w_width;
    int w_height;
    Color clear_color;
    int run;
    Figuras *figuras_buffer;
}EstadoRender;

extern EstadoRender estadosrender;