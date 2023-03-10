#include <gtk/gtk.h>

#include "json_parser.h"
#include "dialogs.h"

extern GtkWidget *commands_container;

void command_button_clicked(GtkButton *button, gpointer user_data) {

    gchar* text = (gchar*)user_data;
    run_command (text);
}

void remove_command(GtkWidget *button, gpointer user_data){

  gchar* text = (gchar*)user_data;
  gchar *gstr = text;
    char str[strlen(gstr) + 1]; // allocate memory for C string
    strcpy(str, gstr); // copy gchar string to C string
    printf("C string: %s\n", str);

    char *pos = strstr(str, "_");
    char *aft_delimiter;
    //double new_id;
    if (pos) {
        *pos = '\0'; // replace delimiter with null character
        aft_delimiter = pos + 1;
        //new_id = atoi(aft_delimiter);
        printf("First half of string: %s\n", aft_delimiter);
    } else {
        printf("Delimiter not found.\n");
    }
  GtkWidget *parent_container = gtk_widget_get_parent(button);
  GtkWidget *parents_parent_container = gtk_widget_get_parent(parent_container);
  GtkWidget *container_to_remove = gtk_widget_get_parent(parents_parent_container);
    warning_dialog(container_to_remove, "Are you sure you want to remove that command?\n\nThis can not be undone.", aft_delimiter);

}

typedef struct {
    gchar *id;
    GtkWidget *WindowWidget;
} editValues;

void edit_command(GtkWidget *button, gpointer user_data){
  editValues *valuesEdit = user_data;

  gchar* text = (gchar*)valuesEdit->id;
  gchar *gstr = text;
    char str[strlen(gstr) + 1]; // allocate memory for C string
    strcpy(str, gstr); // copy gchar string to C string
    printf("C string: %s\n", str);

    char *pos = strstr(str, "_");
    char *aft_delimiter;
    double new_id;
    if (pos) {
        *pos = '\0'; // replace delimiter with null character
        aft_delimiter = pos + 1;
        //new_id = atoi(aft_delimiter);
        //printf("First half of string: %s\n", aft_delimiter);
    } else {
        //printf("Delimiter not found.\n");
    }
  GtkWidget *parent_container = gtk_widget_get_parent(button);
  GtkWidget *parents_parent_container = gtk_widget_get_parent(parent_container);
  GtkWidget *container_to_remove = gtk_widget_get_parent(parents_parent_container);
  edit_dialog(container_to_remove, aft_delimiter, valuesEdit->WindowWidget, "Edit Command");

}


void build_list_item(gchar *aft_delimiter, gchar *name, gchar *command, GtkWidget *window){

  //gtk_widget_set_halign (box2, GTK_ALIGN_CENTER);
  GtkWidget *label = gtk_label_new (name);
  gtk_widget_set_halign (label, GTK_ALIGN_START);
  gtk_widget_set_margin_bottom (label, 5);
  GtkGrid *button_grid = GTK_GRID(gtk_grid_new());
  gtk_grid_set_column_spacing(button_grid, 10);
  gtk_grid_set_row_spacing(button_grid, 10);
  GtkWidget *button_grid2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand (button_grid2, TRUE);
  //gtk_grid_set_column_spacing(button_grid2, 10);
  //gtk_grid_set_row_spacing(button_grid2, 10);
  GtkWidget *list_item_row_container = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_margin_top (list_item_row_container, 10);
  gtk_box_append (GTK_BOX (list_item_row_container), label);
  GtkWidget *fixed_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_hexpand (fixed_container, TRUE);

  if (strstr(command, "git clone") != NULL || strstr(command, "git push") != NULL || strstr(command, "git pull") != NULL) {
      GtkWidget *clone = gtk_button_new_with_label ("Clone");
      GtkWidget *push = gtk_button_new_with_label ("Push");
      GtkWidget *pull = gtk_button_new_with_label ("Pull");
      char *clone_command = g_strdup_printf("%s%s", "clone_", aft_delimiter), *push_command = g_strdup_printf("%s%s", "push_", aft_delimiter), *pull_command = g_strdup_printf("%s%s", "pull_", aft_delimiter);

      //voi\ngtk_grid_attach(GtkGrid *grid, GtkWidget *child, int left, int top, int width, int height);
      gtk_grid_attach(GTK_GRID(button_grid), clone, 0, 0, 1, 1);
      gtk_grid_attach(GTK_GRID(button_grid), push, 1, 0, 1, 1);
      gtk_grid_attach(GTK_GRID(button_grid), pull, 2, 0, 1, 1);
        g_signal_connect(clone, "clicked", G_CALLBACK(command_button_clicked), clone_command);
        g_signal_connect(push, "clicked", G_CALLBACK(command_button_clicked), push_command);
        g_signal_connect(pull, "clicked", G_CALLBACK(command_button_clicked), pull_command);
  } else {
      gchar *copy_command  = g_strdup_printf("%s%s", "copy_", aft_delimiter);
      GtkWidget *copy = gtk_button_new_with_label ("Copy");
      g_signal_connect(copy, "clicked", G_CALLBACK(command_button_clicked), copy_command);
      gtk_grid_attach(GTK_GRID(button_grid), copy, 0, 0, 1, 1);
  }

  gtk_box_append(GTK_BOX(fixed_container), GTK_WIDGET(button_grid));

  gchar *edit_command_id  = g_strdup_printf("%s%s", "edit_", aft_delimiter);

  editValues *valuesEdit = g_new(editValues, 1);
    valuesEdit->id = edit_command_id;
    valuesEdit->WindowWidget = window;
  GtkWidget *edit_button = gtk_button_new_with_label ("Edit");
  g_signal_connect(edit_button, "clicked", G_CALLBACK(edit_command), valuesEdit);
  gtk_box_append(GTK_BOX(button_grid2), edit_button);

  GtkWidget *remove_button = gtk_button_new_with_label ("Remove");
GtkStyleContext *context;
  //gtk_style_context_set_back
  context = gtk_widget_get_style_context (GTK_WIDGET (remove_button));
  // Add a class to the button
  gtk_widget_add_css_class(GTK_WIDGET(remove_button), "remove_button");
  GtkCssProvider *provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_data (provider, ".remove_button {color: red; font-size: 12pt;} button{padding:10px;}", -1);
  gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_box_append(GTK_BOX(button_grid2), remove_button);
  gchar *remove_command_id  = g_strdup_printf("%s%s", "remove_", aft_delimiter);
  g_signal_connect(remove_button, "clicked", G_CALLBACK(remove_command), remove_command_id);
  gtk_box_append(GTK_BOX(fixed_container), GTK_WIDGET(button_grid2));
  gtk_widget_set_halign(GTK_WIDGET(button_grid2), GTK_ALIGN_END);

  gtk_box_append (GTK_BOX(list_item_row_container), GTK_WIDGET(fixed_container));
  // Create the separator
  GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_set_margin_top (separator, 5);
  gtk_widget_set_margin_top (separator, 5);
  gtk_box_append(GTK_BOX(list_item_row_container), separator);
  gtk_box_append(GTK_BOX(commands_container), list_item_row_container);
}



