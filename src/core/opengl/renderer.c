#define SOKOL_GLCORE33

#include "../renderer.h"
#include "../core.h"
#include "shader.glsl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct { sg_pipeline pip; } state;

void renderer_init(void) {
  sg_shader shd = sg_make_shader(sprite_shader_desc());
  state.pip = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = shd,
      .index_type = SG_INDEXTYPE_UINT16,
      .layout =
          {.attrs =
               {
                   [ATTR_vs_position] = {.offset = 0,
                                         .format = SG_VERTEXFORMAT_FLOAT2},
                   [ATTR_vs_color0] = {.offset = 2 * sizeof(float),
                                       .format = SG_VERTEXFORMAT_FLOAT4},
                   [ATTR_vs_texcoord0] = {.offset = 6 * sizeof(float),
                                          .format = SG_VERTEXFORMAT_FLOAT2},
               }},
      .blend = {.enabled = true,
                .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
                .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA},
  });
}

void spritebatch_create(spritebatch_desc *sprt) {
  if (sprt->max_quads == 0) {
    sprt->max_quads = MAX_QUADS;
  }

  sg_buffer_desc vbuf_desc = {
      .size = sprt->max_quads * 4 * sizeof(vertex_desc_t),
      .usage = SG_USAGE_STREAM,
  };
  sg_buffer vbuf = sg_make_buffer(&vbuf_desc);

  sg_buffer_desc ibuf_desc = {
      .size = sprt->max_quads * 6 * sizeof(uint16_t),
      .type = SG_BUFFERTYPE_INDEXBUFFER,
      .usage = SG_USAGE_STREAM,
  };
  sg_buffer ibuf = sg_make_buffer(&ibuf_desc);

  sprt->bind = (sg_bindings){
      .vertex_buffers[0] = vbuf,
      .index_buffer = ibuf,
      .fs_images[SLOT_tex] = sg_alloc_image(),
  };

  if (sprt->texture.type == EZY_TEXTURE_SPRITESHEET) {
    int imgW, imgH, nrChannels;
    int desired_channels = 4;
    // stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(sprt->texture.path, &imgW, &imgH,
                                    &nrChannels, desired_channels);
    if (data) {
      printf("Image Loaded : %s - %dx%d [%d bits]\n", sprt->texture.path, imgW,
             imgH, nrChannels * 8);

      sg_init_image(sprt->bind.fs_images[SLOT_tex],
                    &(sg_image_desc){.width = imgW,
                                     .height = imgH,
                                     .pixel_format = SG_PIXELFORMAT_RGBA8,
                                     .min_filter = SG_FILTER_LINEAR,
                                     .mag_filter = SG_FILTER_LINEAR,
                                     .content.subimage[0][0] = {
                                         .ptr = data,
                                         .size = imgW * imgH * nrChannels,
                                     }});

      stbi_image_free(data);
      sprt->texture.width = imgW;
      sprt->texture.height = imgH;
      sprt->texture.chanels = nrChannels;
    } else {
      printf("Unable to load texture: %s\n", sprt->texture.path);
      exit(EXIT_FAILURE);
      return;
    }
  } else if (sprt->texture.type == EZY_TEXTURE_FONT) {
    int imgW, imgH;
    imgW = sprt->texture.font.width;
    imgH = sprt->texture.font.height;

    uint32_t sz = imgW * imgH;
    unsigned char *bitmap2 = malloc(sz * 4);
    for (unsigned int i = 0; i < sz; ++i) {
      bitmap2[i * 4 + 0] = 255;
      bitmap2[i * 4 + 1] = 255;
      bitmap2[i * 4 + 2] = 255;
      bitmap2[i * 4 + 3] = sprt->texture.font.bitmap[i];
    }

    printf("Font texture generated - %dx%d\n", imgW, imgH);

    sg_init_image(sprt->bind.fs_images[SLOT_tex],
                  &(sg_image_desc){.width = imgW,
                                   .height = imgH,
                                   .pixel_format = SG_PIXELFORMAT_RGBA8,
                                   .min_filter = SG_FILTER_LINEAR,
                                   .mag_filter = SG_FILTER_LINEAR,
                                   .content.subimage[0][0] = {
                                       .ptr = bitmap2,
                                       .size = sz * 4,
                                   }});

    sprt->texture.width = imgW;
    sprt->texture.height = imgH;
    sprt->texture.chanels = 1;
    free(bitmap2);
    free(sprt->texture.font.bitmap);
  }

  sprt->vertices_count = 0;
  sprt->vertices =
      (vertex_desc_t *)malloc(sprt->max_quads * 4 * sizeof(vertex_desc_t));
  sprt->indices_count = 0;
  sprt->indices = (uint16_t *)malloc(sprt->max_quads * 6 * sizeof(uint16_t));
}

void spritebatch_destroy(spritebatch_desc *sprt) {
  free(sprt->vertices);
  free(sprt->indices);
}

void spritebatch_draw(spritebatch_desc *sprt, quad_desc quad) {
  if (sprt->vertices_count > sprt->max_quads * 4) {
    printf(
        "Illegal Usage. Vertices Count(%d) higher than reserved vertices(%d)\n",
        sprt->vertices_count, sprt->max_quads * 4);
    exit(EXIT_FAILURE);
    return;
  }

  int v = sprt->vertices_count;

  float p1x = quad.x;
  float p1y = quad.y;
  float p2x = quad.x + quad.w;
  float p2y = quad.y + quad.h;

  float r, g, b, a;
  r = g = b = a = 1.0f;
  if (quad.type == EZY_QUAD_TEXTURE_COLOR || quad.type == EZY_QUAD_COLOR) {
    r = quad.r;
    g = quad.g;
    b = quad.b;
    a = quad.a;
  }

  float tex_x, tex_y, tex_w, tex_h;
  tex_x = tex_y = 0.0f;
  tex_w = tex_h = 1.0f;
  if (quad.type == EZY_QUAD_TEXTURE || quad.type == EZY_QUAD_TEXTURE_COLOR) {
    if (quad.src_w != 0 && quad.src_h != 0) {
      tex_x = quad.src_x / sprt->texture.width;
      tex_y = quad.src_y / sprt->texture.height;
      tex_w = (quad.src_x + quad.src_w) / sprt->texture.width;
      tex_h = (quad.src_y + quad.src_h) / sprt->texture.height;
    }
  }

  sprt->vertices[v + 0] = (vertex_desc_t){p1x, p1y, r, g, b, a, tex_x, tex_y};
  sprt->vertices[v + 1] = (vertex_desc_t){p2x, p1y, r, g, b, a, tex_w, tex_y};
  sprt->vertices[v + 2] = (vertex_desc_t){p2x, p2y, r, g, b, a, tex_w, tex_h};
  sprt->vertices[v + 3] = (vertex_desc_t){p1x, p2y, r, g, b, a, tex_x, tex_h};
  sprt->vertices_count += 4;

  int i = sprt->indices_count;
  sprt->indices[i + 0] = v + 0;
  sprt->indices[i + 1] = v + 1;
  sprt->indices[i + 2] = v + 2;
  sprt->indices[i + 3] = v + 0;
  sprt->indices[i + 4] = v + 2;
  sprt->indices[i + 5] = v + 3;
  sprt->indices_count += 6;
}

void spritebatch_commit(spritebatch_desc *sprt) {
  if (sprt->vertices_count > 0) {
    sg_update_buffer(sprt->bind.vertex_buffers[0], sprt->vertices,
                     sprt->vertices_count * sizeof(vertex_desc_t));
    sg_update_buffer(sprt->bind.index_buffer, sprt->indices,
                     sprt->indices_count * sizeof(uint16_t));

    sg_apply_pipeline(state.pip);
    sg_apply_bindings(&(sprt->bind));

    vs_params_t vs_params;
    vs_params.proj = HMM_Orthographic(0.0, (float)sapp_width(),
                                      (float)sapp_height(), 0.0, 0.0, 1.0);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &vs_params,
                      sizeof(vs_params));

    sg_draw(0, sprt->indices_count, 1);
    sprt->vertices_count = 0;
    sprt->indices_count = 0;
  }
}

font_texture spritefont_create(font_texture_desc desc) {
  font_texture font = {
      .bitmap = NULL,
  };

  /* load font file */
  long size;
  unsigned char *fontBuffer;

  FILE *fontFile = fopen(desc.filepath, "rb");
  if (!fontFile) {
    printf("Unable to load font file: %s\n", desc.filepath);
    exit(EXIT_FAILURE);
    return font;
  }
  fseek(fontFile, 0, SEEK_END);
  size = ftell(fontFile);       /* how long is the file ? */
  fseek(fontFile, 0, SEEK_SET); /* reset */

  fontBuffer = malloc(size);

  fread(fontBuffer, size, 1, fontFile);
  fclose(fontFile);

  /* prepare font */
  stbtt_fontinfo info;
  if (!stbtt_InitFont(&info, fontBuffer, 0)) {
    printf("Unable to init font: %s\n", desc.filepath);
    exit(EXIT_FAILURE);
    return font;
  }

  char a;
  for (a = 32; a < 127; ++a) {
    font.word[a - 32] = a;
  }
  font.word[a - 32] = '\0';

  font.line_height = desc.line_height;
  int l_h = desc.line_height; /* line height */
  /* calculate font scaling */
  float scale = stbtt_ScaleForPixelHeight(&info, (float)l_h);
  int ascent, descent, lineGap;
  stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
  ascent = (int)roundf(ascent * scale);
  descent = (int)roundf(descent * scale);

  int x = 0;
  int i;

  // Calculate texture width
  for (i = 0; i < strlen(font.word); ++i) {
    /* how wide is this character */
    int ax;
    int lsb;
    stbtt_GetCodepointHMetrics(&info, font.word[i], &ax, &lsb);

    /* advance x */
    x += (int)roundf(ax * scale);

    /* add kerning */
    int kern;
    kern = stbtt_GetCodepointKernAdvance(&info, font.word[i], font.word[i + 1]);
    x += (int)roundf(kern * scale);
  }

  int b_w = x;   /* bitmap width */
  int b_h = l_h; /* bitmap height */
  font.width = b_w;
  font.height = b_h;

  /* create a bitmap for the phrase */
  font.bitmap = calloc(b_w * b_h, sizeof(unsigned char));

  x = 0;
  font.word_x[0] = x;
  for (i = 0; i < strlen(font.word); ++i) {
    /* how wide is this character */
    int ax;
    int lsb;
    stbtt_GetCodepointHMetrics(&info, font.word[i], &ax, &lsb);

    /* get bounding box for character (may be offset to account for chars that
     * dip above or below the line */
    int c_x1, c_y1, c_x2, c_y2;
    stbtt_GetCodepointBitmapBox(&info, font.word[i], scale, scale, &c_x1, &c_y1,
                                &c_x2, &c_y2);

    /* compute y (different characters have different heights */
    int y = ascent + c_y1;

    /* render character (stride and offset is important here) */
    int byteOffset = x + (int)roundf(lsb * scale) + (y * b_w);
    stbtt_MakeCodepointBitmap(&info, font.bitmap + byteOffset, c_x2 - c_x1,
                              c_y2 - c_y1, b_w, scale, scale, font.word[i]);

    /* advance x */
    x += (int)roundf(ax * scale);

    /* add kerning */
    int kern;
    kern = stbtt_GetCodepointKernAdvance(&info, font.word[i], font.word[i + 1]);
    x += (int)roundf(kern * scale);
    font.word_x[i + 1] = x;
  }

  free(fontBuffer);

  return font;
}

void spritefont_destroy(font_texture *font) { free(font->bitmap); }

void spritefont_draw(spritebatch_desc *sprt, char str[], float x, float y,
                     float scale) {
  size_t len = strlen(str);
  float dx = 0;
  int i;
  for (i = 0; i < len; ++i) {
    char c = str[i];
    int c_idx = c - 32;
    int cx = sprt->texture.font.word_x[c_idx];
    int cx1 = sprt->texture.font.word_x[c_idx + 1];
    int cw = cx1 - cx;
    quad_desc quad = {
        .x = x + dx,
        .y = y,
        .w = (float)cw * scale,
        .h = sprt->texture.font.line_height * scale,
        .src_x = (float)cx,
        .src_w = (float)cw,
        .src_h = (float)sprt->texture.font.line_height,
    };
    dx += quad.w;
    spritebatch_draw(sprt, quad);
  }
}
