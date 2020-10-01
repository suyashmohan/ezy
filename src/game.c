#include "../libs/sokol/sokol_time.h"
#include "game.h"
#include "core/renderer.h"

#include <stdio.h>

struct rectangle {
  float x;
  float y;
  float w;
  float h;
  float r;
  float g;
  float b;
  float dx;
  float dy;
} rectangle;

struct rectangle recs[] = {
    [0] = {0.0, 50.0, 50.0, 50.0, 1.0, 0.0, 0.0, 1.0, 1.0},
    [1] = {100.0, 150.0, 50.0, 50.0, 0.0, 1.0, 0.0, 1.0, -1.0},
    [2] = {200.0, 250.0, 50.0, 50.0, 0.0, 0.0, 1.0, -1.0, 1.0},
    [3] = {300.0, 350.0, 50.0, 50.0, 0.0, 1.0, 1.0, -1.0, -1.0},
};

int did_init = 0;
uint64_t last_time = 0;
double timer = 0.0;

void init(void) {
  did_init = 1;
  stm_setup();
}

void update(void) {
  for (int i = 0; i < 4; i++) {
    recs[i].x += recs[i].dx;
    recs[i].y += recs[i].dy;
    if (recs[i].x < 0 || recs[i].x + recs[i].w > 1280.0) {
      recs[i].dx *= -1;
    }
    if (recs[i].y < recs[i].h || recs[i].y > 720.0) {
      recs[i].dy *= -1;
    }
  }

  double laptime = stm_ms(stm_laptime(&last_time));
  timer += laptime;
  if(timer >= 1000.0) {
    printf("FPS: %f\n", 1000.0 / laptime);
    timer = 0.0;
  }
}

void draw() {
  for (int i = 0; i < 4; i++) {
    draw_quad(recs[i].x, recs[i].y, recs[i].w, recs[i].h, recs[i].r, recs[i].g,
              recs[i].b);
  }
}

void frame() {
  if (did_init == 0) {
    init();
  }
  update();
  draw();
}
