#include "../libs/sokol/sokol_app.h"
#include "../libs/sokol/sokol_gfx.h"
#include "../libs/sokol/sokol_glue.h"

#include <stdlib.h>

#define MAX_SPRITES 10000

sg_pass_action pass_action;
sg_bindings bind;
sg_pipeline pip;

typedef struct vertex_desc_t {
  float pos_x;
  float pos_y;
  float pos_z;
  float clr_r;
  float clr_g;
  float clr_b;
  float clr_a;
} vertex_desc_t;
vertex_desc_t* vertices;
int vertices_count;

void draw_quad(float posx, float posy, float width, float height, float clr_r, float clr_g, float clr_b) {
  int i = vertices_count;
  vertices[i+0] = (vertex_desc_t){ posx,         posy,          0.0f, clr_r, clr_g, clr_b, 1.0f };
  vertices[i+1] = (vertex_desc_t){ posx + width, posy,          0.0f, clr_r, clr_g, clr_b, 1.0f };
  vertices[i+2] = (vertex_desc_t){ posx + width, posy - height, 0.0f, clr_r, clr_g, clr_b, 1.0f };
  vertices[i+3] = (vertex_desc_t){ posx,         posy - height, 0.0f, clr_r, clr_g, clr_b, 1.0f };
  vertices_count += 4;
}

void init(void) {
  sg_setup(&(sg_desc){
    .context = sapp_sgcontext()
  });
  pass_action = (sg_pass_action){
    .colors[0] = {.action = SG_ACTION_CLEAR, .val = {0.0f, 0.0f, 0.0f, 1.0f}}
  };

  sg_buffer_desc vbuf_desc = {
    .size = MAX_SPRITES * 4 * sizeof(vertex_desc_t),
    .usage = SG_USAGE_STREAM,
  };
  sg_buffer vbuf = sg_make_buffer(&vbuf_desc);

  uint16_t indices[] = {
    0, 1, 2, // first triangle
    0, 2, 3, // second triangle
  };
  sg_buffer_desc ibuf_desc = {
    .size = sizeof(indices),
    .type = SG_BUFFERTYPE_INDEXBUFFER,
    .content = indices,
  };
  sg_buffer ibuf = sg_make_buffer(&ibuf_desc);

  bind = (sg_bindings){
    .vertex_buffers[0] = vbuf,
    .index_buffer = ibuf,
  };

  sg_shader shd = sg_make_shader(&(sg_shader_desc){
    .vs.source =
      "#version 330\n"
      "layout(location=0) in vec4 position;\n"
      "layout(location=1) in vec4 color0;\n"
      "out vec4 color;\n"
      "void main() {\n"
      "  gl_Position = position;\n"
      "  color = color0;\n"
      "}\n",
    .fs.source =
      "#version 330\n"
      "in vec4 color;\n"
      "out vec4 frag_color;\n"
      "void main() {\n"
      "  frag_color = color;\n"
      "}\n"
  });
  pip = sg_make_pipeline(&(sg_pipeline_desc){
    .shader = shd,
    .index_type = SG_INDEXTYPE_UINT16,
    .layout = {
      .buffers[0].step_func = SG_VERTEXSTEP_PER_INSTANCE,
      .attrs = {
        [0] = {.offset = 0, .format = SG_VERTEXFORMAT_FLOAT3},
        [1] = {.offset = 12, .format = SG_VERTEXFORMAT_FLOAT4},
      }
    },
    .depth_stencil = {
      .depth_compare_func = SG_COMPAREFUNC_LESS_EQUAL,
      .depth_write_enabled = true,
    },
    .rasterizer = {
      .cull_mode = SG_CULLMODE_BACK,
    }
  });

  vertices_count = 0;
  vertices = (vertex_desc_t*)malloc(MAX_SPRITES * 4 * sizeof(vertex_desc_t));
}

void frame(void) {
  vertices_count = 0;
  draw_quad(0.5, 1.0, 0.5, 0.5, 1.0, 0.0, 1.0);
  draw_quad(-1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 0.0);
  int sprites = vertices_count / 4;
  sg_update_buffer(bind.vertex_buffers[0], vertices, vertices_count * sizeof(vertex_desc_t));

  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
  sg_apply_pipeline(pip);
  sg_apply_bindings(&bind);
  sg_draw(0, 6, sprites);
  sg_end_pass();
  sg_commit();
}

void cleanup(void) { 
  free(vertices);
  sg_shutdown();
}

sapp_desc sokol_main(int argc, char *argv[]) {
  return (sapp_desc){
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .width = 1024,
      .height = 600,
      .window_title = "ezy",
  };
}
