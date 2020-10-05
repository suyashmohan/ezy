#include "ex1_stars.h"
#include "../core/renderer.h"

#define MAX_RECTS 10000
#include <stdio.h>
#include <stdlib.h>

struct velocity {
  float dx;
  float dy;
} velocity;

int rect_count = 0;
quad_desc recs[MAX_RECTS];
struct velocity recs_dxy[MAX_RECTS];
sprite_renderer_desc sprt;

void event(const sapp_event *e);
void update(void);
void draw(void);

void ex1_start(void) {
  printf("Starting Game\n");
  sprt = (sprite_renderer_desc){.max_quads = MAX_RECTS,
                                .texture = {.path = "assets/flare.png"}};
  sprite_renderer_create(&sprt);
}

void ex1_frame(const sapp_event *e) {
  if (e != NULL) {
    event(e);
  }
  update();
  draw();
}

void ex1_end(void) {
  printf("Ending Game\n");
  sprite_renderer_destroy(&sprt);
}

void event(const sapp_event *e) {
  if (e->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
    if (rect_count < MAX_RECTS) {
      float w = sprt.texture.width / 2.0;
      float h = sprt.texture.height / 2.0;

      recs[rect_count].x = e->mouse_x - (w / 2.0);
      recs[rect_count].y = e->mouse_y - (h / 2.0);
      recs[rect_count].w = w;
      recs[rect_count].h = h;
      recs[rect_count].r = (float)rand() / (float)(RAND_MAX);
      recs[rect_count].g = (float)rand() / (float)(RAND_MAX);
      recs[rect_count].b = (float)rand() / (float)(RAND_MAX);
      recs[rect_count].a = 1.0f;
      recs_dxy[rect_count].dx = (float)rand() / (float)(RAND_MAX)-0.5f;
      recs_dxy[rect_count].dy = (float)rand() / (float)(RAND_MAX)-0.5f;
      rect_count += 1;
    }
  }
}

void update(void) {
  for (int i = 0; i < rect_count; i++) {
    recs[i].x += recs_dxy[i].dx;
    recs[i].y += recs_dxy[i].dy;
    if (recs[i].x < 0 || recs[i].x + recs[i].w > 1280.0) {
      recs_dxy[i].dx *= -1;
    }
    if (recs[i].y < recs[i].h || recs[i].y > 720.0) {
      recs_dxy[i].dy *= -1;
    }
  }
}

void draw(void) { sprite_renderer_draw(&sprt, recs, rect_count); }
