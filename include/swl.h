#pragma once
#ifdef __cplusplus
extern "C" {
#else
#ifdef _MSC_VER
	typedef unsigned char bool;
	#define true 1
	#define false 0
#else
	#include <stdbool.h>
#endif
#endif

#include "swl-features.h"

#ifdef _MSC_VER
	#ifdef SWL_BUILD_DLL
		#define SWL_API __declspec(dllexport)
	#else
		#ifdef SWL_STATIC
			#define SWL_API
		#else
			#define SWL_API __declspec(dllimport)
		#endif
	#endif
#else
	#define SWL_API
#endif

#define SWLR_BACKEND(index) ((enum swl_result)(1000 + index))
#define SWLR_CONTEXT(index) ((enum swl_result)(2000 + index))

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

enum swl_result
{
	SWLR_OK
};

enum swl_event_type
{
	SWLE_KEYUP,
	SWLE_KEYDOWN,
	SWLE_MOUSEUP,
	SWLE_MOUSEDOWN,
	SWLE_MOUSEMOVE,
	SWLE_RESIZE,
	SWLE_QUIT
};

struct swl_key_event
{
	swl_key_t key;
};

struct swl_mouse_event
{
	int x;
	int y;
};

struct swl_size_event
{
	int width;
	int height;
};

struct swl_event
{
	enum swl_event_type type;
	union
	{
		struct swl_key_event key_event;
		struct swl_mouse_event mouse_event;
		struct swl_size_event size_event;
	};
};

enum swl_result swl_platform_allocate(const char *title, unsigned int width, unsigned int height, bool resizable, swl_window_t *window, swl_display_t *display);
void swl_platform_deallocate(void);

enum swl_result swl_context_allocate(swl_window_t window, swl_display_t display);
void swl_context_deallocate(void);

enum swl_result SWL_API swl_init(const char *title, unsigned int width, unsigned int height, bool resizable);
void SWL_API swl_quit(void);
void SWL_API swl_swap(void);
bool SWL_API swl_query(struct swl_event *event);
bool SWL_API swl_wait(struct swl_event *event);

#ifdef __cplusplus
}
#endif
