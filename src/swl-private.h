#pragma once
#include "swl-utils.h"

struct swl_data {
	const char *title;
	unsigned int width;
	unsigned int height;
	bool resizable;
	bool fullscreen;
	swl_window_t window;
	swl_display_t display;
};

extern struct swl_data swl;
