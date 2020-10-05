#ifndef __GAME_H__
#define __GAME_H__

#include "core/core.h"

// called on every frame. Handle input and render here
void frame(const sapp_event *e);

// called at the start of game. Use to initialise resources
void start(void);

// called at the end of game. Use to free resources
void end(void);

#endif
