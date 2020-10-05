#define SOKOL_GLCORE33

#include "../renderer.h"
#include "../core.h"
#include "shader.glsl.h"

#include <stdio.h>
#include <stdlib.h>

void _sprite_renderer_update(sprite_renderer_desc *sprt, quad_desc *quads,
                             uint32_t quad_count);

sg_pipeline pip;

void renderer_init(void) {
  sg_shader shd = sg_make_shader(sprite_shader_desc());
  pip = sg_make_pipeline(&(sg_pipeline_desc){
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

void sprite_renderer_create(sprite_renderer_desc *sprt) {
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

  // texture_desc tex = sprt->texture;
  int imgW, imgH, nrChannels;
  int desired_channels = 4;
  // stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(sprt->texture.path, &imgW, &imgH, &nrChannels,
                                  desired_channels);
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
                                       .size = imgW * imgH * 4,
                                   }});

    stbi_image_free(data);
    sprt->texture.width = imgW;
    sprt->texture.height = imgH;
    sprt->texture.chanels = nrChannels;
  }

  sprt->vertices_count = 0;
  sprt->vertices =
      (vertex_desc_t *)malloc(sprt->max_quads * 4 * sizeof(vertex_desc_t));
  sprt->indices_count = 0;
  sprt->indices = (uint16_t *)malloc(sprt->max_quads * 6 * sizeof(uint16_t));
}

void sprite_renderer_destroy(sprite_renderer_desc *sprt) {
  free(sprt->vertices);
  free(sprt->indices);
}

void sprite_renderer_draw(sprite_renderer_desc *sprt, quad_desc *quads,
                          uint32_t quad_count) {
  _sprite_renderer_update(sprt, quads, quad_count);

  sg_update_buffer(sprt->bind.vertex_buffers[0], sprt->vertices,
                   sprt->vertices_count * sizeof(vertex_desc_t));
  sg_update_buffer(sprt->bind.index_buffer, sprt->indices,
                   sprt->indices_count * sizeof(uint16_t));

  sg_apply_pipeline(pip);
  sg_apply_bindings(&(sprt->bind));

  vs_params_t vs_params;
  vs_params.proj = HMM_Orthographic(0.0, (float)sapp_width(),
                                    (float)sapp_height(), 0.0, 0.0, 1.0);
  sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &vs_params,
                    sizeof(vs_params));

  sg_draw(0, sprt->indices_count, 1);
}

void _sprite_renderer_update(sprite_renderer_desc *sprt, quad_desc *quads,
                             uint32_t quad_count) {
  sprt->vertices_count = 0;
  sprt->indices_count = 0;
  if (quad_count > sprt->max_quads) {
    printf("Illegal Usage. Quad count(%d) higher than max quads(%d)\n",
           quad_count, sprt->max_quads);
    exit(EXIT_FAILURE);
    return;
  }

  for (int q = 0; q < quad_count; ++q) {
    int v = sprt->vertices_count;

    float p1x = quads[q].x;
    float p1y = quads[q].y;
    float p2x = quads[q].x + quads[q].w;
    float p2y = quads[q].y + quads[q].h;
    float r = quads[q].r;
    float g = quads[q].g;
    float b = quads[q].b;
    float a = quads[q].a;

    sprt->vertices[v + 0] = (vertex_desc_t){p1x, p1y, r, g, b, a, 0.0f, 0.0f};
    sprt->vertices[v + 1] = (vertex_desc_t){p2x, p1y, r, g, b, a, 1.0f, 0.0f};
    sprt->vertices[v + 2] = (vertex_desc_t){p2x, p2y, r, g, b, a, 1.0f, 1.0f};
    sprt->vertices[v + 3] = (vertex_desc_t){p1x, p2y, r, g, b, a, 0.0f, 1.0f};
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
}
