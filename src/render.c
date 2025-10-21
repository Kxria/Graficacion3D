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
const int fovf = 630;

typedef enum _render_mesh_modes {
    RENDER_MESH_PUNTOS = 2<<0,
    RENDER_MESH_VERTICES = 2<<1,
    RENDER_MESH_RELLENO = 2<<2,
}RENDER_MESH_MODES;



int comparar(const void *a, const void *b) {
    Triangulo *A = (Triangulo*)a;
    Triangulo *B = (Triangulo*)b;

    return A->avg_z - B->avg_z;
}

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
            Vec3 vertices_transformados[3];

            for(int j = 0 ; j < 3 ; ++j) {
                Vec3 punto = cara_vertice[j];
        
                // matriz de transformacion
                Mat4 mt = mat4_eye();
                // printf("Matriz identidad\n");
        
                // escalamiento
                mat4_push_escala(&mt, estadosrender.meshes[m].escala);
                mat4_push_rotar(&mt, estadosrender.meshes[m].rotacion);
                mat4_push_traslado(&mt, estadosrender.meshes[m].traslado);

        
                Vec4 p = {{punto.unpack.x, punto.unpack.y, punto.unpack.z, 1.f}};
                p = mat4_dot_vec4(&mt, &p);
                punto = vec4_to_vec3(&p);
                vertices_transformados[j] = punto;
            }
            
            // Back-Face Culling
            if(!back_face_culling(camara, vertices_transformados)) {
                continue;
            }

            float avg_z = (vertices_transformados[0].unpack.z + 
                           vertices_transformados[1].unpack.z + 
                           vertices_transformados[1].unpack.z)/3.f;

            for(int j = 0 ; j < 3 ; ++j) {
                Vec4 pp = {{vertices_transformados[j].unpack.x, vertices_transformados[j].unpack.y, vertices_transformados[j].unpack.z, 1.f}};
                Mat4 PM = mat4_matriz_proyecion(fovf,estadosrender.w_height/(float)estadosrender.w_width,1.f,100.f);

                // printf("Matriz Proyeccion\n");
                // print_matriz(&PM);

                Vec4 punto_proyectado = proyeccion(&PM,pp);
                //escala ya que los puntos estan entre 0-1 
                punto_proyectado.unpack.y *= estadosrender.w_height/2.f;
                punto_proyectado.unpack.x *= estadosrender.w_width/2.f;
                //centrar
                punto_proyectado.unpack.y += estadosrender.w_height/2.f;
                punto_proyectado.unpack.x += estadosrender.w_width/2.f;
                //generar triangulo
                triangulo_proyectado.p[j].unpack.x = punto_proyectado.unpack.x;
                triangulo_proyectado.p[j].unpack.y = punto_proyectado.unpack.y;
                triangulo_proyectado.p[j].unpack.z = punto_proyectado.unpack.z;

                triangulo_proyectado.avg_z = avg_z;
            }
            // int dummy;
            // scanf("%d", &dummy);
            pushto_array(estadosrender.meshes[m].triangulos, triangulo_proyectado);
        }
        qsort(estadosrender.meshes[m].triangulos, 
              array_size(estadosrender.meshes[m].triangulos), 
              sizeof(estadosrender.meshes[m].triangulos[0]), 
              comparar);
    }
}

void update() {
    // modificar valores
    estadosrender.meshes[0].rotacion.unpack.x += 0.001f;
    estadosrender.meshes[0].rotacion.unpack.y += 0.001f;
    estadosrender.meshes[0].rotacion.unpack.z += 0.001f;
    transformar();

}


void render_input(void) {
    // int puntos;
    // int vertices;
    // int relleno;
    
    if(estadosrender.evento.type == SDL_EVENT_QUIT) {
        estadosrender.run = 0;
    }

    if(estadosrender.evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

    } else if(estadosrender.evento.type == SDL_EVENT_MOUSE_BUTTON_UP) {

    }

    // if(estadosrender.evento.type ==  SDL_EVENT_KEY_DOWN) {
    //     if(estadosrender.evento.key.scancode == SDL_SCANCODE_1) {
    //         // puntos = 1;
    //         // RENDER_MESH_MODES = 
    //         printf("Pintar puntos\n");
    //     } else if(estadosrender.evento.key.scancode == SDL_SCANCODE_2) {
    //         // vertices = 1;
    //         printf("Pintar vertices\n");
    //     } else if(estadosrender.evento.key.scancode == SDL_SCANCODE_3) {
    //         // relleno = 1;
    //         printf("Rellenar\n");
    //     }
    // }
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
    estadosrender.meshes[0].rotacion.unpack.x = 0.f;
    estadosrender.meshes[0].rotacion.unpack.y = 0.f;
    estadosrender.meshes[0].rotacion.unpack.z = 0.f;
    
    estadosrender.meshes[0].escala.unpack.x = 1.f;
    estadosrender.meshes[0].escala.unpack.y = 1.f;
    estadosrender.meshes[0].escala.unpack.z = 1.f;

    estadosrender.meshes[0].traslado.unpack.z = 5.f;
}

void render_frame_menu(int puntos, int vertices, int relleno) {
    // int puntos;
    // por cada mesh
    for(int m = 0 ; m < array_size(estadosrender.meshes) ; ++m) {
        // por cada triangulo
        int num_trian = array_size(estadosrender.meshes->triangulos);
        for(int i = 0 ; i < num_trian ; i++) {
            // vertices triangulos
            Triangulo trian = estadosrender.meshes[m].triangulos[i];
            if(puntos) {
                fill_cuadro(trian.p[0], 4, 4, 0xFF00FFFF, 0xFF00FFFF);
                fill_cuadro(trian.p[1], 4, 4, 0xFF00FFFF, 0xFF00FFFF);
                fill_cuadro(trian.p[2], 4, 4, 0xFF00FFFF, 0xFF00FFFF);
            }

            // lineas de los triangulos
            if(vertices) {
                draw_trian(trian.p[0].unpack.x, trian.p[0].unpack.y, 
                           trian.p[1].unpack.x, trian.p[1].unpack.y, 
                           trian.p[2].unpack.x, trian.p[2].unpack.y, 0x00FFFFFF);
            }

            if(relleno) {
                printf("Aqui se supone que se rellena el triangulo\n");
            }
        }
    }
    SDL_RenderPresent(estadosrender.renderer);
}

void render_frame(void) {
    //por cada mesh
    for(int m = 0 ; m < array_size(estadosrender.meshes) ; ++m) {
        //por cada triangulo
        int num_trian = array_size(estadosrender.meshes[m].triangulos);
        for(int i = 0 ; i < num_trian ; i++) {
            //vertices de los triángulos
            Triangulo trian = estadosrender.meshes[m].triangulos[i];//cubo_triangulos[i];
            
            //painter algorithm
                                                                    //
            //fill_cuadro(trian.p[0],4, 4, 0xff00ffff,0xff00ffff);
            //fill_cuadro(trian.p[1],4, 4, 0xff00ffff,0xff00ffff);
            //fill_cuadro(trian.p[2],4, 4, 0xff00ffff,0xff00ffff);
            //lineas de los triángulos
            // fill_trian(trian.p[0],trian.p[1],trian.p[2],0xff00ffff,0xff00ffff);
            
            draw_trian( trian.p[0].unpack.x,trian.p[0].unpack.y,
                        trian.p[1].unpack.x,trian.p[1].unpack.y,
                        trian.p[2].unpack.x,trian.p[2].unpack.y,
                        0x00FFFFFF);
        }

    }

    SDL_RenderPresent(estadosrender.renderer);
}

int back_face_culling(Vec3 camara, Vec3* puntos) {
    Vec3 BA = resta_vec3(puntos[1], puntos[0]);
    normalizar_vec3_inplace(&BA);

    Vec3 CA = resta_vec3(puntos[2], puntos[0]);
    normalizar_vec3_inplace(&CA);

    Vec3 N = cross_vec3(BA, CA);
    normalizar_vec3_inplace(&N);

    Vec3 Ray = resta_vec3(camara, puntos[0]);
    normalizar_vec3_inplace(&Ray);
    
    return dot_vec3(N, Ray) > 0;
}
