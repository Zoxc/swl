#pragma once
#include "swl-utils.h"

struct swl_data {
	const char *title;
	unsigned int width;
	unsigned int height;
	bool resizable;
	bool cursor;
	bool fullscreen;
	size_t opengl_version;
	size_t depth_size;
	size_t stencil_size;
	swl_window_t window;
	swl_display_t display;
};

extern struct swl_data swl;

enum swl_result swl_platform_update(enum swl_configurable field, size_t value);
