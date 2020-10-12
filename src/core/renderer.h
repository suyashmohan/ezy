#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "core.h"
#include <stdint.h>

// TIPS: Change this number as per your game requirements
#define MAX_QUADS 10000
#define FILEPATH_SIZE 256

typedef struct vertex_desc {
  float pos_x;
  float pos_y;
  float clr_r;
  float clr_g;
  float clr_b;
  float clr_a;
  float tex_u;
  float tex_v;
} vertex_desc;

typedef struct texture {
  int width;
  int height;
  int channels;
  unsigned char *bitmap;
} texture;

typedef struct font_texture {
  float line_height;
  texture tex;
  uint16_t word_x[96];
  char word[96];
} font_texture;

typedef struct batchrenderer_desc {
  texture tex;
  uint16_t max_quads;
} batchrenderer_desc;

typedef struct batchrenderer {
  sg_bindings bind;
  vertex_desc *vertices;
  uint16_t *indices;
  uint32_t vertices_count;
  uint32_t indices_count;
  uint8_t vertices_per_quad;
  uint8_t indices_per_quad;
  uint32_t max_quads;
  texture tex;
} batchrenderer;

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

// batchrenderer functions
batchrenderer batchrenderer_create(batchrenderer_desc br_desc);
void batchrenderer_destroy(batchrenderer br);
void batchrenderer_draw(batchrenderer *br, quad_desc quad);
void batchrenderer_commit(batchrenderer *br);

#endif
