#include "game.h"
#include "examples/ex1_stars.h"
#include "examples/ex2_stars2.h"

#define EXAMPLE 2

void start(void) {
  switch (EXAMPLE) {
  case 1:
    ex1_start();
    break;
  case 2:
    ex2_start();
    break;
  default:
    break;
  }
}

void frame(const sapp_event *e) {
  switch (EXAMPLE) {
  case 1:
    ex1_frame(e);
    break;
  case 2:
    ex2_frame(e);
    break;
  default:
    break;
  }
}

void end(void) {
  switch (EXAMPLE) {
  case 1:
    ex1_end();
    break;
  case 2:
    ex2_end();
    break;
  default:
    break;
  }
}
