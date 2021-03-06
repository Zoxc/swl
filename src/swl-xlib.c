#include "swl-private.h"
#include "X11/Xutil.h"

static swl_key_t keysym_to_swl(KeySym key)
{
	switch(key)
	{
		case XK_Up:
			return SWLK_UP;
		
		case XK_Down:
			return SWLK_DOWN;
		
		case XK_Left:
			return SWLK_LEFT;
		
		case XK_Right:
			return SWLK_RIGHT;
		
		case XK_Return:
			return SWLK_RETURN;
			
		default:
			return SWLK_UNKNOWN;
	}
};

static Window x11_window;
static Display *x11_display;
static XVisualInfo x11_visual;
static Colormap x11_colormap;
static Atom wm_delete_message;

static bool process_event(struct swl_event *event, XEvent *x11_event)
{
	switch(x11_event->type)
	{
		case KeyPress:
			event->type = SWLE_KEYDOWN;
			event->key_event.key = keysym_to_swl(XLookupKeysym(&x11_event->xkey, 0));
			break;

		case KeyRelease:
			event->type = SWLE_KEYUP;
			event->key_event.key = keysym_to_swl(XLookupKeysym(&x11_event->xkey, 0));
			break;

		case ButtonPress:
			event->type = SWLE_MOUSEDOWN;
			event->mouse_event.x = x11_event->xbutton.x;
			event->mouse_event.y = x11_event->xbutton.y;
			break;

		case ButtonRelease:
			event->type = SWLE_MOUSEUP;
			event->mouse_event.x = x11_event->xbutton.x;
			event->mouse_event.y = x11_event->xbutton.y;
			break;

		case MotionNotify:
			event->type = SWLE_MOUSEMOVE;
			event->mouse_event.x = x11_event->xmotion.x;
			event->mouse_event.y = x11_event->xmotion.y;
			break;
			
		case ClientMessage:
			if(x11_event->xclient.data.l[0] != wm_delete_message)
				return false;
			
			event->type = SWLE_QUIT;
			break;

		default:
			return false;
			break;
	}
	
	return true;
}

SWL_API bool swl_query(struct swl_event *event)
{
	XEvent x11_event;
	
	int pending = XPending(x11_display);
	
	while(pending--)
	{
		XNextEvent(x11_display, &x11_event);
		
		if(process_event(event, &x11_event))
			return true;
	}
	
	return false;
}

SWL_API bool swl_wait(struct swl_event *event)
{
	XEvent x11_event;
	
	while(true)
	{
		XNextEvent(x11_display, &x11_event);
		
		if(process_event(event, &x11_event))
			break;
	}
	
	return true;
}

enum swl_result swl_platform_update(enum swl_configurable field, size_t value)
{
	return SWLR_OK;
}

SWL_API enum swl_result swl_platform_allocate(const char *title, unsigned int width, unsigned int height, swl_window_t *window, swl_display_t *display)
{
	XSetWindowAttributes swa;
	unsigned int mask;
	int depth;
	long x11_screen;
	Atom wm_state;
	Atom wm_fullscreen;
	XEvent x_event;
	
	x11_display = XOpenDisplay(0);

	if (!x11_display)
		return SWLR_ERROR_BACKEND_XLIB_DISPLAY;
	
	x11_screen = XDefaultScreen(x11_display);

	depth = DefaultDepth(x11_display, x11_screen);
	
	if(!XMatchVisualInfo(x11_display, x11_screen, depth, TrueColor, &x11_visual))
		return SWLR_ERROR_BACKEND_XLIB_VISUAL;

	x11_colormap = XCreateColormap(x11_display, RootWindow(x11_display, x11_screen), x11_visual.visual, AllocNone);

	swa.colormap = x11_colormap;
	swa.background_pixel = 0xFFFFFFFF;
	swa.border_pixel = 0;
	swa.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | LeaveWindowMask;

	mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

	x11_window = XCreateWindow(x11_display, RootWindow(x11_display, x11_screen), 0, 0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, mask, &swa);

	XSetStandardProperties(x11_display, x11_window, title, title, None, 0, 0, 0);

	wm_delete_message = XInternAtom(x11_display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(x11_display, x11_window, &wm_delete_message, 1);
	
	XMapWindow(x11_display, x11_window);

	XFlush(x11_display);

	if(swl.fullscreen)
	{
		wm_state = XInternAtom(x11_display, "_NET_WM_STATE", False);
		wm_fullscreen = XInternAtom(x11_display,"_NET_WM_STATE_FULLSCREEN", False);
		
		x_event.xclient.type = ClientMessage;
		x_event.xclient.serial = 0;
		x_event.xclient.send_event = True;
		x_event.xclient.window = x11_window;
		x_event.xclient.message_type = wm_state;
		x_event.xclient.format = 32;
		x_event.xclient.data.l[0] = 1;
		x_event.xclient.data.l[1] = wm_fullscreen;
		x_event.xclient.data.l[2] = 0;
		
		XSendEvent(x11_display, DefaultRootWindow(x11_display), False, SubstructureRedirectMask | SubstructureNotifyMask, &x_event);
	}

	*window = x11_window;
	*display = x11_display;
	
	return SWLR_OK;
}

SWL_API void swl_platform_deallocate(void)
{
	if(x11_window)
		XDestroyWindow(x11_display, x11_window);

	if(x11_colormap)
		XFreeColormap(x11_display, x11_colormap);

	if (x11_display)
		XCloseDisplay(x11_display);
}
