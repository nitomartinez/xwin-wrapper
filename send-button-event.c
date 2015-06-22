#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>


int parseint(const char *str) 
{
  int value; 
  if (strlen(str) > 2 && str[0] == '0' && str[1] == 'x') 
    {
      sscanf(str + 2, "%x", &value);
    }
  else
    {
      value = atoi(str);
    }
  return value;
}


unsigned int get_input_state(Display *xdpy) {
  Window root, dummy;
  int root_x, root_y, win_x, win_y;
  unsigned int mask;
  root = DefaultRootWindow(xdpy);
  _Xdebug = 1;

  Bool result = XQueryPointer(xdpy, root, &dummy, &dummy,
			      &root_x, &root_y, &win_x, &win_y, &mask);

  printf("get_input_state XQueryPointer=%d\n", result);

  return mask;
}

int clickbutton(Window windowid, Window subwindowid, int xroot, int yroot, int x, int y) {
  int screen = 0;

  Window w = InputFocus;

  XButtonEvent xbpe;
  XMotionEvent xmte;
  const char *displayname = getenv("DISPLAY");

  Display *display = XOpenDisplay(displayname);
  if (display == NULL) {
    fprintf(stderr, "Error opening display\n");
    return -1;
  }


  xmte.type = MotionNotify;
  xmte.display = display;
  xmte.window = windowid;
  xmte.root = RootWindow(display, screen);
  xmte.subwindow = None;
  xmte.time = CurrentTime;
  xmte.x_root = xroot;
  xmte.y_root = yroot;
  xmte.x = x;
  xmte.y = y;
  xmte.same_screen = True; /* Should we detect if window is on the same
                                 screen as cursor? */
  xmte.is_hint = NotifyNormal;

  int ret = XSendEvent(display, windowid, True, PointerMotionMask, (XEvent *)&xmte);
  int events = XFlush(display);
  printf("Number of events after XFlush was %d\n", events);

  if (!ret) {
    fprintf(stderr, "Error in XsendEvent: %d\n", ret);
    return ret;
  }



  xbpe.display = display;
  xbpe.window = windowid;
  xbpe.button = 1;
  xbpe.root = RootWindow(display, screen);
  xbpe.same_screen = True; /* Should we detect if window is on the same
                                 screen as cursor? */
  xbpe.state = get_input_state(display)  | Button1MotionMask;

  xbpe.subwindow = subwindowid;
  xbpe.time = CurrentTime;
  xbpe.type = ButtonPress | Button1MotionMask;
  xbpe.x_root = xroot;
  xbpe.y_root = yroot;
  xbpe.x = x;
  xbpe.y = y;

  /* Get the coordinates of the cursor relative to xbpe.window and also find what
   * subwindow it might be on */
  /*  Bool translate_result = XTranslateCoordinates(display, xbpe.root, xbpe.window, 
      xbpe.x_root, xbpe.y_root, &xbpe.x, &xbpe.y, &xbpe.subwindow);
  */
  /* Hack */
  xbpe.x += 10;
  xbpe.y += 10;

  printf("root window: 0x%x, window: 0x%x, xroot: %d, yroot: %d, x: %d, y:%d, subw: 0x%x\n", (unsigned)xbpe.root, (unsigned)xbpe.window, xbpe.x_root, xbpe.y_root, xbpe.x, xbpe.y, (unsigned)xbpe.subwindow);

  ret = XSendEvent(display, w, True, ButtonPressMask, (XEvent *)&xbpe);
  events = XFlush(display);
  printf("Number of events after XFlush was %d\n", events);

  if (!ret) {
    fprintf(stderr, "Error in XsendEvent: %d\n", ret);
    return ret;
  }

  /* Button release */
  xbpe.state = get_input_state(display);
  xbpe.type = ButtonRelease;

  ret = XSendEvent(display, w, True, ButtonPressMask, (XEvent *)&xbpe);
  events = XFlush(display);
  printf("Number of events after XFlush was %d\n", events);

  if (!ret) {
    fprintf(stderr, "Error in XsendEvent button release: %d\n", ret);
    return ret;
  }
  return ret;

}

int main(int argc, char *argv[], char *envp[]) {
    if (argc < 5) {
      fprintf(stderr,"You need to pass the window id xroot, yroot\n");
      return 1;
    }

    unsigned windowid=parseint(argv[1]);
    unsigned subwindowid=parseint(argv[1]);
    int xroot=parseint(argv[2]);
    int yroot=parseint(argv[3]);
    int xwin=parseint(argv[4]);
    int ywin=parseint(argv[5]);

    Window window = (Window)windowid;

    int result = clickbutton(window, subwindowid, xroot, yroot, xwin, ywin);

    return result;
}
