#ifndef __GAME_H__
#define __GAME_H__

/*
On every frame, `frame` function will be called.
This function must be implemented and this is where game logic goes.
`frame` will get reference to events passed from sokol library.
*/

#include "core/core.h"

void frame(const sapp_event *e);

#endif
