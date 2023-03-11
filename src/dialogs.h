#ifndef DIALOGS_H
#define DIALOGS_H
#include <gtk/gtk.h>
//void my_function(gint my_variable);
void warning_dialog(GtkWidget *parent, char *message, double new_id);
void edit_dialog(GtkWidget *parent, double new_id, GtkWidget *window, char *message);
#endif /* DIALOGS_H */




