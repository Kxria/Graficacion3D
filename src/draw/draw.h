#pragma once

#include "../math/vectores.h"
#include "linea.h"
#include "figuras.h"

#include <stdint.h>

void draw_pixel(int x, int y, uint32_t color);
void draw_trian(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_cuadrado(int x0, int y0, int largo, int altura, uint32_t color);
void draw_circuloTrig(int radio , int x0, int y0, int numvertices, uint32_t color);
void draw_circuloPM(int radio , int x0, int y0, uint32_t color);
void draw_linea(int x0, int y0, int x1, int y1, uint32_t color);

// Curvas
void draw_linea_lerp(Vec2 p1, Vec2 p2, uint32_t color);
void draw_curva(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color);

// Relleno triangulo
void fill_triangulo(Triangulo *triangulo, uint32_t color);
void fill_figura(void *figura, uint32_t fill_color);
void fill_flood(Vec2 *pInicial, uint32_t fill_color);
void fill_cuadro(Vec3 pos, int w, int h, uint32_t color, uint32_t contorno);

uint32_t getColor(Vec2 *pixel);
