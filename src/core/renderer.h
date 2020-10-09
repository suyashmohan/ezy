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

typedef struct font_texture_desc {
  const char *filepath;
  int line_height;
} font_texture_desc;

typedef struct font_texture {
  unsigned char *bitmap;
  int width;
  int height;
  int line_height;
  int word_x[96];
  char word[96];
} font_texture;

typedef enum texture_type {
  TEXTURE_SPRITESHEET = 0,
  TEXTURE_FONT,
} texture_type;

typedef struct texture_desc {
  texture_type type;
  char path[FILEPATH_SIZE];
  font_texture font;
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

// spritebatch functions
void spritebatch_create(spritebatch_desc *sprt);
void spritebatch_destroy(spritebatch_desc *sprt);
void spritebatch_draw(spritebatch_desc *sprt, quad_desc quads);
void spritebatch_commit(spritebatch_desc *sprt);

// font functions
font_texture spritefont_create(font_texture_desc desc);
void spritefont_destroy(font_texture *font);
void spritefont_draw(spritebatch_desc *sprt, char str[], float x, float y,
                     float scale);

#endif
