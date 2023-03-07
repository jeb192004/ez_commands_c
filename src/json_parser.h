#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <glib.h>
#include <json-glib/json-glib.h>
//void my_function(gint my_variable);
JsonNode* parse_json(const gchar* text);
void build_list(void);
void run_command(gchar *buttonId);
void build_json_with_new_command(gchar *title, gchar *command);
guint get_json_size(void);
void remove_command_from_json(double id);
#endif /* JSON_PARSER_H */









