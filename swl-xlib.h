#pragma once
#include "X11/Xlib.h"
#include "X11/keysym.h"

/*
 * Key type definition
 */
typedef KeySym swl_key_t;

#define SWLK_UP XK_Up
#define SWLK_DOWN XK_Down
#define SWLK_LEFT XK_Left
#define SWLK_RIGHT XK_Right
#define SWLK_GO XK_Return

#define SWLR_ERROR_PLATFORM_XLIB_DISPLAY SWLR_PLATFORM(0)
#define SWLR_ERROR_PLATFORM_XLIB_VISUAL SWLR_PLATFORM(1)

