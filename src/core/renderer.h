#ifndef __RENDERER_H__
#define __RENDERER_H__

// TIPS: Change this number as per your game requirements
#define MAX_SPRITES 10000

typedef struct texture {
    uint32_t tex_id;
    int width;
    int height;
    int chanels;
} texture;

// renderer_* functions are meant to be used internally
void renderer_init(void);
void renderer_shutdown(void);
void renderer_draw_setup(void);
void renderer_draw(void);

// function to draw a colored rectangle
void draw_quad(float posx, float posy, float width, float height, float clr_r,
               float clr_g, float clr_b);

// load a texture
texture load_texture(const char *file);

#endif
