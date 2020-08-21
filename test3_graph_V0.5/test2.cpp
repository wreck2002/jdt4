
/**
 * 当计算jdt4时，第2个参数为10时，表示从文件中读取jdt4，并绘制出来。
 * 
 */

#include <iostream>
#include <cairo.h>
#include <gtk/gtk.h>
#include "test2_jdt3.h"
#include "test2_jdt4.h"


using namespace std;

double coordx[200];
double coordy[200];

int g_count = 0;
int cur_index = 3;
int g_option = 0;

static gboolean
on_expose_event(GtkWidget *widget,
                GdkEventExpose *event,
                gpointer data) {
  cairo_t *cr;
  int i, j;
  cr = gdk_cairo_create(widget->window);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 0.5);
  for ( i = 0; i < (g_count / 2); i++ ) {
    // for ( j  = 0; j <= count -1; j++ ) {
    cairo_move_to(cr, coordx[2 * i],     coordy[2 * i]);
    cairo_line_to(cr, coordx[2 * i + 1], coordy[2 * i + 1]);
    // }
  }
  g_count = 0;
  cairo_stroke(cr);
  cairo_destroy(cr);
  return FALSE;
}

gboolean clicked(GtkWidget *widget, GdkEventButton *event,
                 gpointer user_data) {
  std::cout << "click event: " << event->button << ", " <<
    cur_index << std::endl;
  if (event->button == 1) {
          // coordx[count] = event->x;
          // coordy[count++] = event->y;
    if (cur_index < (MAX_VALID_GRAPH_COUNT - 1)) {
      cur_index++;
    }
    update4(cur_index);
    gtk_widget_queue_draw(widget);
  }
  if (event->button == 3) {
    if (cur_index > 0) {
      cur_index--;
    }
    update4(cur_index);
    gtk_widget_queue_draw(widget);
  }
  return TRUE;
}


int main (int argc, char *argv[]) {
  g_count = 0;
  coordx[0] = 0.0;
  coordy[0] = 0.0;
  coordx[1] = 100.0;
  coordy[1] = 100.0;

  if (argc >= 2) {
    g_option = atoi(argv[1]);
  }

  cout << "argc = " << argc << endl;
  cout << "g_option = " << g_option << endl;

  // main_jdt3();
  main_jdt4();

  if (argc < 2 || g_option == 3) {
    return 0;
  }



  GtkWidget *window;
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        
        gtk_widget_add_events (window, GDK_BUTTON_PRESS_MASK);
        
        g_signal_connect(window, "expose-event",
                         G_CALLBACK(on_expose_event), NULL);
        g_signal_connect(window, "destroy",
                         G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(window, "button-press-event", 
                         G_CALLBACK(clicked), NULL);
        
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_title(GTK_WINDOW(window), "lines");
        gtk_window_set_default_size(GTK_WINDOW(window), 400, 300); 
        gtk_widget_set_app_paintable(window, TRUE);
        
        gtk_widget_show_all(window);
        
        gtk_main();
        
        return 0;
}




