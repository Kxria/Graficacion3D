#pragma once

/*
Es el archivo interfaza
para mostrar en pantalla
*/

#include "global.h"
#include "draw/figuras.h"

#include <stdint.h>

void clear_color_buffer(void);
void clear_z_buffer(void);
void copy_buffer_to_texture(void );
void _Init(void);
void update(void);
void render_frame(void);
void render_input(void);
void transformar(void);
int back_face_culling(Vec3 camara, Vec4 *puntos);

Vec2* pivote_mas_cerca(Vec2 mp, Figuras *figs, float umbral);
void transformaciones(Vec3 escala, float radianesRotacionY, Vec3 traslado);

