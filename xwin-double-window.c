/*
 * Two simple widget. 
 *
 * 1) Creates a window where it should reparent to, and creates
 * a plug widget inside it (an embedder window with a button in it). The parent
 * should be a mapped window inside NX
 * 
 * 2) Creates a window in the same location but on top of the NX input window.
 *
 * The process is as follows:
 *
 * if the 1) first window receives
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


/*
 *
 */
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

static GtkWidget *entry;
static void print_hello_world( GtkWidget *widget,
                   gpointer   data )
{
  g_print ("Hello World: <%s>\n", gtk_entry_get_text(GTK_ENTRY(entry)));
}


GdkWindow *get_window_cotainer(int id) {
  GdkWindow* window_container = gdk_window_foreign_new(id);;
  return window_container;
}

GtkWidget *create_bottom_window() {
  GtkWidget *window = gtk_window_new (GTK_WINDOW_POPUP);
  /*  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);*/
  if (GTK_WIDGET_MAPPED(window))
    gtk_widget_unmap(window);

  g_signal_connect(G_OBJECT(window), "delete_event",
		   G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(window), "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *button = gtk_button_new_with_label("Click");
  g_signal_connect (button, "clicked",
		    G_CALLBACK (print_hello_world), NULL);

  
  entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(entry), "write here");

  GtkWidget *vbox = gtk_vbox_new (FALSE, 0);

  gtk_container_add (GTK_CONTAINER (window), vbox);
  //  gtk_widget_show (vbox);
  gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
  //  gtk_widget_show (entry);
  gtk_container_add (GTK_CONTAINER (vbox),button);

  //  gtk_widget_show (button);
  //gtk_widget_show_all (window);


  return window;
}

GtkWidget *create_top_window(GtkWidget *bottom_window) {
  return NULL;
}

static void bottom_window_movement(GtkWindow *window, 
				   GdkEvent *event, gpointer data) {

   int x = event->configure.x;
   int y = event->configure.y;
   g_print("Bottom window was moved: x=%d,y=%d\n", x, y);
   
}

static void bottom_window_enter(GtkWindow *window, 
				   GdkEvent *event, gpointer data) {

   g_print("Bottom window enter\n");
   
}

static void bottom_window_leave(GtkWindow *window, 
				   GdkEvent *event, gpointer data) {

   g_print("Bottom window leave\n");
   
}


void create_window_bottom_events(GtkWidget *bottom, GtkWidget *top) {

  // if (GTK_WIDGET_MAPPED(bottom))
  //   gtk_widget_unmap(bottom);
  gtk_widget_add_events(GTK_WIDGET(bottom), GDK_CONFIGURE);

  g_signal_connect(G_OBJECT(bottom), "configure-event",
		   G_CALLBACK(bottom_window_movement), NULL);

  gtk_widget_add_events(GTK_WIDGET(bottom), GDK_ENTER_NOTIFY | GDK_LEAVE_NOTIFY);

  g_signal_connect(G_OBJECT(bottom), "enter-notify-event",
		   G_CALLBACK(bottom_window_enter), NULL);

  g_signal_connect(G_OBJECT(bottom), "leave-notify-event",
		   G_CALLBACK(bottom_window_leave), NULL);

  /*  gtk_widget_show_all (bottom);*/

}

void create_window_top_events(GtkWidget *bottom, GtkWidget *top) {
  return;
}

int main(int argc, char **argv) {
  /* GtkWidget is the storage type for widgets */
  //  GtkWidget *windowbottom, *windowtop;

  if (argc < 2) {
    g_print("You need to pass the window id\n");
    return 1;
  }

  /*
   * This is called in all GTK applications. Arguments are parsed
   * from the command line and are returned to the application.
   */
  gtk_init (&argc, &argv);
  
  int parentbottomwindowid=parseint(argv[1]);
  GdkWindow *window_container = get_window_cotainer(parentbottomwindowid);

  GtkWidget *windowbottom = create_bottom_window();
  GtkWidget *windowtop = create_top_window(windowbottom);



  /* TODO review XEMBED and NPAPI hook where move notifications are received */
  create_window_bottom_events(windowbottom, windowtop);
  //create_window_top_events(windowtop, windowtop);
  //create_window_bottom_events(windowbottom, windowtop);

  /*  gtk_widget_show_all (windowtop);*/
  gtk_widget_show_all (windowbottom);
  
  gdk_window_reparent(GDK_WINDOW(windowbottom->window), window_container, 0, 0);



  gtk_main ();
    
  return 0;

}
