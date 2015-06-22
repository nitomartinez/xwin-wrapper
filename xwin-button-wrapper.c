/*
 * Really simple widget. Creates a window where it should reparent to, and creates
 * a plug widget inside it (an embedder window with a button in it).
 * 
 * Test program to create an embedded window which receives an event to test nxagent.
 * 
 * Receives as the first argument the windowid where it should reparent to
 * and sends via stdout the windowid where the plug is waiting for.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdk.h>


/* This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below. */
static void hello( GtkWidget *widget,
                   gpointer   data )
{
    g_print ("Hello World\n");
}

static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    /* If you return FALSE in the "delete-event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    g_print ("delete event occurred\n");

    /* Change TRUE to FALSE and the main window will be destroyed with
     * a "delete-event". */

    return TRUE;
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


/* Another callback */
static void destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}

int main( int   argc,
          char *argv[] )
{
    /* GtkWidget is the storage type for widgets */
    GtkWidget *window;
    /* GtkWindow *window; */
    GtkWidget *button;
    GtkWidget *box;
    GtkWidget *socket;
    GdkWindow* window_container;

    if (argc < 2) {
      g_print("You need to pass the window id\n");
      return 1;
    }
    int windowid=parseint(argv[1]);
    
    /* This is called in all GTK applications. Arguments are parsed
     * from the command line and are returned to the application. */
    gtk_init (&argc, &argv);
    
    /* create a new window */
    /*    window = gtk_window_new (GTK_WINDOW_TOPLEVEL); */

    window = gtk_window_new (GTK_WINDOW_POPUP);


    /* Create socket */
    /* socket = gtk_socket_new (); */
    /* gtk_container_add (GTK_CONTAINER (window), socket); */
    /* gtk_widget_set_size_request (socket, 640, 385); */
    /* gtk_widget_show (socket); */

    /* Showing the window last so everything pops up at once. */
    /* gtk_widget_realize (window); */

    /* Reparent */
    window_container = gdk_window_foreign_new(windowid);
    if (GTK_WIDGET_MAPPED(window))
      gtk_widget_unmap(window);

    g_signal_connect(G_OBJECT(window), "delete_event",
		     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(window), "destroy",
		     G_CALLBACK(gtk_main_quit), NULL);

    /* g_print ("socket id 0x%x\n", gtk_socket_get_id(GTK_SOCKET(socket))); */
    
    /* When the window is given the "delete-event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
    g_signal_connect (window, "delete-event",
		      G_CALLBACK (delete_event), NULL);
    
    /* Here we connect the "destroy" event to a signal handler.  
     * This event occurs when we call gtk_widget_destroy() on the window,
     * or if we return FALSE in the "delete-event" callback. */
    g_signal_connect (window, "destroy",
		      G_CALLBACK (destroy), NULL);
    
    /* Sets the border width of the window. */
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
    /* Creates a new button with the label "Hello World". */
    button = gtk_button_new_with_label ("Hello World");

    /*
    g_printf("window events: 0x%x\n", gtk_widget_get_events(window));
    g_printf("button events: 0x%x\n", gtk_widget_get_events(button));

    GTK_WIDGET_SET_FLAGS ( button, GTK_CAN_FOCUS );
    gtk_widget_add_events ( GTK_WIDGET(button), GDK_FOCUS_CHANGE_MASK );
    GTK_WIDGET_SET_FLAGS ( window, GTK_CAN_FOCUS );
    gtk_widget_add_events ( GTK_WIDGET(window), GDK_FOCUS_CHANGE_MASK );
    gdk_window_set_events(GDK_WINDOW(window->window), GDK_BUTTON_PRESS_MASK );

    gtk_widget_set_events(window, GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_MOTION_MASK |  GDK_BUTTON1_MOTION_MASK | GDK_BUTTON2_MOTION_MASK | GDK_BUTTON3_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK); 
    gtk_widget_set_events(button, GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_MOTION_MASK |  GDK_BUTTON1_MOTION_MASK | GDK_BUTTON2_MOTION_MASK | GDK_BUTTON3_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);


    g_printf("window events after: 0x%x\n", gtk_widget_get_events(window));
    g_printf("button events after: 0x%x\n", gtk_widget_get_events(button));
 
    */

    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    /* When the button receives the "clicked" signal, it will call the
     * function hello() passing it NULL as its argument.  The hello()
     * function is defined above. */
    g_signal_connect (button, "clicked",
		      G_CALLBACK (hello), NULL);
    
    /* This will cause the window to be destroyed by calling
     * gtk_widget_destroy(window) when "clicked".  Again, the destroy
     * signal could come from here, or the window manager. */
    g_signal_connect_swapped (button, "clicked",
			      G_CALLBACK (gtk_widget_destroy),
                              window);
    
    /* This packs the button into the window (a gtk container). */
    gtk_container_add (GTK_CONTAINER (window), button);

    
    /* The final step is to display this newly created widget. */
    gtk_widget_show (button);
    
    /* and the window */
    gtk_widget_show (window);

    gdk_window_reparent(window->window, window_container, 0, 0);

    gdk_window_set_accept_focus (window->window, TRUE);
    /* gdk_window_set_focus_on_map (window->window, TRUE); */
    gdk_window_set_keep_above (window->window, TRUE);
    /* gtk_window_set_decorated ((struct GtkWindow *)window, FALSE); */
    gdk_window_set_accept_focus (button->window, TRUE);
    /* gdk_window_set_focus_on_map (button->window, TRUE); */
    gdk_window_set_keep_above (button->window, TRUE);
    /* All GTK applications must have a gtk_main(). Control ends here
     * and waits for an event to occur (like a key press or
     * mouse event). */
    g_printf("window events end: 0x%x\n", gtk_widget_get_events(window));
    g_printf("button events end: 0x%x\n", gtk_widget_get_events(button));
    g_printf("window can focus: %d\n", gtk_widget_get_can_focus(window));
    g_printf("button can focus: %d\n", gtk_widget_get_can_focus(button));

    gulong data = 0;

    GdkAtom qvd_redirect_event = gdk_atom_intern("_QVD_REDIRECT_EVENT", False);
    /*Atom qvd_redirect_event = gdk_x11_get_xatom_by_name("_QVD_REDIRECT_EVENT");*/
    gdk_property_change(window->window, 
			qvd_redirect_event,
			gdk_x11_xatom_to_atom(gdk_x11_get_xatom_by_name("INTEGER")),
			/* GDK_NONE, */
			32,
			GDK_PROP_MODE_REPLACE,
			(const gchar *)&data,
			1); 


    gtk_main ();
    
    return 0;
}
