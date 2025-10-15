#include "render.h"
#include "color/colores.h"
#include "draw/draw.h"
#include "draw/linea.h"
#include "memoria/memoria.h"
#include "math/lerp.h"
#include "math/vectores.h"
#include "math/matrix.h"
#include "draw/figuras.h"

#include <math.h>
#include <stdio.h>

Vec3 camara;
const float fovf = 630;

void transformar(void) {
    for(int m = 0 ; m < array_size(estadosrender.meshes) ; ++m) {
        free_array(estadosrender.meshes[m].triangulos);
        estadosrender.meshes[m].triangulos = 0;
        // por cada cara del buffer
        int num_caras = array_size(estadosrender.meshes[m].indices);
        for(int i = 0 ; i < num_caras ; i++) {
            Cara_t punto_cara = estadosrender.meshes[m].indices[i];
            
            Vec3 cara_vertice[3];
            cara_vertice[0] = estadosrender.meshes[m].vertices[punto_cara.a - 1];
            cara_vertice[1] = estadosrender.meshes[m].vertices[punto_cara.b - 1];
            cara_vertice[2] = estadosrender.meshes[m].vertices[punto_cara.c - 1];
            
            // por cada vertice
            Triangulo triangulo_proyectado;
            for(int j = 0 ; j < 3 ; ++j) {
                Vec3 punto = cara_vertice[j];
        
                // matriz de transformacion
                Mat4 mt = mat4_eye();
        
                // escalamiento
                mat4_push_escala(&mt, estadosrender.meshes[m].escala);
                mat4_push_rotar(&mt, estadosrender.meshes[m].rotacion);
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
            pushto_array(estadosrender.meshes[m].triangulos, triangulo_proyectado);
        }
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
    // modificar valores
    estadosrender.meshes[0].rotacion.unpack.x += 0.00f;
    estadosrender.meshes[0].rotacion.unpack.y += 0.001f;
    estadosrender.meshes[0].rotacion.unpack.z += 0.00f;
    transformar();

}


void render_input(void) {
    if(estadosrender.evento.type == SDL_EVENT_QUIT) {
        estadosrender.run = 0;
    }
}

void clear_color_buffer() {
    for(int y = 0; y < estadosrender.w_height; ++y) {
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
    camara.unpack.z = -5.f;

    // cargar mesh (MODIFICAR DEPENDIENDO DEL OBJ)
    Mesh cubo = loadMesh("assets/cube.obj", VERTICES | INDICES);

    pushto_array(estadosrender.meshes, cubo);

    estadosrender.meshes[0].rotacion.unpack.x = 0.01f;
    estadosrender.meshes[0].rotacion.unpack.y = 0.01f;
    estadosrender.meshes[0].rotacion.unpack.z = 0.01f;

    estadosrender.meshes[0].escala.unpack.x = 1.f;
    estadosrender.meshes[0].escala.unpack.y = 1.f;
    estadosrender.meshes[0].escala.unpack.z = 1.f;
}

void render_frame() {
    // por cada mesh
    for(int m = 0 ; m < array_size(estadosrender.meshes) ; ++m) {
        // por cada triangulo
        int num_trian = array_size(estadosrender.meshes->triangulos);
        for(int i = 0 ; i < num_trian ; i++) {
            // vertices triangulos
            Triangulo trian = estadosrender.meshes[m].triangulos[i];
            fill_cuadro(trian.p[0], 4, 4, 0xFF00FFFF, 0xFF00FFFF);
            fill_cuadro(trian.p[1], 4, 4, 0xFF00FFFF, 0xFF00FFFF);
            fill_cuadro(trian.p[2], 4, 4, 0xFF00FFFF, 0xFF00FFFF);

            // lineas de los triangulos
            draw_trian(trian.p[0].unpack.x, trian.p[0].unpack.y, 
                    trian.p[1].unpack.x, trian.p[1].unpack.y, 
                    trian.p[2].unpack.x, trian.p[2].unpack.y, 0xFF00FFFF);
        }
    }
    SDL_RenderPresent(estadosrender.renderer);
}