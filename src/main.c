/* main.c
 *
 * Copyright 2023 James Burnison
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "config.h"
#include <json-glib/json-glib.h>
#include <glib/gi18n.h>
#include <stdint.h>
#include <gtk/gtk.h>

//MY FUNCTIONS IN OTHER FILES
#include "my_clipboard.h"
#include "json_parser.h"
#include "get_commands_file.h"

JsonNode *jsonArray;
GtkWidget *commands_container;

void on_button_clicked(GtkButton *button, gpointer user_data) {
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
        build_json_with_new_command(text1, text2);
        //get_json_size();
  }

}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{

  GtkWidget *button, *button1;
  GtkWidget *main_box, *box;
  GtkWidget *title_entry, *command_entry;

  //CREATE THE WINDOW
  GtkWidget *window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Ez Commands");
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 400);

  //CREATE THE MAIN CONTAINER
  main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_margin_top (main_box, 10);
  gtk_widget_set_margin_start (main_box, 10);
  gtk_widget_set_margin_bottom (main_box, 10);
  gtk_widget_set_margin_end (main_box, 10);
  //ADD THE MAIN CONTAINER TO THE WINDOW
  gtk_window_set_child(GTK_WINDOW(window), main_box);

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
  g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), window);
  g_object_set_data(G_OBJECT(window), "entry1", title_entry);
  g_object_set_data(G_OBJECT(window), "entry2", command_entry);

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  //gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
  //gtk_widget_set_valign (box, GTK_ALIGN_CENTER);

  gtk_box_append (GTK_BOX(main_box), box);

  //g_signal_connect (button, "clicked", G_CALLBACK (load_commands_file), NULL);

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
  //GET COMMANDS FILE
  const guint8 *data = get_commands_file("./Documents/ez_commands.json");
  //PARSE COMMANDS FILE AND RETURN JSON ARRAY
  jsonArray = parse_json((const gchar *)data);
  //LOAD LIST OF COMMANDS FROM JSON ARRAY, THEN LOAD LIST
  build_list();

  gtk_widget_show (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("com.jeb.ez_commands", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
