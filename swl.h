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

#ifdef SWL_BACKEND_WINAPI
	#include "swl-winapi.h"
#endif

#ifdef SWL_BACKEND_XLIB
	#include "swl-xlib.h"
#endif

enum swl_result
{
	SWLR_OK,
	SWLR_ERROR_DISPLAY_CREATION,
	SWLR_ERROR_SURFACE_CREATION,
	SWLR_ERROR_CONTEXT_CREATION,
	SWLR_ERROR_EGL_CREATION,
	SWLR_ERROR_EGL_CONFIG,
	SWLR_ERROR_EGL_ERROR
};

#define SWLR_PLATFORM(index) ((enum swl_result)(1000 + index))

enum swl_event_type
{
	SWLE_KEYUP,
	SWLE_KEYDOWN,
	SWLE_MOUSEUP,
	SWLE_MOUSEDOWN,
	SWLE_MOUSEMOVE,
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

struct swl_event
{
	enum swl_event_type type;
	union
	{
		struct swl_key_event key_event;
		struct swl_mouse_event mouse_event;
	};
};

enum swl_result SWL_API swl_init(const char *title, unsigned int width, unsigned int height);
void SWL_API swl_quit(void);
void SWL_API swl_swap(void);
bool SWL_API swl_query(struct swl_event *event);
bool SWL_API swl_wait(struct swl_event *event);

#ifdef __cplusplus
}
#endif
