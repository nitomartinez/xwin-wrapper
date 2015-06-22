#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>

#define QVD_XPROP_TRANSLATION "_QVD_CLIENT_WID"


Window get_window_id_prop(Window windowid)
{
  const char *property = QVD_XPROP_TRANSLATION;
  Atom     actual_type;
  int      actual_format;
  unsigned long  nitems;
  unsigned long  bytes;
  long     *data = NULL;

  Display *display = XOpenDisplay(0);
  if (!display)
    {
      printf("can't open display\n");
      return (0);
    }
  if (windowid == 0) {
      printf("Empty window id\n");
      return (0);
  }

  printf("get_window_id_prop, trying to get property : %s for window id 0x%x\n", property, (unsigned)windowid);
  Atom propertyAtom = XInternAtom(display, property, True);
  printf("get_window_id_prop, after getting property: %s, %ld\n", property, propertyAtom);
  if (propertyAtom == None)
    {
      printf("The Xwindows property %s does not exists\n", property);
      return 0;
    }
  Status result = XGetWindowProperty(
                display,
                windowid,
                propertyAtom, //replace this with your property
                0,
                (~0L),
                False,
                AnyPropertyType,
                &actual_type,
                &actual_format,
                &nitems,
                &bytes,
                (unsigned char**)&data);
  if (result != Success)
    {
      printf("Error getting XGetWindowProperty %s for id 0x%lx\n", property, windowid);
      return (0);
    }
  printf ("get_window_id_prop: data actual_type: %ld, format: %d, nitems: %ld, bytes: %ld, data: 0x%lx %ld\n", actual_type, actual_format, nitems, bytes, *data, *data);
  if (nitems < 1 || data == NULL)
    return 0;
  return *data;
}

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


static int errorHandler(Display *dpy, XErrorEvent *error)
{
      char buffer1[1024];
      XGetErrorText(dpy, error -> error_code, buffer1, 1024);
      fprintf(stderr, 
	      "_X Error of failed request:  %s\n_  Major opcode of failed request: %3d \n_ Serial number of failed request:%5ld\n_  Current serial number in output stream:?????\n",
	      buffer1,
	      error -> request_code,
	      error -> serial);
      return 2;
}

int main( int   argc,
          char *argv[] )
{

    if (argc < 2) {
      g_print("You need to pass the window id\n");
      return 1;
    }

    Display *display = XOpenDisplay(0);
    if (!display)
      {
	printf("can't open display\n");
	return (1);
      }
    
    XSetErrorHandler(errorHandler);

    unsigned windowid=parseint(argv[1]);
    Window root, parent, *children;
    unsigned nchildren;
    printf("XQueryTree window 0x%x\n", windowid);
    Status s = XQueryTree(display, (Window)windowid, &root, &parent, &children, &nchildren);
    printf("XQueryTree status %d\n", s);
    XSetErrorHandler(NULL);

    printf("XQueryTree window 0x%x, root: 0x%x, parent: 0x%x, nchildren: %d\n", windowid, (unsigned)root, (unsigned)parent, nchildren);

    unsigned translated_winid =  get_window_id_prop(windowid);
    printf("The windowid 0x%x was translated to 0x%x for the property %s", windowid, translated_winid, QVD_XPROP_TRANSLATION);
}
