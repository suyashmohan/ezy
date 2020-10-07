#include "game.h"
#include "examples/ex1_stars.h"
#include "examples/ex2_stars2.h"

int example;

void start(void) {
  example = 1;
  ex1_start();
  ex2_start();
}

void frame(const sapp_event *e) {
  if (e != NULL && e->type == SAPP_EVENTTYPE_KEY_UP) {
    if (e->key_code == SAPP_KEYCODE_LEFT) {
      example = 1;
    } else if (e->key_code == SAPP_KEYCODE_RIGHT) {
      example = 2;
    } else if (e->key_code == SAPP_KEYCODE_DOWN) {
      example = 0;
    }
  }

  if (example == 1) {
    ex1_frame(e);
  } else if (example == 2) {
    ex2_frame(e);
  } else {
    ex1_frame(e);
    ex2_frame(e);
  }
}

void end(void) {
  ex1_end();
  ex2_end();
}
