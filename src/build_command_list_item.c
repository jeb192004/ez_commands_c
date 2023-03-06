#include <gtk/gtk.h>

#include "json_parser.h"

extern GtkWidget *commands_container;

void command_button_clicked(GtkButton *button, gpointer user_data) {

    gchar* text = (gchar*)user_data;
    run_command (text);
}
void build_list_item(double id, gchar *name, gchar *command){

  //gtk_widget_set_halign (box2, GTK_ALIGN_CENTER);
  GtkWidget *label = gtk_label_new (name);
  gtk_widget_set_halign (label, GTK_ALIGN_START);
  gtk_widget_set_margin_bottom (label, 5);
  GtkGrid *button_grid = GTK_GRID(gtk_grid_new());
  gtk_grid_set_column_spacing(button_grid, 10);
  gtk_grid_set_row_spacing(button_grid, 10);
  GtkWidget *list_item_row_container = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_margin_top (list_item_row_container, 10);
  gtk_box_append (GTK_BOX (list_item_row_container), label);

  if (strstr(command, "git clone") != NULL || strstr(command, "git push") != NULL || strstr(command, "git pull") != NULL) {
      GtkWidget *clone = gtk_button_new_with_label ("Clone");
      GtkWidget *push = gtk_button_new_with_label ("Push");
      GtkWidget *pull = gtk_button_new_with_label ("Pull");
      char *clone_command = g_strdup_printf("%s%g", "clone_", id), *push_command = g_strdup_printf("%s%g", "push_", id), *pull_command = g_strdup_printf("%s%g", "pull_", id);

      //voi\ngtk_grid_attach(GtkGrid *grid, GtkWidget *child, int left, int top, int width, int height);
      gtk_grid_attach(GTK_GRID(button_grid), clone, 0, 0, 1, 1);
      gtk_grid_attach(GTK_GRID(button_grid), push, 1, 0, 1, 1);
      gtk_grid_attach(GTK_GRID(button_grid), pull, 2, 0, 1, 1);
        g_signal_connect(clone, "clicked", G_CALLBACK(command_button_clicked), clone_command);
        g_signal_connect(push, "clicked", G_CALLBACK(command_button_clicked), push_command);
        g_signal_connect(pull, "clicked", G_CALLBACK(command_button_clicked), pull_command);
  } else {
      gchar *copy_command  = g_strdup_printf("%s%g", "copy_", id);
      GtkWidget *copy = gtk_button_new_with_label ("Copy");
      g_signal_connect(copy, "clicked", G_CALLBACK(command_button_clicked), copy_command);
      gtk_grid_attach(GTK_GRID(button_grid), copy, 0, 0, 1, 1);
  }
  GtkWidget *edit_button = gtk_button_new_with_label ("Edit");
  gtk_grid_attach(GTK_GRID(button_grid), edit_button, 5, 0, 1, 1);
  GtkWidget *remove_button = gtk_button_new_with_label ("Remove");
  gtk_grid_attach(GTK_GRID(button_grid), remove_button, 6, 0, 1, 1);
  gtk_box_append (GTK_BOX (list_item_row_container), GTK_WIDGET(button_grid));
  // Create the separator
  GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_set_margin_top (separator, 5);
  gtk_widget_set_margin_top (separator, 5);
  gtk_box_append(GTK_BOX(list_item_row_container), separator);
  gtk_box_append(GTK_BOX(commands_container), list_item_row_container);
}


