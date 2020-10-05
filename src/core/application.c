#include "../game.h"
#include "core.h"
#include "renderer.h"
#include <stdio.h>

sg_pass_action pass_action;
const sapp_event *last_event;

void app_init(void) {
  sg_setup(&(sg_desc){.context = sapp_sgcontext()});
  pass_action =
      (sg_pass_action){.colors[0] = {.action = SG_ACTION_CLEAR,
                                     .val = {0.0f, 0.0f, 0.0f, 1.0f}}};
  renderer_init();
  start();
}

void app_frame(void) {
  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
  frame(last_event);
  sg_end_pass();
  sg_commit();
  last_event = NULL;
}

void app_cleanup(void) { 
  end();
  sg_shutdown();
}

void app_event(const sapp_event *e) { last_event = e; }

sapp_desc sokol_main(int argc, char *argv[]) {
  return (sapp_desc){
      .init_cb = app_init,
      .frame_cb = app_frame,
      .cleanup_cb = app_cleanup,
      .event_cb = app_event,
      .width = 1280,
      .height = 720,
      .window_title = "ezy",
      .sample_count = 4,
  };
}
