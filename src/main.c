#include "../libs/sokol/sokol_app.h"
#include "../libs/sokol/sokol_gfx.h"
#include "../libs/sokol/sokol_glue.h"

sg_pass_action pass_action;
sg_bindings bind;
sg_pipeline pip;

void init(void) {
  sg_setup(&(sg_desc){
    .context = sapp_sgcontext()
  });
  pass_action = (sg_pass_action){
    .colors[0] = {
      .action = SG_ACTION_CLEAR,
      .val = {1.0f, 0.0f, 0.0f, 1.0f}
    }
  };

  float vertices[] = {
    // positions            colors
    -0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, // point 1
     0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // point 2
     0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // point 3
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, // point 4
  };
  uint16_t indices[] = {
    0, 1, 2, // first triangle
    0, 2, 3, // second triangle
  };

  sg_buffer_desc vbuf_desc = {
    .size = sizeof(vertices),
    .content = vertices,
  };
  sg_buffer vbuf = sg_make_buffer(&vbuf_desc);

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
      .attrs = {
        [0] = {.offset = 0, .format = SG_VERTEXFORMAT_FLOAT3},
        [1] = {.offset = 12, .format = SG_VERTEXFORMAT_FLOAT4},
      }
    }
  });
}

void frame(void) {
  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());

  sg_apply_pipeline(pip);
  sg_apply_bindings(&bind);
  sg_draw(0, 6, 1);

  sg_end_pass();
  sg_commit();
}

void cleanup(void) { sg_shutdown(); }

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
