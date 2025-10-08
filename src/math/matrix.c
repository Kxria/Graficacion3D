#include "matrix.h"

Mat4 mat4_eye(void) {
    Mat4 identidad;
    identidad.data[0] = 1;
    identidad.data[5] = 1;
    identidad.data[10] = 1;
    identidad.data[15] = 1;
    return identidad;
}

void mat4_push_escala(Mat4 *matriz, Vec3 escala) {
    matriz->data[0] = escala.unpack.x;
    matriz->data[5] = escala.unpack.y;
    matriz->data[10] = escala.unpack.z;
}

void mat4_push_traslado(Mat4 *matriz, Vec3 pos) {
    matriz->data[3] = pos.unpack.x;
    matriz->data[7] = pos.unpack.y;
    matriz->data[11] = pos.unpack.z;
}

/////////////////////// ROTACIONES ///////////////////////////

void mat4_push_rotar(Mat4 *matriz, Vec3 rotacion) {
    float cosa = cos(rotacion.unpack.z), sina = sin(rotacion.unpack.z);
    float cosb = cos(rotacion.unpack.y), sinb = sin(rotacion.unpack.y);
    float coss = cos(rotacion.unpack.x), sins = sin(rotacion.unpack.x);

    matriz->data[0] *= cosa * cosb;
    matriz->data[1]  = cosa * sinb * sins - sina * coss;
    matriz->data[2]  = cosa * sinb * coss + sina * sins;
    matriz->data[4]  = sina * cosb;
    matriz->data[5] *= sina *sinb * sins + cosa * coss;
    matriz->data[6]  = sina * sinb * coss - cosa * sins;
    matriz->data[8]  = -sinb;
    matriz->data[9]  = cosb * sins;
    matriz->data[10] *= cosb * coss;
}


void mat4_push_rotar_x(Mat4 *matriz, float radianes) {
    matriz->data[0] = cos(radianes);
    matriz->data[2] = sin(radianes);
    matriz->data[8] = -sin(radianes);
    matriz->data[10] = cos(radianes);
}

void mat4_push_rotar_y(Mat4 *matriz, float radianes) {
    matriz->data[0] = cos(radianes);
    matriz->data[1] = -sin(radianes);
    matriz->data[4] = sin(radianes);
    matriz->data[5] = cos(radianes);
}

void mat4_push_rotar_z(Mat4 *matriz, float radianes) {
    matriz->data[5] = cos(radianes);
    matriz->data[6] = -sin(radianes);
    matriz->data[9] = sin(radianes);
    matriz->data[10] = cos(radianes);
}

Vec4 mat4_dot_vec4(Mat4 *matriz, Vec4 *punto) {
    Vec4 c; 

    // [a1, b1, c1, d1]   [x]   [(a1 * x) + (d1 * y) + (c1 * z) + (d1 * 1)]
    // [a2, b2, c2, d2] . [y] = [(a2 * x) + (d2 * y) + (c2 * z) + (d2 * 1)]
    // [a3, b3, c3, d3]   [z]   [(a3 * x) + (d3 * y) + (c3 * z) + (d3 * 1)]
    // [a4, b4, c4, d4]   [1]   [(a4 * x) + (d4 * y) + (c4 * z) + (d4 * 1)]

    c.unpack.x = matriz -> data[0] * punto -> unpack.x +
                 matriz -> data[1] * punto -> unpack.y +
                 matriz -> data[2] * punto -> unpack.z +
                 matriz -> data[3] * punto -> unpack.w;

    c.unpack.y = matriz -> data[4] * punto -> unpack.x +
                 matriz -> data[5] * punto -> unpack.y +
                 matriz -> data[6] * punto -> unpack.z +
                 matriz -> data[7] * punto -> unpack.w;

    c.unpack.z = matriz -> data[8] * punto -> unpack.x +
                 matriz -> data[9] * punto -> unpack.y +
                 matriz -> data[10] * punto -> unpack.z +
                 matriz -> data[11] * punto -> unpack.w;

    c.unpack.w = matriz -> data[12] * punto -> unpack.x +
                 matriz -> data[13] * punto -> unpack.y +
                 matriz -> data[14] * punto -> unpack.z +
                 matriz -> data[15] * punto -> unpack.w;
    return c;
}

/////////////////////// PROYECCIONES ///////////////////////////

Vec2 proyeccion_ortografica(Vec3 punto, float fovf) {
    Vec2 p = {{ punto.unpack.x * fovf, punto.unpack.y * fovf }};

    return p;
}

Vec2 proyeccion_perspectiva(Vec3 punto, float fovf) {
    Vec2 p = {{ fovf * punto.unpack.x/punto.unpack.z, 
                fovf * punto.unpack.y/punto.unpack.z }};

    return p;

}