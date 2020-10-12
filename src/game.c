#include "game.h"
#include "examples/ex1_stars.h"
#include "examples/ex2_stars2.h"
#include "examples/ex3_font.h"

int example;

void start(void) {
  printf("Starting Game\n");
  example = 1;
  ex1_start();
  ex2_start();
  ex3_start();
}

void frame(const sapp_event *e) {
  if (e != NULL && e->type == SAPP_EVENTTYPE_KEY_UP) {
    if (e->key_code == SAPP_KEYCODE_1) {
      example = 1;
    } else if (e->key_code == SAPP_KEYCODE_2) {
      example = 2;
    } else if (e->key_code == SAPP_KEYCODE_3) {
      example = 3;
    } else if (e->key_code == SAPP_KEYCODE_0) {
      example = 0;
    }
  }

  if (example == 1) {
    ex1_frame(e);
  } else if (example == 2) {
    ex2_frame(e);
  } else if (example == 3) {
    ex3_frame(e, 0);
  } else {
    int r1 = ex1_frame(e);
    int r2 = ex2_frame(e);
    ex3_frame(e, r1 + r2);
  }
}

void end(void) {
  printf("Ending Game\n");
  ex1_end();
  ex2_end();
  ex3_end();
}
