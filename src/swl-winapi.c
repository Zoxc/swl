#include "swl-private.h"

static HWND window_handle = 0;
static ATOM register_class = 0;

static bool update_cursor()
{
	if(!swl.cursor)
	{
		RECT rect;
		POINT top_left, bottom_right;
		
		if(GetClientRect(window_handle, &rect) == 0)
			return false;
		
		top_left.x = rect.left;
		top_left.y = rect.top;
		
		bottom_right.x = rect.right;
		bottom_right.y = rect.bottom;
		
		if(ClientToScreen(window_handle, &top_left) == 0)
			return false;
		
		if(ClientToScreen(window_handle, &bottom_right) == 0)
			return false;
		
		rect.left = top_left.x;
		rect.top = top_left.y;
		
		rect.right = bottom_right.x;
		rect.bottom = bottom_right.y;
		
		if(ClipCursor(&rect) == 0)
			return false;
	}
	
	return true;
}

static swl_key_t vk_to_swl(DWORD key)
{
	switch(key)
	{
		case 'Q':
			return SWLK_Q;
		
		case 'W':
			return SWLK_W;
		
		case 'E':
			return SWLK_E;
		
		case 'A':
			return SWLK_A;
		
		case 'S':
			return SWLK_S;
		
		case 'D':
			return SWLK_D;
		
		case VK_UP:
			return SWLK_UP;
		
		case VK_DOWN:
			return SWLK_DOWN;
		
		case VK_LEFT:
			return SWLK_LEFT;
		
		case VK_RIGHT:
			return SWLK_RIGHT;
		
		case VK_RETURN:
			return SWLK_RETURN;
			
		case VK_ESCAPE:
			return SWLK_ESCAPE;
			
		default:
			return SWLK_UNKNOWN;
	}
};

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

		case WM_USER + 1:
			event->type = SWLE_MOUSERAW;
			event->mouse_event.x = (int)msg->wParam;
			event->mouse_event.y = (int)msg->lParam;
			break;

		case WM_KEYDOWN:
			event->type = SWLE_KEYDOWN;
			event->key_event.key = vk_to_swl(msg->wParam);
			break;

		case WM_KEYUP:
			event->type = SWLE_KEYUP;
			event->key_event.key = vk_to_swl(msg->wParam);
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

SWL_API bool swl_query(struct swl_event *event)
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

SWL_API bool swl_wait(struct swl_event *event)
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
		case WM_INPUT:
            {
                RAWINPUT input;
				
                UINT size = sizeof(input);
				
                GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &input, &size, sizeof(input.header));
				
				PostMessage(hWnd, WM_USER + 1, input.data.mouse.lLastX, input.data.mouse.lLastY);
            }
			break;
			
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

enum swl_result swl_platform_update(enum swl_configurable field, size_t value)
{
	switch(field)
	{
		case SWLC_CURSOR:
			{
				if(value)
				{
					ShowCursor(TRUE);
					ClipCursor(0);
					return SWLR_OK;
				}
				else
				{
					ShowCursor(FALSE);
					return update_cursor();
				}
			}
		
		default:
			return SWLR_UNSUPPORTED;
	}
}

SWL_API enum swl_result swl_platform_allocate(const char *title, unsigned int width, unsigned int height, swl_window_t *window, swl_display_t *display)
{
	WNDCLASS swc;
	ATOM register_class;
	RECT rect;
	HDC hdc;
	DWORD style = WS_CAPTION | WS_VISIBLE | WS_SYSMENU;

	if(swl.resizable)
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
	
	RAWINPUTDEVICE mouse;
	mouse.usUsagePage = 1;
	mouse.usUsage = 2;
	mouse.dwFlags = 0;
	mouse.hwndTarget = window_handle;
	
	if(RegisterRawInputDevices(&mouse, 1, sizeof(mouse)) == FALSE)
		return SWLR_ERROR_BACKEND_WINAPI_RAW_INPUT;
	
	return SWLR_OK;
}

SWL_API void swl_platform_deallocate(void)
{
	if(window_handle)
		DestroyWindow(window_handle);
	
	if(register_class)
		UnregisterClass((LPCTSTR)(size_t)register_class, 0);
}
