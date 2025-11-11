#include "lerp.h"
#include  "vectores.h"
#include "../draw/draw.h"
#include <stdint.h>

//  Interpolacion lineal
Vec2 lerpv2(Vec2 p1, Vec2 p2, float t){
    Vec2 vector;

    vector.unpack.x = p1.unpack.x + (p2.unpack.x - p1.unpack.x) * t;
    vector.unpack.y = p1.unpack.y + (p2.unpack.y - p1.unpack.y) * t;

    return vector;
}

//  Interpolacion cuadratica
Vec2 lerp2v2(Vec2 p1, Vec2 p2, Vec2 p3, float t){
    return lerpv2(lerpv2(p1, p2, t), lerpv2(p2, p3, t), t);
}

void draw_curva(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color){
    Vec2 ctrlV;

    for(float t = 0; t <= 1; t += 0.001){
        ctrlV = lerp2v2(p1, p2, p3, t);
        draw_pixel(ctrlV.unpack.x, ctrlV.unpack.y, color);
    }
}


//              1
//            1   1
//         1    2   1
//       1   3    3   1
//     1   4    6   4   1