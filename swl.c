#include "swl.h"
#ifdef SWL_OPENGL
	#include <GL/glew.h>
#endif

enum swl_result swl_platform_allocate(const char *title, unsigned int width, unsigned int height, bool resizable, swl_window_t *window, swl_display_t *display);
void swl_platform_deallocate(void);

enum swl_result swl_context_allocate(swl_window_t window, swl_display_t display);
void swl_context_deallocate(void);
void swl_context_swap(void);

static struct {
	/*
	 * Common stuff
	 */
	const char *title;
	unsigned int width;
	unsigned int height;
	bool resizable;
	swl_window_t window;
	swl_display_t display;
} swl;

static enum swl_result swl_allocate(void)
{
	enum swl_result result = swl_platform_allocate(swl.title, swl.width, swl.height, swl.resizable, &swl.window, &swl.display);
	
	if(result != SWLR_OK)
		return result;
	
	result = swl_context_allocate(swl.window, swl.display); 

	#ifdef SWL_OPENGL
		if(glewInit() != GLEW_OK)
			return SWLR_ERROR_GLEW_ERROR;
	#endif
	
	return result;
}

static void swl_deallocate(void)
{
	swl_context_deallocate();
	swl_platform_deallocate();
}

enum swl_result SWL_API swl_init(const char *title, unsigned int width, unsigned int height, bool resizable)
{
	enum swl_result result;

	swl.title = title;
	swl.width = width;
	swl.height = height;
	swl.resizable = resizable;
	
	result = swl_allocate();
	
	if(result != SWLR_OK)
		swl_deallocate();

	return result;
}

void SWL_API swl_quit(void)
{
	swl_deallocate();
}

void SWL_API swl_swap()
{
	swl_context_swap();
}
