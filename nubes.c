#include <cairo.h>
#include <gtk/gtk.h>

GMutex mutex_interface;
static cairo_surface_t *surface = NULL;
static int surface_width;
static int surface_height;

struct {
  gushort count;
  cairo_surface_t *image_nube1;  
  cairo_surface_t *image_nube2; 
  cairo_surface_t *image_globo1;
  cairo_surface_t *image_globo2;
  gint x_globo1;
  gint y_globo1;
  gint x_globo2;
  gint y_globo2;
} glob;

static gboolean drawing_area_configure_cb (GtkWidget *widget, GdkEventConfigure *event)
{
        if (event -> type == GDK_CONFIGURE)
        {
                if (surface != (cairo_surface_t *)NULL)
                {
                        cairo_surface_destroy (surface);
                }
                GtkAllocation allocation;
                gtk_widget_get_allocation (widget, &allocation);
                surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, allocation.width, allocation.height);
                surface_width = allocation.width;
                surface_height = allocation.height;
        }
}


static void do_drawing(cairo_t *);


static void do_drawing2(cairo_t *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{      
  do_drawing(cr);

  return FALSE;
}

static gboolean on_draw_event2(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{      
  do_drawing2(cr);
  return FALSE;
}

static void do_drawing(cairo_t *cr)
{
  g_mutex_lock(&mutex_interface);
  if(glob.y_globo1 <= glob.y_globo2 + 1 && glob.x_globo1 >= glob.x_globo2 - 80){
	  
	  glob.y_globo1 -= 1;
	  glob.x_globo1 += 1;
	  
  }
  else{
    if(glob.y_globo1 > 0){
        glob.y_globo1 -= 1;
    }else{
        glob.y_globo1 = 600;
    }

    if(glob.x_globo1 < 600){
        glob.x_globo1 += 1;
    }else{
        glob.x_globo1 = 0;
    }
  }
  cairo_set_source_surface(cr, glob.image_nube1, 10, 10);
  cairo_paint(cr);
  cairo_set_source_surface(cr, glob.image_nube2, 400, 100);
  cairo_paint(cr);  
  cairo_set_source_surface(cr, glob.image_globo1, glob.x_globo1, glob.y_globo1);
  cairo_paint(cr);  
  g_mutex_unlock(&mutex_interface);
}

static void do_drawing2(cairo_t *cr)
{
  g_mutex_lock(&mutex_interface);
  if(glob.y_globo2 >= glob.y_globo1 && glob.x_globo2 <= glob.x_globo1 + 125 && !(glob.x_globo2 < glob.x_globo1 - 10) ){
  }
  else{
    if(glob.y_globo2 > 0){
        glob.y_globo2 -= 1;
    }else{
        glob.y_globo2 = 600;
    }

    if(glob.x_globo2 > 0){
        glob.x_globo2 -= 1;
    }else{
        glob.x_globo2 = 600;
    }
  }  
  cairo_set_source_surface(cr, glob.image_globo2, glob.x_globo2, glob.y_globo2);
  cairo_paint(cr);
  g_mutex_unlock(&mutex_interface);
}

static gboolean time_handler(GtkWidget *widget)
{
  glob.count += 1;
  gtk_widget_queue_draw(widget);

  return TRUE;
}


int main(int argc, char *argv[])
{
  GtkWidget *ventana;
  GtkWidget *area_dibujo;
  glob.count = 0;
  glob.x_globo1 = 0;
  glob.y_globo1 = 600;
  
  
  glob.x_globo2 = 600;
  glob.y_globo2 = 600;
  
  glob.image_nube1 = cairo_image_surface_create_from_png("nube.png");
  glob.image_nube2 = cairo_image_surface_create_from_png("nube.png");
  glob.image_globo1 = cairo_image_surface_create_from_png("globo_peque.png");
  glob.image_globo2 = cairo_image_surface_create_from_png("globo_peque.png");

  gtk_init(&argc, &argv);

  ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  area_dibujo = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER (ventana), area_dibujo);

  g_signal_connect(G_OBJECT(area_dibujo), "draw",G_CALLBACK(on_draw_event), NULL); 
  g_signal_connect(G_OBJECT(area_dibujo), "draw",G_CALLBACK(on_draw_event2), NULL);
  g_signal_connect(G_OBJECT(area_dibujo), "configure-event", G_CALLBACK (drawing_area_configure_cb), NULL);
  g_signal_connect(ventana, "destroy",G_CALLBACK (gtk_main_quit), NULL);


  gtk_window_set_position(GTK_WINDOW(ventana), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(ventana), 600, 600); 
  gtk_window_set_title(GTK_WINDOW(ventana), "Nubes");
  g_timeout_add(15, (GSourceFunc) time_handler, (gpointer) ventana);

  gtk_widget_show_all(ventana);

  gtk_main();

  cairo_surface_destroy(glob.image_nube1);
  cairo_surface_destroy(glob.image_nube2);
  cairo_surface_destroy(glob.image_globo1);
  cairo_surface_destroy(glob.image_globo2);

  return 0;
} 
