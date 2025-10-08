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

#define N_PUNTOS 9*9*9
Vec3 cubo_puntos[N_PUNTOS];
Vec2 puntos_transformados[N_PUNTOS];

Vec3 camara;
Vec3 rotaciones;
Vec3 escalamiento;

const float fovf = 640;

Vec2 *punto_seleccionado = NULL;

void transformar(void) {
    // por cada punto
    for(int i = 0 ; i < N_PUNTOS ; i++) {
        Vec3 vertice = cubo_puntos[i];

        // matriz de transformacion
        Mat4 mt = mat4_eye();

        // escalamiento
        mat4_push_escala(&mt, escalamiento);
        mat4_push_rotar(&mt, rotaciones);
        mat4_push_traslado(&mt, camara);

        Vec4 p = {{vertice.unpack.x, vertice.unpack.y, vertice.unpack.z, 1.f}};

        p = mat4_dot_vec4(&mt, &p);
        vertice = vec4_to_vec3(&p);
        puntos_transformados[i] = proyeccion_perspectiva(vertice, fovf);

        puntos_transformados[i].unpack.x += estadosrender.w_width / 2.f;
        puntos_transformados[i].unpack.y += estadosrender.w_height / 2.f;
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

    // en espcacio local, crear cubo
    int p = 0;
    for(float x = -1 ; x <= 1 ; x += 0.25) {
        for(float y = -1 ; y <= 1 ; y += 0.25) {
            for(float z = -1 ; z <= 1 ; z += 0.25) {
                Vec3 punto = {{x, y, z}};
                cubo_puntos[p++] = punto;
            }
        }
    }
}

void render_frame() {
    for(int i = 0 ; i < array_size(estadosrender.figuras_buffer) ; i++) {
        draw_figura(&estadosrender.figuras_buffer[i]);
    }

    for(int i = 0 ; i < N_PUNTOS ; i++) {
        Vec2 punto = puntos_transformados[i];
        fill_cuadro(punto, 4, 4, 0xFF00FFFF, 0xFF00FFFF);
    }

    SDL_RenderPresent(estadosrender.renderer);
}