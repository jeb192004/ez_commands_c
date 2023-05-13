#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <glib.h>
#include <json-glib/json-glib.h>
#include <gtk/gtk.h>
//void my_function(gint my_variable);
JsonNode* parse_json(const gchar* text);
void build_list(GtkWidget *window);
void run_command(gchar *buttonId);
void build_json_with_new_command(gchar *title, gchar *command, GtkWidget *window);
guint get_json_size(void);
void remove_command_from_json(gchar *id);
void edit_json_with_new_command(gchar *new_title, gchar *new_command, gchar *new_id, GtkWidget *window, bool isNewCommand);
gchar* generate_new_id(void);
#endif /* JSON_PARSER_H */

















