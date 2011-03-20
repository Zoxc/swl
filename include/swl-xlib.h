#pragma once
#include "X11/Xlib.h"
#include "X11/keysym.h"

typedef Window swl_window_t;
typedef Display *swl_display_t;

#define SWLR_ERROR_BACKEND_XLIB_DISPLAY SWLR_BACKEND(0)
#define SWLR_ERROR_BACKEND_XLIB_VISUAL SWLR_BACKEND(1)

