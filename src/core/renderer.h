#ifndef __RENDERER_H__
#define __RENDERER_H__

#define MAX_SPRITES 10000

void renderer_init(void);
void renderer_shutdown(void);
void renderer_draw_setup(void);
void renderer_draw(void);

void draw_quad(float posx, float posy, float width, float height, float clr_r,
               float clr_g, float clr_b);

#endif
