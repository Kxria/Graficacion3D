#include "global.h"

EstadoRender estadosrender = {
    .renderer = 0,
    .textura = 0,
    .evento = {0},
    .color_buffer = 0,
    .z_buffer = 0,
    .clear_color = {0x808080FF},
    .w_width = 1080,
    .w_height = 720,
    .run = 1,
	.meshes = 0
};
