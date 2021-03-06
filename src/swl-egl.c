#include "swl-private.h"
#include <EGL/egl.h>

static EGLDisplay egl_display = EGL_NO_DISPLAY;
static EGLSurface egl_surface = EGL_NO_SURFACE;

SWL_API enum swl_result swl_context_allocate(swl_window_t window, swl_display_t display)
{
	EGLConfig egl_config;
	EGLContext egl_context;
	
	EGLint major_version, minor_version;
	
	EGLint config_attributes[16];
	
	size_t config_index = 0;
	
	config_attributes[config_index++] = EGL_SURFACE_TYPE;
	config_attributes[config_index++] = EGL_WINDOW_BIT;
	
	EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

	switch(swl.opengl_version)
	{
		case SWLV_OPENGL_ES2:
			config_attributes[config_index++] = EGL_RENDERABLE_TYPE;
			config_attributes[config_index++] = EGL_OPENGL_ES2_BIT;
			context_attributes[2] = 2;
			break;
		
		case SWLV_OPENGL_ES:
			config_attributes[config_index++] = EGL_RENDERABLE_TYPE;
			config_attributes[config_index++] = EGL_OPENGL_ES_BIT;
			context_attributes[2] = 1;
			break;
			
		default:
			return SWLR_UNSUPPORTED;
	}
	
	if(swl.depth_size)
	{
		config_attributes[config_index++] = EGL_DEPTH_SIZE;
		config_attributes[config_index++] = swl.depth_size;
	}
	
	if(swl.stencil_size)
	{
		config_attributes[config_index++] = EGL_STENCIL_SIZE;
		config_attributes[config_index++] = swl.stencil_size;
	}
	
	config_attributes[config_index++] = EGL_NONE;
	
	EGLint configs;

	egl_context = EGL_NO_CONTEXT;
	egl_display = eglGetDisplay((EGLNativeDisplayType)display);
	
	if(egl_display == EGL_NO_DISPLAY)
	{
		egl_display = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
		
		if(egl_display == EGL_NO_DISPLAY)
			return SWLR_ERROR_CONTEXT_EGL_DISPLAY_CREATION;
	}

	if (!eglInitialize(egl_display, &major_version, &minor_version))
		return SWLR_ERROR_CONTEXT_EGL_CREATION;

	if (!eglChooseConfig(egl_display, config_attributes, &egl_config, 1, &configs) || (configs != 1))
		return SWLR_ERROR_CONTEXT_EGL_CONFIG;

	egl_surface = eglCreateWindowSurface(egl_display, egl_config, (EGLNativeWindowType)window, NULL);

	if(egl_surface == EGL_NO_SURFACE)
		return SWLR_ERROR_CONTEXT_EGL_SURFACE_CREATION;
	
	egl_context = eglCreateContext(egl_display, egl_config, NULL, context_attributes);
	
	if(egl_context == EGL_NO_CONTEXT)
			return SWLR_ERROR_CONTEXT_EGL_CONTEXT_CREATION;

	eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);
	
	if(eglGetError() != EGL_SUCCESS)
		return SWLR_ERROR_CONTEXT_EGL_ERROR;
	else
		return SWLR_OK;
}

SWL_API void swl_context_deallocate(void)
{
	if(egl_display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglTerminate(egl_display);
	}
}

SWL_API void swl_context_swap(void)
{
	eglSwapBuffers(egl_display, egl_surface);
}

SWL_API void *swl_get_function(const char *name)
{
	return (void *)eglGetProcAddress(name);
}
