#include <windows.h>
#include "swl.h"

static HGLRC wgl_rc;
static HDC wgl_dc;

SWL_API enum swl_result swl_context_allocate(swl_window_t window, swl_display_t display)
{
    PIXELFORMATDESCRIPTOR pfd;
	int format;

    ZeroMemory (&pfd, sizeof (PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof (PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    format = ChoosePixelFormat (display, &pfd);
    SetPixelFormat (display, format, &pfd);
	
	wgl_rc = wglCreateContext(display);
	wgl_dc = display;
	
	if(!wgl_rc)
		return SWLR_ERROR_CONTEXT_WGL_CONTEXT_CREATION;

    wglMakeCurrent(display, wgl_rc);
	
	return SWLR_OK;
}

SWL_API void swl_context_deallocate(void)
{
	wglDeleteContext(wgl_rc);
}

SWL_API void swl_context_swap(void)
{
	SwapBuffers(wgl_dc);
}

SWL_API void *swl_get_function(const char *name)
{
	return (void *)wglGetProcAddress(name);
}
