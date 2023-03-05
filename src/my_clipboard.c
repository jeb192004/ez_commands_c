#include <gtk/gtk.h>


void copy_text_to_clipboard(gchar *text){
  GdkDisplay *display = gdk_display_get_default ();
  GdkClipboard *clipboard = gdk_display_get_clipboard (display);
  gdk_clipboard_set_text (clipboard, text);
}

