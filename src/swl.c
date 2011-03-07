#include "swl-private.h"

void swl_context_swap(void);

struct swl_data swl;

SWL_API void SWL_API swl_set_resizable(bool resizable)
{
	swl.resizable = resizable;
}

SWL_API void swl_set_fullscreen(bool fullscreen)
{
	swl.fullscreen = fullscreen;
}

static enum swl_result swl_allocate(void)
{
	enum swl_result result = swl_platform_allocate(swl.title, swl.width, swl.height, &swl.window, &swl.display);
	
	if(result != SWLR_OK)
		return result;
	
	result = swl_context_allocate(swl.window, swl.display); 
	
	return result;
}

static void swl_deallocate(void)
{
	swl_context_deallocate();
	swl_platform_deallocate();
}

SWL_API enum swl_result swl_init(const char *title, unsigned int width, unsigned int height)
{
	enum swl_result result;

	swl.title = title;
	swl.width = width;
	swl.height = height;
	
	result = swl_allocate();
	
	if(result != SWLR_OK)
		swl_deallocate();

	return result;
}

SWL_API void swl_quit(void)
{
	swl_deallocate();
}

SWL_API void swl_swap()
{
	swl_context_swap();
}
