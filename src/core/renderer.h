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
  uint16_t width;
  uint16_t height;
  uint16_t chanels;
} texture_desc;

typedef struct spritebatch_desc {
  texture_desc texture;
  uint32_t max_quads;
  // For internal Use
  sg_bindings bind;
  vertex_desc_t *vertices;
  uint16_t *indices;
  uint32_t vertices_count;
  uint32_t indices_count;
} spritebatch_desc;

typedef enum quad_type {
  EZY_QUAD_TEXTURE = 0,
  EZY_QUAD_TEXTURE_COLOR,
  EZY_QUAD_COLOR,
} quad_type;

typedef struct quad_desc {
  float x;
  float y;
  float w;
  float h;
  float r;
  float g;
  float b;
  float a;
  float src_x;
  float src_y;
  float src_w;
  float src_h;
  quad_type type;
} quad_desc;

// renderer_* functions are meant to be used internally
void renderer_init(void);

// load a texture
void spritebatch_create(spritebatch_desc *sprt);
void spritebatch_destroy(spritebatch_desc *sprt);
void spritebatch_draw(spritebatch_desc *sprt, quad_desc quads);
void spritebatch_commit(spritebatch_desc *sprt);

#endif
