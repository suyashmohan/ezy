#define SOKOL_GLCORE33

#include "../renderer.h"
#include "../../../libs/Handmade-Math/HandmadeMath.h"
#include "../../../libs/sokol/sokol_app.h"
#include "../../../libs/sokol/sokol_gfx.h"
#include "../../../libs/sokol/sokol_glue.h"
#include "shader.glsl.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct vertex_desc_t {
  float pos_x;
  float pos_y;
  float clr_r;
  float clr_g;
  float clr_b;
  float clr_a;
} vertex_desc_t;

sg_bindings bind;
sg_pipeline pip;
vertex_desc_t *vertices;
uint16_t *indices;
int vertices_count;
int indices_count;

void renderer_init(void) {
  sg_buffer_desc vbuf_desc = {
      .size = MAX_SPRITES * 4 * sizeof(vertex_desc_t),
      .usage = SG_USAGE_STREAM,
  };
  sg_buffer vbuf = sg_make_buffer(&vbuf_desc);

  sg_buffer_desc ibuf_desc = {
      .size = MAX_SPRITES * sizeof(indices),
      .type = SG_BUFFERTYPE_INDEXBUFFER,
      .usage = SG_USAGE_STREAM,
  };
  sg_buffer ibuf = sg_make_buffer(&ibuf_desc);

  bind = (sg_bindings){
      .vertex_buffers[0] = vbuf,
      .index_buffer = ibuf,
  };

  sg_shader shd = sg_make_shader(sprite_shader_desc());
  pip = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = shd,
      .index_type = SG_INDEXTYPE_UINT16,
      .layout = {.attrs =
                     {
                         [ATTR_vs_position] = {.offset = 0,
                                               .format =
                                                   SG_VERTEXFORMAT_FLOAT2},
                         [ATTR_vs_color0] = {.offset = 2 * sizeof(float),
                                             .format = SG_VERTEXFORMAT_FLOAT4},
                     }},
      .blend = {.enabled = true,
                .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
                .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA},
  });

  vertices_count = 0;
  vertices = (vertex_desc_t *)malloc(MAX_SPRITES * 4 * sizeof(vertex_desc_t));
  indices_count = 0;
  indices = (uint16_t *)malloc(MAX_SPRITES * 6 * sizeof(uint16_t));
}

void renderer_shutdown(void) {
  free(vertices);
  free(indices);
}

void renderer_draw_setup(void) {
  vertices_count = 0;
  indices_count = 0;
}

void renderer_draw(void) {
  sg_update_buffer(bind.vertex_buffers[0], vertices,
                   vertices_count * sizeof(vertex_desc_t));
  sg_update_buffer(bind.index_buffer, indices,
                   indices_count * sizeof(uint16_t));

  sg_apply_pipeline(pip);
  sg_apply_bindings(&bind);

  vs_params_t vs_params;
  vs_params.proj = HMM_Orthographic(0.0, (float)sapp_width(), 0.0,
                                    (float)sapp_height(), 0.0, 1.0);
  sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &vs_params,
                    sizeof(vs_params));

  sg_draw(0, indices_count, 1);
}

void draw_quad(float posx, float posy, float width, float height, float clr_r,
               float clr_g, float clr_b) {
  if (vertices_count >= MAX_SPRITES * 4) {
    printf("Unable to allocate memory. Vertices Count:%d\n", vertices_count);
    exit(EXIT_FAILURE);
  }
  int v = vertices_count;
  vertices[v + 0] = (vertex_desc_t){posx, posy, clr_r, clr_g, clr_b, 1.5f};
  vertices[v + 1] =
      (vertex_desc_t){posx + width, posy, clr_r, clr_g, clr_b, 1.5f};
  vertices[v + 2] =
      (vertex_desc_t){posx + width, posy - height, clr_r, clr_g, clr_b, 1.5f};
  vertices[v + 3] =
      (vertex_desc_t){posx, posy - height, clr_r, clr_g, clr_b, 1.5f};
  vertices_count += 4;

  int i = indices_count;
  indices[i + 0] = v + 0;
  indices[i + 1] = v + 1;
  indices[i + 2] = v + 2;
  indices[i + 3] = v + 0;
  indices[i + 4] = v + 2;
  indices[i + 5] = v + 3;
  indices_count += 6;
}