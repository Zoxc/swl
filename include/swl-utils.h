#pragma once
#include "swl.h"
#include "swl-features.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SWL_BACKEND_WINAPI
	#include "swl-winapi.h"
#endif

#ifdef SWL_BACKEND_XLIB
	#include "swl-xlib.h"
#endif

#ifdef SWL_CONTEXT_EGL
	#include "swl-egl.h"
#endif

#ifdef SWL_CONTEXT_WGL
	#include "swl-wgl.h"
#endif

SWL_API enum swl_result swl_platform_allocate(const char *title, unsigned int width, unsigned int height, swl_window_t *window, swl_display_t *display);
SWL_API void swl_platform_deallocate(void);

SWL_API enum swl_result swl_context_allocate(swl_window_t window, swl_display_t display);
SWL_API void swl_context_deallocate(void);

#ifdef __cplusplus
}
#endif
