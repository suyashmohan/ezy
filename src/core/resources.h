#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "renderer.h"

texture load_image(const char file[]);
font_texture load_font(const char file[], float line_height);

#endif 