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

#ifdef SWL_OPENGL_ES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

enum swl_result
{
	SWLR_OK,
	SWLR_ERROR_GLEW_ERROR
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

enum swl_result SWL_API swl_init(const char *title, unsigned int width, unsigned int height, bool resizable);
void SWL_API swl_quit(void);
void SWL_API swl_swap(void);
bool SWL_API swl_query(struct swl_event *event);
bool SWL_API swl_wait(struct swl_event *event);

#ifdef __cplusplus
}
#endif
