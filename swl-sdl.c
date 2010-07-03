#include <EGL/egl.h>
#include "SDL/SDL.h"
#include "SDL/SDL_syswm.h"
#include "swl.h"

enum swl_result swl_platform_allocate(const char *title, unsigned int width, unsigned int height, EGLNativeWindowType *handle, EGLDisplay *display)
{
	if(!SDL_Init(SDL_INIT_VIDEO))
		return SWLR_ERROR_PLATFORM_SDL_INIT;

	if (SDL_SetVideoMode(width, height, 32, SDL_ANYFORMAT) == 0)
		return SWLR_ERROR_PLATFORM_SDL_MODE;

	SDL_SysWMinfo info;
	
	SDL_GetWMInfo(&info);
	
	*handle = (EGLNativeWindowType)info.window;
	
	#ifdef WIN32
		HDC device = GetDC(*handle);
		
		if (!device)
			return SWLR_ERROR_PLATFORM_SDL_DEVICE;
	#endif
	
	*display = eglGetDisplay((EGLNativeDisplayType)device);
	
	return SWLR_OK;
}

void swl_platform_deallocate(void)
{
	SDL_Quit();
}
