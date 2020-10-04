#include "game.h"
#include "core/renderer.h"

#define MAX_RECTS 10000
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int rect_count = 0;
struct rectangle recs[MAX_RECTS];
int did_init = 0;
uint64_t last_time = 0;
double timer = 0.0;

void init(void) {
  did_init = 1;
  srand(time(NULL));
  stm_setup();
  load_texture("assets/flare.png");
}

void update(void) {
  for (int i = 0; i < rect_count; i++) {
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
  if (timer >= 1000.0) {
    printf("FPS: %f\n", 1000.0 / laptime);
    timer = 0.0;
  }
}

void draw(void) {
  for (int i = 0; i < rect_count; i++) {
    draw_quad(recs[i].x, recs[i].y, recs[i].w, recs[i].h, recs[i].r, recs[i].g,
              recs[i].b);
  }
}

void event(const sapp_event *e) {
  if (e->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
    if (rect_count < MAX_RECTS) {
      recs[rect_count].x = e->mouse_x;
      recs[rect_count].y = 720 - e->mouse_y;
      recs[rect_count].w = 50.0;
      recs[rect_count].h = 50.0;
      recs[rect_count].r = (float)rand() / (float)(RAND_MAX);
      recs[rect_count].g = (float)rand() / (float)(RAND_MAX);
      recs[rect_count].b = (float)rand() / (float)(RAND_MAX);
      recs[rect_count].dx = (float)rand() / (float)(RAND_MAX)-0.5f;
      recs[rect_count].dy = (float)rand() / (float)(RAND_MAX)-0.5f;
      rect_count += 1;
    }
  }
}

void frame(const sapp_event *e) {
  if (did_init == 0) {
    init();
  }
  if (e != NULL) {
    event(e);
  }
  update();
  draw();
}
