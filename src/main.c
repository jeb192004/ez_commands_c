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
#include "layout.h"

#include "my_clipboard.h"
#include "json_parser.h"
#include "get_commands_file.h"
#include "build_command_list_item.h"
#include "dialogs.h"


JsonNode *jsonArray;
//GtkWidget *commands_container;
GtkWidget *window;



static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  //BUILD THE MAIN VIEW (LAYOUT.C FILE)
  window = build_main_view(app);
  //BUILD HEADER BAR (LAYOUT.C FILE)
  build_header_bar(window);
  // Use the system theme
  GtkSettings *settings = gtk_settings_get_default();
  g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

  //GET COMMANDS FILE
  const guint8 *data = get_commands_file("./Documents/ez_commands.json");
  if(data==0){
    g_print ("%s\n", "ez_command.json file does't exist");
    create_ez_commands_json();
  }else{
    //PARSE COMMANDS FILE AND RETURN JSON ARRAY
    jsonArray = parse_json((const gchar *)data);
    //LOAD LIST OF COMMANDS FROM JSON ARRAY, THEN LOAD LIST
    build_list(window);
  }

  gtk_window_present(GTK_WINDOW(window));
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

