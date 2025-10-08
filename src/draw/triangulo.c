#include "draw.h"
#include "linea.h"

void draw_trian(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color){
    draw_linea(x0, y0, x1, y1, color);
    draw_linea(x1, y1, x2, y2, color);
    draw_linea(x2, y2, x0, y0, color);
}
