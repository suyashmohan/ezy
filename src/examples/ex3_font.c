#include "ex3_font.h"
#include "../core/fontsprite.h"
#include "../core/renderer.h"
#include "../core/resources.h"

#define MAX_RECTS 10000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

font_texture font;
batchrenderer renderer3;
int fps = 0;
uint64_t last_time = 0;
double elapsed_time = 0.0;
int frame_count = 0;

void ex3_start(void) {
  font = load_font("./assets/monogram_extended.ttf", 48.0f);
  renderer3 = batchrenderer_create((batchrenderer_desc){
      .max_quads = MAX_RECTS,
      .tex = font.tex,
  });
  free(font.tex.bitmap);

  stm_setup();
}

void ex3_frame(const sapp_event *e, int rects) {
  const double delta = stm_ms(stm_laptime(&last_time));
  elapsed_time += delta;
  if (elapsed_time > 1000.0) {
    elapsed_time = 0.0;
    fps = frame_count;
    frame_count = 0;
  } else {
    frame_count += 1;
  }

  char fps_str[10];
  char rects_str[10];
  sprintf(fps_str, "FPS: %d", fps);
  sprintf(rects_str, "%d", rects);

  fontsprite_draw(font, &renderer3, "Hello World", 50.0, 100.0f, 1.0f);
  fontsprite_draw(font, &renderer3, fps_str, 0.0, 0.0f, 1.0f);
  fontsprite_draw(font, &renderer3, rects_str, 0.0, 48.0f, 1.0f);

  batchrenderer_commit(&renderer3);
}

void ex3_end(void) { batchrenderer_destroy(renderer3); }
