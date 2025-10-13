#include "render.h"
#include "draw/draw.h"
#include "draw/linea.h"
#include "math/lerp.h"
#include "math/vectores.h"
#include "math/matrix.h"
#include "draw/figuras.h"
#include "memoria/memoria.h"
#include "color/colores.h"

#include <math.h>
#include <stdio.h>


#define N_PUNTOS 8
#define N_CARAS 6*2

Vec3 cubo_puntos[N_PUNTOS] = {
    {{-1, -1, -1}}, // 1
    {{-1,  1, -1}}, // 2
    {{ 1,  1, -1}}, // 3
    {{ 1, -1, -1}}, // 4
    {{ 1,  1,  1}}, // 5
    {{ 1, -1,  1}}, // 6
    {{-1,  1,  1}}, // 7
    {{-1, -1,  1}}, // 8
};

Cara_t cubo_caras[N_CARAS] = {
    {.a = 1,.b = 2, .c = 3},
    {.a = 1,.b = 3, .c = 4},
    
    {.a = 4,.b = 3, .c = 5},
    {.a = 4,.b = 5, .c = 6},
    
    {.a = 6,.b = 5, .c = 7},
    {.a = 6,.b = 7, .c = 8},

    {.a = 8,.b = 7, .c = 2},
    {.a = 8,.b = 2, .c = 1},

    {.a = 2,.b = 7, .c = 5},
    {.a = 2,.b = 5, .c = 3},

    {.a = 8,.b = 8, .c = 1},
    {.a = 8,.b = 1, .c = 4},
};

Triangulo cubo_triangulos[N_CARAS];

Vec3 camara;
Vec3 rotaciones;
Vec3 escalamiento;
const float fovf = 630;

Vec2 *punto_seleccionado = NULL;

void transformar(void) {
    // por cada cara
    for(int i = 0 ; i < N_CARAS ; i++) {
        Cara_t punto_cara = cubo_caras[i];
        Vec3 cara_vertice[3];
        cara_vertice[2] = cubo_puntos[punto_cara.c - 1];
        cara_vertice[0] = cubo_puntos[punto_cara.a - 1];
        cara_vertice[1] = cubo_puntos[punto_cara.b - 1];
        
        // por cada vertice
        Triangulo triangulo_proyectado;
        for(int j = 0 ; j < 3 ; ++j) {
            Vec3 punto = cara_vertice[j];
    
            // matriz de transformacion
            Mat4 mt = mat4_eye();
    
            // escalamiento
            mat4_push_escala(&mt, escalamiento);
            mat4_push_rotar(&mt, rotaciones);
            mat4_push_traslado(&mt, camara);
    
            Vec4 p = {{punto.unpack.x, punto.unpack.y, punto.unpack.z, 1.f}};
            p = mat4_dot_vec4(&mt, &p);
            punto = vec4_to_vec3(&p);
            Vec2 punto_proyectado = proyeccion_perspectiva(punto, fovf);

            // mover centro
            punto_proyectado.unpack.x += estadosrender.w_width / 2.f;
            punto_proyectado.unpack.y += estadosrender.w_height / 2.f;
            
            // generar triangulo
            triangulo_proyectado.p[j] = punto_proyectado;
        }
        cubo_triangulos[i] = triangulo_proyectado;
    }
}

Vec2* pivote_mas_cerca(Vec2 mp, Figuras *figs, float umbral) {
    
    int a = 0;
    int b = array_size(figs) - 1;
    
    float low;
    float high;
    float centro;

    for(int i = 0 ; i < array_size(figs) ; i++) {
        int r = (a + b) / 2;

        low = distanciav2(mp, figs[a].cuadro.pos);
        high = distanciav2(mp, figs[b].cuadro.pos);
        centro = distanciav2(mp, figs[r].cuadro.pos);

        if(fabs(low) <= umbral) {
            return &figs[a].cuadro.pos;
        } else if (fabs(high) <= umbral) {
            return &figs[b].cuadro.pos;
        } else if (fabs(centro) <= umbral) {
            return &figs[r].cuadro.pos;
        }

        if(r < b) {
            b = r;
        } else if (r > a) {
            a = r;
        }
    }

    return NULL;
}

void update() {
    rotaciones.unpack.x += 0.001f;
    rotaciones.unpack.y += 0.001f;
    rotaciones.unpack.z += 0.001f;
    transformar();
}


void render_input(void) {
    if(estadosrender.evento.type == SDL_EVENT_QUIT){
        estadosrender.run = 0;
    }
}

void clear_color_buffer() {
    for(int y = 0; y < estadosrender.w_height; ++y){
        for(int x = 0; x < estadosrender.w_width; ++x){
            draw_pixel(x, y, estadosrender.clear_color.hex);
        }
    }
}

void copy_buffer_to_texture() {
    SDL_UpdateTexture(estadosrender.texture, 
                      NULL, 
                      estadosrender.color_buffer, 
                      (int)(estadosrender.w_width * sizeof(int)));

    SDL_RenderTexture(estadosrender.renderer, 
                      estadosrender.texture, 
                      NULL, 
                      NULL);
}

void _Init() {
    rotaciones.unpack.x = 0.f;
    rotaciones.unpack.y = 0.f;
    rotaciones.unpack.z = 0.f;

    escalamiento.unpack.x = 1.f;
    escalamiento.unpack.y = 1.f;
    escalamiento.unpack.z = 1.f;

    camara.unpack.z = -5.f;
}

void render_frame() {
    // renderizar caras
    for(int i = 0 ; i < N_CARAS ; i++) {
        Triangulo trian = cubo_triangulos[i];

        // vertuces triangulos
        fill_cuadro(trian.p[0], 4, 4, 0xFF00FFFF, 0xFF00FFFF);
        fill_cuadro(trian.p[1], 4, 4, 0xFF00FFFF, 0xFF00FFFF);
        fill_cuadro(trian.p[2], 4, 4, 0xFF00FFFF, 0xFF00FFFF);

        draw_trian(trian.p[0].unpack.x, trian.p[0].unpack.y, 
                   trian.p[1].unpack.x, trian.p[1].unpack.y, 
                   trian.p[2].unpack.x, trian.p[2].unpack.y, 0xFF00FFFF);
    }

    SDL_RenderPresent(estadosrender.renderer);
}