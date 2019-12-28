/*
 * main.c
 *
 *  Created on: Oct 16, 2019
 *      Author: artur
 */

#include "main.h"
#include "fatfs.h"

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <zx_keyscans.h>
#include "mainfnc.h"
#include "memory.h"
#include "zx_display.h"
#include "kbd_driver.h"

//char* utoa(unsigned int n, char s[], unsigned int rad)
//{
//	int i, j;
//
//	i = 0;
//	do {       /* генерируем цифры в обратном порядке */
//		s[i] = n % rad + '0';   /* берем следующую цифру */
//		if(s[i] > '9') s[i] = s[i]-'9'-1+'a';
//		i++;
//	} while ((n /= rad) > 0);     /* удаляем */
//	s[i] = '\0';
//
//	char c;
//	/*реверсируем цифры в строке*/
//	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
//		c = s[i];
//		s[i] = s[j];
//		s[j] = c;
//	}
//
//	return s;
//}

gpointer run_thread(gpointer image) {
	MX_FATFS_Init();
	main_init();

	while (1) {
		main_loop();
	}

	return NULL;
}

gboolean process_newline(void *image) {

	TIM3_IRQ_HANDLER();
	if(zxlnum >= ZX_LINES) {
		gtk_widget_queue_draw((GtkWidget*)image);
	}

	return TRUE;
}

gpointer timer_thread(gpointer image) {
	while(1) {
		struct timespec tw = {0,325520};
		struct timespec tr;
		nanosleep (&tw, &tr);
		process_newline(image);
	}

	return NULL;
}

static void Run (GtkImage *image)
{
	GError* err = NULL;

	if (!g_thread_try_new("run_thread", run_thread, (gpointer) image, &err))
	{
		g_printerr ("Failed to create Run thread: %s\n", err->message);
	}

	if (!g_thread_try_new("timer_thread", timer_thread, (gpointer) image, &err))
	{
		g_printerr ("Failed to create timer thread: %s\n", err->message);
	}
}

static void which_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
//	if(event->type == GDK_KEY_PRESS)
//		g_printerr ("Key pressed: %x %x\n", event->keyval, event->hardware_keycode);
//	else
//		g_printerr ("Key released: %x %x\n", event->keyval, event->hardware_keycode);

	if(event->keyval == GDK_KEY_Return) {
		if(event->type == GDK_KEY_PRESS)
			ext_kbdscans[6] |= 0x01;
		else
			ext_kbdscans[6] &= ~0x01;
	}
	if(event->keyval == GDK_KEY_space) {
		if(event->type == GDK_KEY_PRESS)
			ext_kbdscans[7] |= 0x01;
		else
			ext_kbdscans[7] &= ~0x01;
	}
	if(event->hardware_keycode >= 0x0a && event->hardware_keycode <= 0x13) {
		if(event->type == GDK_KEY_PRESS)
			ext_kbdscans[zx_keyscans[event->hardware_keycode - 0x0a][ZX_KEYSCAN_ADDR]] |= zx_keyscans[event->hardware_keycode - 0x0a][ZX_KEYSCAN_BIT];
		else
			ext_kbdscans[zx_keyscans[event->hardware_keycode - 0x0a][ZX_KEYSCAN_ADDR]] &= ~zx_keyscans[event->hardware_keycode - 0x0a][ZX_KEYSCAN_BIT];
	}
	if(event->keyval >= GDK_KEY_A && event->keyval <= GDK_KEY_Z) {
		if(event->type == GDK_KEY_PRESS)
			ext_kbdscans[zx_keyscans[event->keyval - GDK_KEY_A + 10][ZX_KEYSCAN_ADDR]] |= zx_keyscans[event->keyval - GDK_KEY_A + 10][ZX_KEYSCAN_BIT];
		else
			ext_kbdscans[zx_keyscans[event->keyval - GDK_KEY_A + 10][ZX_KEYSCAN_ADDR]] &= ~zx_keyscans[event->keyval - GDK_KEY_A + 10][ZX_KEYSCAN_BIT];
	}
	if(event->keyval >= GDK_KEY_a && event->keyval <= GDK_KEY_z) {
		if(event->type == GDK_KEY_PRESS)
			ext_kbdscans[zx_keyscans[event->keyval - GDK_KEY_a + 10][ZX_KEYSCAN_ADDR]] |= zx_keyscans[event->keyval - GDK_KEY_a + 10][ZX_KEYSCAN_BIT];
		else
			ext_kbdscans[zx_keyscans[event->keyval - GDK_KEY_a + 10][ZX_KEYSCAN_ADDR]] &= ~zx_keyscans[event->keyval - GDK_KEY_a + 10][ZX_KEYSCAN_BIT];
	}

	if(event->keyval == GDK_KEY_Shift_L) {
		if(event->type == GDK_KEY_PRESS)
			ext_kbdscans[0] |= 0x01;
		else
			ext_kbdscans[0] &= ~0x01;
	}
	if(event->keyval == GDK_KEY_Shift_R) {
		if(event->type == GDK_KEY_PRESS)
			ext_kbdscans[7] |= 0x02;
		else
			ext_kbdscans[7] &= ~0x02;
	}
}

static gboolean draw_image(GtkWidget *image, cairo_t *context, gpointer data)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, ILI9341_PWIDTH, ILI9341_PHEIGHT);
	uint8_t *pixels = gdk_pixbuf_get_pixels(pixbuf);
	uint8_t *pi, *po;

	for(int i=0;i<ILI9341_PHEIGHT;i++)
		for(int j=0;j<ILI9341_PWIDTH;j++) {
			pi = ili9341_image + (i*ILI9341_PWIDTH+j)*3;
			po = pixels        + (i*ILI9341_PWIDTH+j)*3;
			po[0] = pi[0];
			po[1] = pi[1];
			po[2] = pi[2];
		}

	gtk_image_set_from_pixbuf((GtkImage*)image, pixbuf);

	g_object_unref(pixbuf);

	return FALSE;
}

int main (int argc, char *argv[])
{
	GtkWidget *win = NULL;
	GtkWidget *image = NULL;

	/* Initialize GTK+ */
	gtk_init (&argc, &argv);

	/* Create the main window */
	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width ((GtkContainer*)win, 10);
	gtk_window_set_title ((GtkWindow*)win, "Screen");
	gtk_window_set_position ((GtkWindow*)win, GTK_WIN_POS_CENTER);
	gtk_window_set_default_size ( (GtkWindow*)win, ILI9341_PWIDTH+20, ILI9341_PHEIGHT+20);
	gtk_widget_realize (win);
	g_signal_connect (win, "destroy", gtk_main_quit, NULL);

	image = gtk_image_new();
	gtk_widget_set_size_request(image, ILI9341_PWIDTH, ILI9341_PHEIGHT);
	g_signal_connect(G_OBJECT(image), "draw", G_CALLBACK(draw_image), NULL);
	gtk_container_add ((GtkContainer*)win, image);

	ILI9341_Init();

	g_signal_connect (G_OBJECT (win), "key_press_event", G_CALLBACK (which_key_press), NULL);
	g_signal_connect (G_OBJECT (win), "key_release_event", G_CALLBACK (which_key_press), G_OBJECT (win));

	Run((gpointer) image);

	/* Enter the main loop */
	gtk_widget_show_all (win);

	gtk_main ();


	return 0;
}
