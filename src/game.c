#include "game.h"
#include  "examples/ex1_stars.h"

void start(void) {
  ex1_start();
}

void frame(const sapp_event *e) {
  ex1_frame(e);
}

void end(void) {
  ex1_end();
}
