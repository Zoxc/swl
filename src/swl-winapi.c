#include <windows.h>
#include "swl.h"

static HWND window_handle = 0;
static ATOM register_class = 0;

static bool process_message(struct swl_event *event, MSG *msg)
{
	if(msg->message == WM_QUIT)
	{
		event->type = SWLE_QUIT;
		return true;
	}
			
	if(msg->hwnd != window_handle)
		return false;
		
	switch(msg->message)
	{
		case WM_USER:
			event->type = SWLE_RESIZE;
			event->size_event.width = msg->wParam;
			event->size_event.height = msg->lParam;
			break;

		case WM_KEYDOWN:
			event->type = SWLE_KEYDOWN;
			event->key_event.key = (swl_key_t)msg->wParam;
			break;

		case WM_KEYUP:
			event->type = SWLE_KEYUP;
			event->key_event.key = (swl_key_t)msg->wParam;
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			event->type = SWLE_MOUSEDOWN;
			event->mouse_event.x = LOWORD(msg->lParam);
			event->mouse_event.y = HIWORD(msg->lParam);
			break;

		case WM_MOUSEMOVE:
			event->type = SWLE_MOUSEMOVE;
			event->mouse_event.x = LOWORD(msg->lParam);
			event->mouse_event.y = HIWORD(msg->lParam);
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			event->type = SWLE_MOUSEUP;
			event->mouse_event.x = LOWORD(msg->lParam);
			event->mouse_event.y = HIWORD(msg->lParam);
			break;

		default:
			return false;
	}

	return true;
}

bool SWL_API swl_query(struct swl_event *event)
{
	MSG msg;
	
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		
		if(process_message(event, &msg))
		{
			DispatchMessage(&msg);
			return true;
		}
		
		DispatchMessage(&msg);
	}
	
	return false;
}

bool SWL_API swl_wait(struct swl_event *event)
{
	MSG msg;
	
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		
		if(process_message(event, &msg))
		{
			DispatchMessage(&msg);
			return true;
		}
		
		DispatchMessage(&msg);
	}
	
	event->type = SWLE_QUIT;
	
	return false;
}

static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SYSCOMMAND:
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			SetCapture(hWnd);
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			ReleaseCapture();
			break;

		case WM_SIZE:
			PostMessage(hWnd, WM_USER, LOWORD(lParam), HIWORD(lParam));
			return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 1;

		case WM_PAINT:
			// Request redraw
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

enum swl_result swl_platform_allocate(const char *title, unsigned int width, unsigned int height, bool resizable, swl_window_t *window, swl_display_t *display)
{
	WNDCLASS swc;
	ATOM register_class;
	RECT rect;
	HDC hdc;
	DWORD style = WS_CAPTION | WS_VISIBLE | WS_SYSMENU;

	if(resizable)
		style |= WS_SIZEBOX;

	swc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	swc.lpfnWndProc = wnd_proc;
	swc.cbClsExtra = 0;
	swc.cbWndExtra = 0;
	swc.hInstance = 0;
	swc.hIcon = 0;
	swc.hCursor = LoadCursor(NULL, IDC_ARROW);
	swc.lpszMenuName = 0;
	swc.hbrBackground = 0;
	swc.lpszClassName = "RenderFrame";

	register_class = RegisterClass(&swc);

	if (!register_class)
		return SWLR_ERROR_BACKEND_WINAPI_REGISTER_CLASS;

	SetRect(&rect, 0, 0, width, height);
	
	AdjustWindowRectEx(&rect, style, false, 0);
	
	window_handle = CreateWindow("RenderFrame", title, style, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0, 0);
	
	if(window_handle == 0)
		return SWLR_ERROR_BACKEND_WINAPI_HANDLE;
	
	hdc = GetDC(window_handle);
	
	if (!hdc)
		return SWLR_ERROR_BACKEND_WINAPI_DEVICE;

	*display = hdc;
	*window = window_handle;
	
	return SWLR_OK;
}

void swl_platform_deallocate(void)
{
	if(window_handle)
		DestroyWindow(window_handle);
	
	if(register_class)
		UnregisterClass((LPCTSTR)(size_t)register_class, 0);
}
