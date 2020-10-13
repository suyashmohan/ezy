#include "ex2_stars2.h"
#include "../core/renderer.h"
#include "../core/resources.h"

#define MAX_RECTS 10000
#include <stdio.h>
#include <stdlib.h>

struct velocity2 {
  float dx;
  float dy;
} velocity2;

int rect_count2 = 0;
int rect_add_count2 = 10;
quad_desc recs2[MAX_RECTS];
struct velocity2 recs_dxy2[MAX_RECTS];
batchrenderer renderer2;

void event2(const sapp_event *e);
void update2(void);
void draw2(void);

void ex2_start(void) {
  texture tex = load_image("./assets/texture.png");
  renderer2 = batchrenderer_create((batchrenderer_desc){
      .max_quads = MAX_RECTS,
      .tex = tex,
  });
  // free(bitmap);
}

int ex2_frame(const sapp_event *e) {
  if (e != NULL) {
    event2(e);
  }
  update2();
  draw2();
  return rect_count2;
}

void ex2_end(void) { batchrenderer_destroy(renderer2); }

void event2(const sapp_event *e) {
  if (e->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
    if (rect_count2 < MAX_RECTS) {
      float w = 48.0f;
      float h = 48.0f;

      for (int i = 0; i < rect_add_count2; ++i) {
        recs2[rect_count2] = (quad_desc){
            .x = e->mouse_x - (w / 2.0f),
            .y = e->mouse_y - (h / 2.0f),
            .w = w,
            .h = h,
            .src_y = (float)(rand() % 19) * 16.0f,
            .src_w = 16.0f,
            .src_h = 16.0f,
        };

        recs_dxy2[rect_count2] =
            (struct velocity2){
                .dx = (float)rand() / (float)(RAND_MAX)-0.5f,
                .dy = (float)rand() / (float)(RAND_MAX)-0.5f,
            },

        rect_count2 += 1;
      }
    }
  }
}

void update2(void) {
  for (int i = 0; i < rect_count2; i++) {
    recs2[i].x += recs_dxy2[i].dx;
    recs2[i].y += recs_dxy2[i].dy;
    if (recs2[i].x < 0 || recs2[i].x + recs2[i].w > 1280.0) {
      recs_dxy2[i].dx *= -1;
    }
    if (recs2[i].y < 0 || recs2[i].y + recs2[i].h > 720.0) {
      recs_dxy2[i].dy *= -1;
    }
  }
}

void draw2(void) {
  for (int i = 0; i < rect_count2; i++) {
    batchrenderer_draw(&renderer2, recs2[i]);
  }
  batchrenderer_commit(&renderer2);
}
