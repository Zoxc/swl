#pragma once
#include "X11/Xlib.h"
#include "X11/Xutil.h"

/*
 * Render Frame Action definition
 */
typedef DWORD render_frame_action_t;

#define RFA_UP 0
#define RFA_DOWN 1
#define RFA_LEFT 2
#define RFA_RIGHT 3
#define RFA_GO 4

struct render_frame_platform
{
	Window x11_window;
	Display* x11_display;
	long x11_screen;
	XVisualInfo* x11_visual;
	Colormap x11_colormap;
};
