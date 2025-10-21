#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "global.h"
#include "render.h"
#include "draw/linea.h"
#include "memoria/memoria.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000/FPS)
int frame_tiempo_prev = 0;
// int puntos = 0;
// int vertices = 0;
// int relleno = 0;

void render_init(void);
void render_clean(void);
void grabar_imagen(void);

#define SDL_WINDOW_OPENGL   SDL_UINT64_C(0x0000000000000002)

SDL_Window *window = NULL;

int main(int argc, char** argv) {
    render_init();
    _Init();

    while(estadosrender.run) {
        int delay_frame = FRAME_TARGET_TIME -(SDL_GetTicks() - frame_tiempo_prev);
        
        if(delay_frame > 0 && delay_frame <= FRAME_TARGET_TIME) {
            SDL_Delay(delay_frame);
        }

        while(SDL_PollEvent(&estadosrender.evento)) {            
            render_input();
			
            // if(estadosrender.evento.type == SDL_EVENT_KEY_DOWN) {
            //     if(estadosrender.evento.key.scancode == SDL_SCANCODE_1){
            //         grabar = 1;
            //     }
            // }

        //     if(estadosrender.evento.type ==  SDL_EVENT_KEY_DOWN) {
        //         if(estadosrender.evento.key.scancode == SDL_SCANCODE_1) {
        //             puntos = 1;
        //             printf("Pintar puntos\n");
        //         } else if(estadosrender.evento.key.scancode == SDL_SCANCODE_2) {
        //             vertices = 1;
        //             printf("Pintar vertices\n");
        //         } else if(estadosrender.evento.key.scancode == SDL_SCANCODE_3) {
        //             relleno = 1;
        //             printf("Rellenar\n");
        //         }
        //     }
        }
        
        update();
        
        
        // if(grabar) {
        //     grabar_imagen();
        //     printf("Se grabo la imagen\n");
        //     grabar = 0;
        // }
        
        copy_buffer_to_texture();
        clear_color_buffer();
        render_frame();
        // if(puntos) {
        //     render_frame_menu(puntos, vertices, relleno);
        //     puntos = 0;
        // }
        // if(vertices) {
        //     render_frame_menu(puntos, vertices, relleno);
        //     vertices = 0;
        // }
        // if(relleno) {
        //     render_frame_menu(puntos, vertices, relleno);
        //     relleno = 0;
        // }
    }

    return 0;
}

void render_init(void) {
    SDL_CreateWindowAndRenderer("Window", 
                                estadosrender.w_width, 
                                estadosrender.w_height, 
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, 
                                &window, 
                                &estadosrender.renderer);

    assert(window && "No se creó la ventana...\n");
    assert(estadosrender.renderer && "No se creó el render...\n");

    estadosrender.color_buffer = malloc(estadosrender.w_width * estadosrender.w_height * sizeof(uint32_t));
    assert(estadosrender.color_buffer && "No se creó el color buffer...\n");

    estadosrender.texture = SDL_CreateTexture(estadosrender.renderer, 
                                              SDL_PIXELFORMAT_RGBA8888, 
                                              SDL_TEXTUREACCESS_STREAMING, 
                                              estadosrender.w_width, 
                                              estadosrender.w_height);
    assert(estadosrender.texture && "No se creó la textura...\n");
}

void render_clean(void) {
    // free(estadosrender.figuras_buffer);
    free_array(estadosrender.meshes);
    SDL_DestroyTexture(estadosrender.texture);
    SDL_DestroyRenderer(estadosrender.renderer);
    SDL_DestroyWindow(window);
    free(estadosrender.color_buffer);
}

void grabar_imagen(void) {
    FILE *file = fopen("imagen.ppm", "w");

    fprintf(file, "%s\n", "P3");
    fprintf(file, "%d %d\n", estadosrender.w_width, estadosrender.w_height);
    fprintf(file, "%d\n", 255);

    for(int i = 0; i < estadosrender.w_height * estadosrender.w_width; ++i) {
        Color pixel = {estadosrender.color_buffer[i]};
        fprintf(file, "%d %d %d\n", pixel.rgba.r, pixel.rgba.g, pixel.rgba.b);
    }

    fclose(file);
}