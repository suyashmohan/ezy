#define SOKOL_GLCORE33

#include "../renderer.h"
#include "../core.h"
#include "shader.glsl.h"

#include <stdio.h>
#include <stdlib.h>

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
                .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
      },
      .depth_stencil = {
                .depth_compare_func = SG_COMPAREFUNC_LESS_EQUAL,
                .depth_write_enabled = true,
      },
      .rasterizer = {
          .cull_mode = SG_CULLMODE_BACK,
      },
  });
}

batchrenderer batchrenderer_create(batchrenderer_desc br_desc) {
  batchrenderer br = {
    .vertices_per_quad = 4,
    .indices_per_quad = 6,
  };

  if (br_desc.max_quads == 0) {
    br_desc.max_quads = MAX_QUADS;
  }
  br.max_quads = br_desc.max_quads;
  br.tex = br_desc.tex;

  sg_buffer_desc vbuf_desc = {
      .size = br.max_quads * br.vertices_per_quad * sizeof(vertex_desc),
      .usage = SG_USAGE_STREAM,
  };
  sg_buffer vbuf = sg_make_buffer(&vbuf_desc);

  sg_buffer_desc ibuf_desc = {
      .size = br.max_quads * br.indices_per_quad * sizeof(uint16_t),
      .type = SG_BUFFERTYPE_INDEXBUFFER,
      .usage = SG_USAGE_STREAM,
  };
  sg_buffer ibuf = sg_make_buffer(&ibuf_desc);

  br.bind = (sg_bindings){
      .vertex_buffers[0] = vbuf,
      .index_buffer = ibuf,
      .fs_images[SLOT_tex] = sg_alloc_image(),
  };

  sg_init_image(br.bind.fs_images[SLOT_tex],
                  &(sg_image_desc){.width = br_desc.tex.width,
                                   .height = br_desc.tex.height,
                                   .pixel_format = SG_PIXELFORMAT_RGBA8,
                                   .min_filter = SG_FILTER_LINEAR,
                                   .mag_filter = SG_FILTER_LINEAR,
                                   .content.subimage[0][0] = {
                                     .ptr = br_desc.tex.bitmap,
                                       .size = br_desc.tex.width * br_desc.tex.height * br_desc.tex.channels,
                                   }});

  br.vertices_count = 0;
  br.vertices =
      (vertex_desc *)malloc(br.max_quads * br.vertices_per_quad * sizeof(vertex_desc));
  br.indices_count = 0;
  br.indices = (uint16_t *)malloc(br.max_quads * br.indices_per_quad * sizeof(uint16_t));

  return br;
}

void batchrenderer_destroy(batchrenderer br) {
  if(br.vertices != NULL) {
    free(br.vertices);
  }
  if(br.indices != NULL) {
    free(br.indices);
  }
}

void batchrenderer_draw(batchrenderer *br, quad_desc quad) {
  if (br->vertices_count > br->max_quads * br->vertices_per_quad) {
    printf(
        "Illegal Usage. Vertices Count(%d) higher than reserved vertices(%d)\n",
        br->vertices_count, br->max_quads * br->vertices_per_quad);
    exit(EXIT_FAILURE);
    return;
  }

  int v = br->vertices_count;

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
      tex_x = quad.src_x / br->tex.width;
      tex_y = quad.src_y / br->tex.height;
      tex_w = (quad.src_x + quad.src_w) / br->tex.width;
      tex_h = (quad.src_y + quad.src_h) / br->tex.height;
    }
  }

  br->vertices[v + 0] = (vertex_desc){p1x, p1y, r, g, b, a, tex_x, tex_y};
  br->vertices[v + 1] = (vertex_desc){p2x, p1y, r, g, b, a, tex_w, tex_y};
  br->vertices[v + 2] = (vertex_desc){p2x, p2y, r, g, b, a, tex_w, tex_h};
  br->vertices[v + 3] = (vertex_desc){p1x, p2y, r, g, b, a, tex_x, tex_h};
  br->vertices_count += 4;

  int i = br->indices_count;
  br->indices[i + 0] = v + 0;
  br->indices[i + 1] = v + 1;
  br->indices[i + 2] = v + 2;
  br->indices[i + 3] = v + 0;
  br->indices[i + 4] = v + 2;
  br->indices[i + 5] = v + 3;
  br->indices_count += 6;
}

void batchrenderer_commit(batchrenderer *br) {
  if (br->vertices_count > 0) {
    sg_update_buffer(br->bind.vertex_buffers[0], br->vertices,
                     br->vertices_count * sizeof(vertex_desc));
    sg_update_buffer(br->bind.index_buffer, br->indices,
                     br->indices_count * sizeof(uint16_t));

    sg_apply_pipeline(state.pip);
    sg_apply_bindings(&(br->bind));

    vs_params_t vs_params;
    vs_params.proj = HMM_Orthographic(0.0, (float)sapp_width(),
                                      (float)sapp_height(), 0.0, 0.0, 1.0);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &vs_params,
                      sizeof(vs_params));

    sg_draw(0, br->indices_count, 1);
    br->vertices_count = 0;
    br->indices_count = 0;
  }
}

