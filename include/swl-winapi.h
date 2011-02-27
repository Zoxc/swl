#pragma once
#ifndef NOMINMAX
	#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include "windows.h"	

typedef HWND swl_window_t;
typedef HDC swl_display_t;

/*
 * Key type definition
 */
typedef DWORD swl_key_t;

#define SWLK_UP VK_UP
#define SWLK_DOWN VK_DOWN
#define SWLK_LEFT VK_LEFT
#define SWLK_RIGHT VK_RIGHT
#define SWLK_GO VK_RETURN

#define SWLR_ERROR_BACKEND_WINAPI_REGISTER_CLASS SWLR_BACKEND(0)
#define SWLR_ERROR_BACKEND_WINAPI_HANDLE SWLR_BACKEND(1)
#define SWLR_ERROR_BACKEND_WINAPI_DEVICE SWLR_BACKEND(2)
