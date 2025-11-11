#include "draw.h"
#include "linea.h"
#include "../draw/figuras.h"

void draw_trian(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color){
    draw_linea(x0, y0, x1, y1, color);
    draw_linea(x1, y1, x2, y2, color);
    draw_linea(x2, y2, x0, y0, color);
}

Vec3 normal_triangulo(Triangulo *trian){
	Vec3 AB = resta_vec3(trian -> pos[1], trian -> pos[0]);
	Vec3 AC = resta_vec3(trian -> pos[2], trian -> pos[0]);

	normalizar_vec3_inplace(&AB);
	normalizar_vec3_inplace(&AC);

	Vec3 normal = cross_vec3(AB, AC);
	normalizar_vec3_inplace(&normal);

	trian -> normal = normal;
	return normal;
}
