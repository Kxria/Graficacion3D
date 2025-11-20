#pragma once

#include <stdint.h>
#include <SDL3/SDL.h>
#include "estructuras/mesh.h"
#include "color/colores.h"
#include "draw/figuras.h"

typedef struct _estado {
    SDL_Renderer *renderer;
    SDL_Texture *textura;
    SDL_Event evento;
    uint32_t *color_buffer;
    float *z_buffer;
    int w_width;
    int w_height;
    Color clear_color;
    int run;
    Mesh *meshes;

    // Figuras *figuras_buffer;
    // Figuras *figuras_temp_buffer;
}EstadoRender;

extern EstadoRender estadosrender;
