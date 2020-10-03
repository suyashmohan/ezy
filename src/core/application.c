#include "core.h"
#include "renderer.h"
#include "../game.h"
#include <stdio.h>

sg_pass_action pass_action;
const sapp_event *last_event;

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
  frame(last_event);
  renderer_draw();
  sg_end_pass();
  sg_commit();
}

void _cleanup(void) {
  renderer_shutdown();
  sg_shutdown();
}

void _event(const sapp_event *e) {
  last_event = e;
}

sapp_desc sokol_main(int argc, char *argv[]) {
  return (sapp_desc){
      .init_cb = _init,
      .frame_cb = _frame,
      .cleanup_cb = _cleanup,
      .event_cb = _event,
      .width = 1280,
      .height = 720,
      .window_title = "ezy",
  };
}
