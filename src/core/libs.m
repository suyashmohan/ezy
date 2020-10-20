#ifdef __APPLE__

// Import definitions of libraries on MacOS

#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define HANDMADE_MATH_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION
#define DR_MP3_IMPLEMENTATION
#include "core.h"

#endif
