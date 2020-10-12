#include "fontsprite.h"
#include <string.h>

void fontsprite_draw(font_texture font, batchrenderer *renderer, char str[],
                     float x, float y, float scale) {
  size_t len = strlen(str);
  float dx = 0;
  int i;
  for (i = 0; i < len; ++i) {
    char c = str[i];
    int c_idx = c - 32;
    int cx = font.word_x[c_idx];
    int cx1 = font.word_x[c_idx + 1];
    int cw = cx1 - cx;
    quad_desc quad = {
        .x = x + dx,
        .y = y,
        .w = (float)cw * scale,
        .h = font.line_height * scale,
        .src_x = (float)cx,
        .src_w = (float)cw,
        .src_h = font.line_height,
    };
    dx += quad.w;
    batchrenderer_draw(renderer, quad);
  }
}
