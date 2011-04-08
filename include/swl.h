#pragma once
#include <stddef.h>

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

enum swl_result
{
	SWLR_OK,
	SWLR_UNSUPPORTED
};

enum swl_event_type
{
	SWLE_KEYUP,
	SWLE_KEYDOWN,
	SWLE_MOUSEUP,
	SWLE_MOUSEDOWN,
	SWLE_MOUSEMOVE,
	SWLE_MOUSERAW,
	SWLE_RESIZE,
	SWLE_FOCUS,
	SWLE_QUIT
};

typedef enum swl_key_type
{
	SWLK_UNKNOWN,
	SWLK_Q,
	SWLK_W,
	SWLK_E,
	SWLK_A,
	SWLK_S,
	SWLK_D,
	SWLK_UP,
	SWLK_DOWN,
	SWLK_LEFT,
	SWLK_RIGHT,
	SWLK_RETURN,
	SWLK_ESCAPE
} swl_key_t;

struct swl_focus_event
{
	bool acquired;
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
		struct swl_focus_event focus_event;
	};
};

enum swl_configurable
{
	SWLC_FULLSCREEN,
	SWLC_RESIZABLE,
	SWLC_OPENGL_VERSION,
	SWLC_DEPTH_SIZE,
	SWLC_STENCIL_SIZE,
	SWLC_CURSOR
};

enum swl_opengl_version
{
	SWLV_OPENGL_ES2,
	SWLV_OPENGL_ES
};

SWL_API enum swl_result swl_init(const char *title, unsigned int width, unsigned int height);
SWL_API enum swl_result swl_set_config(enum swl_configurable field, size_t value);
SWL_API void swl_quit(void);
SWL_API void swl_swap(void);
SWL_API bool swl_query(struct swl_event *event);
SWL_API bool swl_wait(struct swl_event *event);
SWL_API void *swl_get_function(const char *name);

#ifdef __cplusplus
}
#endif
