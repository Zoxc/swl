#include <EGL/egl.h>
#include "swl.h"

enum swl_result swl_platform_allocate(const char *title, unsigned int width, unsigned int height, EGLNativeWindowType *handle, EGLDisplay *display);
void swl_platform_deallocate(void);

static struct {
	/*
	 * Common stuff
	 */
	const char *title;
	unsigned int width;
	unsigned int height;
	
	/*
	 * EGL stuff
	 */
	EGLDisplay egl_display;
	EGLConfig egl_config;
	EGLSurface egl_surface;
	EGLContext egl_context;
	EGLNativeWindowType egl_handle;
} swl;

static enum swl_result swl_allocate(void)
{
	swl.egl_display = EGL_NO_DISPLAY;
	swl.egl_surface = EGL_NO_SURFACE;
	swl.egl_context = EGL_NO_CONTEXT;
	
	enum swl_result result = swl_platform_allocate(swl.title, swl.width, swl.height, &swl.egl_handle, &swl.egl_display);
	
	if(result != SWLR_OK)
		return result;

	if(swl.egl_display == EGL_NO_DISPLAY)
	{
		swl.egl_display = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
		
		if(swl.egl_display == EGL_NO_DISPLAY)
			return SWLR_ERROR_DISPLAY_CREATION;
	}

	EGLint major_version, minor_version;

	if (!eglInitialize(swl.egl_display, &major_version, &minor_version))
		return SWLR_ERROR_EGL_CREATION;

	const EGLint config_attributes[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE};

	int configs;
	
	if (!eglChooseConfig(swl.egl_display, config_attributes, &swl.egl_config, 1, &configs) || (configs != 1))
		return SWLR_ERROR_EGL_CONFIG;

	swl.egl_surface = eglCreateWindowSurface(swl.egl_display, swl.egl_config, swl.egl_handle, NULL);

	if(swl.egl_surface == EGL_NO_SURFACE)
	{
		swl.egl_surface = eglCreateWindowSurface(swl.egl_display, swl.egl_config, NULL, NULL);
		
		if(swl.egl_surface == EGL_NO_SURFACE)
			return SWLR_ERROR_SURFACE_CREATION;
	}

	EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

	swl.egl_context = eglCreateContext(swl.egl_display, swl.egl_config, NULL, context_attributes);
	
	if(swl.egl_context == EGL_NO_CONTEXT)
			return SWLR_ERROR_CONTEXT_CREATION;

	eglMakeCurrent(swl.egl_display, swl.egl_surface, swl.egl_surface, swl.egl_context);
	
	return SWLR_OK;
}

static void swl_deallocate(void)
{
	if(swl.egl_display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(swl.egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglTerminate(swl.egl_display);
	}

	swl_platform_deallocate();
}

enum swl_result SWL_API swl_init(const char *title, unsigned int width, unsigned int height)
{
	swl.title = title;
	swl.width = width;
	swl.height = height;
	
	enum swl_result result = swl_allocate();
	
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
	eglSwapBuffers(swl.egl_display, swl.egl_surface);
}
