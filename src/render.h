#pragma once

#include "global.h"
#include "draw/figuras.h"
#include <stdint.h>

void clear_color_buffer(void);
void copy_buffer_to_texture(void);
void _Init(void);
void render_frame(void);

void render_frame_menu(int puntos, int vertices, int relleno);

void update(void);
void render_input(void);

void transformar(void);

int back_face_culling(Vec3 camara, Vec3 *puntos);
int comparar(const void *a, const void *b);