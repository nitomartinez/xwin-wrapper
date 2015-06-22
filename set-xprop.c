#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

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

int main( int   argc,
          char *argv[] )
{
 
    if (argc < 2) {
      g_print("You need to pass the window id\n");
      return 1;
    }
    int windowid=parseint(argv[1]);

    const char *displayname = getenv("DISPLAY");
    Display *display = XOpenDisplay(displayname);
    if (display == NULL) {
      fprintf(stderr, "Error opening display\n");
      return -1;
    }
    _Xdebug = 1;

    Atom qvd_client_wid_prop = XInternAtom(display, "_QVD_REDIRECT_EVENT", False);

    if (qvd_client_wid_prop == BadAlloc || qvd_client_wid_prop == BadAtom || qvd_client_wid_prop == None) {
      fprintf(stderr, "Error creating _QVD_REDIRECT_EVENT Atom: %d\n", (unsigned)qvd_client_wid_prop);
      return -1;
    }

    Window w = (Window) windowid;

    /*
    Atom integer_prop = XInternAtom(display, "INTEGER", True);
    if (integer_prop == BadAlloc || integer_prop == BadAtom || integer_prop == None) {
      fprintf(stderr, "Error creating _QVD_REDIRECT_EVENT Atom: %d\n", (unsigned)integer_prop);
      return -1;
    }
    */

    /*    XTextProperty p;*/
    unsigned long value = 0;
    XTextProperty p = 
      {
	(char *)&value,
	XA_INTEGER,
	32,
	1
      };
    
    /*    char *list[2] = { "test", NULL};
    XStringListToTextProperty(list,1,&p);
    */
    XSetTextProperty(display, w, &p, qvd_client_wid_prop);
    XSync(display, False);
    return 0;
}
