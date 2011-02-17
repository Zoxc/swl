#include <EGL/egl.h>
#include "swl.h"

static EGLDisplay egl_display = EGL_NO_DISPLAY;
static EGLSurface egl_surface = EGL_NO_SURFACE;

enum swl_result swl_context_allocate(swl_window_t window, swl_display_t display)
{
	EGLConfig egl_config;
	EGLContext egl_context;
	
	EGLint major_version, minor_version;
	
	const EGLint config_attributes[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE};
		
	int configs;

	EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

	egl_context = EGL_NO_CONTEXT;
	egl_display = eglGetDisplay(display);
	
	if(egl_display == EGL_NO_DISPLAY)
	{
		egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		
		if(egl_display == EGL_NO_DISPLAY)
			return SWLR_ERROR_CONTEXT_EGL_DISPLAY_CREATION;
	}

	if (!eglInitialize(egl_display, &major_version, &minor_version))
		return SWLR_ERROR_CONTEXT_EGL_CREATION;

	if (!eglChooseConfig(egl_display, config_attributes, &egl_config, 1, &configs) || (configs != 1))
		return SWLR_ERROR_CONTEXT_EGL_CONFIG;

	egl_surface = eglCreateWindowSurface(egl_display, egl_config, window, NULL);

	if(egl_surface == EGL_NO_SURFACE)
	{
		egl_surface = eglCreateWindowSurface(egl_display, egl_config, NULL, NULL);
		
		if(egl_surface == EGL_NO_SURFACE)
			return SWLR_ERROR_CONTEXT_EGL_SURFACE_CREATION;
	}

	egl_context = eglCreateContext(egl_display, egl_config, NULL, context_attributes);
	
	if(egl_context == EGL_NO_CONTEXT)
			return SWLR_ERROR_CONTEXT_EGL_CONTEXT_CREATION;

	eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);
	
	if(eglGetError() != EGL_SUCCESS)
		return SWLR_ERROR_CONTEXT_EGL_ERROR;
	else
		return SWLR_OK;
}

void swl_context_deallocate(void)
{
	if(egl_display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglTerminate(egl_display);
	}
}

void swl_context_swap(void)
{
	eglSwapBuffers(egl_display, egl_surface);
}
