#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "core.h"
#include <stdint.h>

// TIPS: Change this number as per your game requirements
#define MAX_QUADS 10000
#define FILEPATH_SIZE 256

typedef struct vertex_desc_t {
  float pos_x;
  float pos_y;
  float clr_r;
  float clr_g;
  float clr_b;
  float clr_a;
  float tex_u;
  float tex_v;
} vertex_desc_t;

typedef struct texture_desc {
  char path[FILEPATH_SIZE];
  int width;
  int height;
  int chanels;
} texture_desc;

typedef struct sprite_renderer_desc {
  texture_desc texture;
  uint32_t max_quads;
  // For internal Use
  sg_bindings bind;
  vertex_desc_t *vertices;
  uint16_t *indices;
  int vertices_count;
  int indices_count;
} sprite_renderer_desc;

typedef struct quad_desc {
  float x;
  float y;
  float w;
  float h;
  float r;
  float g;
  float b;
  float a;
} quad_desc;

// renderer_* functions are meant to be used internally
void renderer_init(void);

// load a texture
void sprite_renderer_create(sprite_renderer_desc *sprt);
void sprite_renderer_destroy(sprite_renderer_desc *sprt);
void sprite_renderer_draw(sprite_renderer_desc *sprt, quad_desc *quads,
                          uint32_t quad_count);

#endif
