#include "../../libs/sokol/sokol_app.h"
#include "../../libs/sokol/sokol_gfx.h"
#include "../../libs/sokol/sokol_glue.h"
#include "../game.h"
#include "renderer.h"

sg_pass_action pass_action;

void _init(void) {
  sg_setup(&(sg_desc){.context = sapp_sgcontext()});
  pass_action =
      (sg_pass_action){.colors[0] = {.action = SG_ACTION_CLEAR,
                                     .val = {0.0f, 0.0f, 0.0f, 1.0f}}};
  renderer_init();
}

void _frame(void) {
  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
  renderer_draw_setup();
  frame();
  renderer_draw();
  sg_end_pass();
  sg_commit();
}

void _cleanup(void) {
  renderer_shutdown();
  sg_shutdown();
}

sapp_desc sokol_main(int argc, char *argv[]) {
  return (sapp_desc){
      .init_cb = _init,
      .frame_cb = _frame,
      .cleanup_cb = _cleanup,
      .width = 1280,
      .height = 720,
      .window_title = "ezy",
  };
}
