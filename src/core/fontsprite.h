#ifndef __FONTSPRITE_H__
#define __FONTSPRITE_H__

#include "renderer.h"

void fontsprite_draw(font_texture font, batchrenderer *renderer, char str[],
                     float x, float y, float scale);

#endif
