
#include <gtk/gtk.h>



#include "json_parser.h"
#include "dialogs.h"


GtkWidget *commands_container;

void on_add_command_button_clicked(GtkButton *button, gpointer user_data) {
  GtkWidget *window = GTK_WIDGET(user_data);
  /* Get the text from the first entry */
  GtkEditable *entry1 = GTK_EDITABLE(g_object_get_data(G_OBJECT(user_data), "entry1"));
  GtkEditable *entry2 = GTK_EDITABLE(g_object_get_data(G_OBJECT(user_data), "entry2"));

  const char *text1 = gtk_editable_get_text(entry1);
  const char *text2 = gtk_editable_get_text(entry2);

  if (text1 == NULL || strlen(text1) == 0) {
        g_print("Title is empty or null.\n");
  }else if (text2 == NULL || strlen(text2) == 0) {
        g_print("Command is empty or null.\n");
  } else {
        build_json_with_new_command(text1, text2, window);
        gtk_editable_set_text (entry1, "");
        gtk_editable_set_text (entry2, "");
  }

}

GtkWidget* build_main_view(GtkApplication *app){
  GtkWidget *button, *button1;
  GtkWidget *main_box, *box;
  GtkWidget *title_entry, *command_entry;
  //CREATE THE WINDOW
  GtkWidget *window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Ez Commands");
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 400);


  main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_margin_top (main_box, 10);
  gtk_widget_set_margin_start (main_box, 10);
  gtk_widget_set_margin_bottom (main_box, 10);
  gtk_widget_set_margin_end (main_box, 10);
  //ADD THE MAIN CONTAINER TO THE WINDOW
  gtk_window_set_child(GTK_WINDOW(window), main_box);
/*
  //CREATE THE ENTRY FOR THE TITLE
  title_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(title_entry), "Title");
  gtk_widget_set_hexpand(GTK_WIDGET(title_entry), TRUE);
  gtk_widget_set_margin_bottom (title_entry, 5);
  //ADD THE TITLE ENTRY TO THE MAIN CONTAINER
  gtk_box_append (GTK_BOX(main_box), title_entry);
  //CREATE THE ENTRY FOR THE COMMAND
  command_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(command_entry), "Command");
  gtk_widget_set_hexpand(GTK_WIDGET(command_entry), TRUE);
  gtk_widget_set_margin_bottom (command_entry, 5);
  //ADD THE COMMAND ENTRY TO THE MAIN CONTAINER
  gtk_box_append (GTK_BOX(main_box),command_entry);
  //CREATE THE BUTTON TO ADD A NEW COMMAND
  button1 = gtk_button_new_with_label("Add New Command");
  gtk_widget_set_margin_bottom (button1, 5);
  //ADD THE "ADD COMMAND BUTTON TO THE MAIN  CONTAINER
  gtk_box_append (GTK_BOX(main_box), button1);

  // Connect the button to the on_button_clicked function
  g_signal_connect(button1, "clicked", G_CALLBACK(on_add_command_button_clicked), window);
  g_object_set_data(G_OBJECT(window), "entry1", title_entry);
  g_object_set_data(G_OBJECT(window), "entry2", command_entry);
*/
  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_append (GTK_BOX(main_box), box);
// Create a scrolled window to contain the box
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  //CONTAINER FOR ALL LIST ITEMS FOR COMMANDS
  commands_container = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_hexpand(commands_container, TRUE);
  gtk_widget_set_vexpand(commands_container, TRUE);
  gtk_widget_set_margin_top (commands_container, 10);
  gtk_widget_set_margin_bottom (commands_container, 10);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), commands_container);
  gtk_box_append (GTK_BOX(box), scrolled_window);

  return window;
}

static void on_open_add_command_dialog_button_clicked(GtkMenuButton *button, gpointer user_data){
  GtkWidget *window = (GtkWidget*)user_data;
  edit_dialog(window, "0000", window, "Add New Command");
}

static void on_popup_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Button clicked!\n");
    //gtk_popover_popup(GTK_POPOVER(user_data));
    //gtk_popover_popdown(GTK_POPOVER(user_data));
}


void build_header_bar(GtkWidget *window){
  // Create the header bar
GtkWidget *header_bar = gtk_header_bar_new();

  // Add a button to the right side of the header bar
  GtkWidget *menubutton = gtk_button_new();
  gtk_button_set_icon_name(GTK_BUTTON (menubutton), "emblem-system-symbolic");
  g_signal_connect(menubutton, "clicked", G_CALLBACK(on_popup_button_clicked), window);
  gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), menubutton);
  // Add a button to the left side of the header bar
  GtkWidget *add = gtk_button_new();
  gtk_button_set_icon_name(GTK_BUTTON (add), "value-increase-symbolic");
  g_signal_connect(add, "clicked", G_CALLBACK(on_open_add_command_dialog_button_clicked), window);
  gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), add);

// Set the header bar as the title bar for the window
gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

}

