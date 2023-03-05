#include <gtk/gtk.h>
#include <json-glib/json-glib.h>

#include "build_command_list_item.h"
#include "replace.h"
#include "my_clipboard.h"

extern JsonNode *jsonArray;
JsonNode* parse_json(const gchar* str) {

    gchar *json_string = str;
    JsonNode *root;
    JsonParser *parser;
    GError *error = NULL;
    parser = json_parser_new();
    if (!json_parser_load_from_data(parser, json_string, -1, &error)) {
        g_error("Unable to parse JSON string: %s", error->message);
        g_error_free(error);
        //return 1;
    }
  //ROOT IS JSON ARRAY.  I THINK...
  //RETURN ARRAY TO MAIN TO STORE FOR LATER USE.
  //CREATE NEW FUNCTION AND PASS JSON ARRAY(FROM MAIN.C) TO THAT FOR LOOPING THRU THE ARRAY AND BUILDING THE LIST OF COMMANDS
    root = json_parser_get_root(parser);
    return root;
}

void build_list(GtkWidget *box2){

  if (JSON_NODE_HOLDS_ARRAY(jsonArray)) {
        JsonArray *array = json_node_get_array(jsonArray);
        guint len = json_array_get_length(array);
        for (guint i = 0; i < len; i++) {
            JsonNode *node = json_array_get_element(array, i);
            if (JSON_NODE_HOLDS_OBJECT(node)) {
            JsonObject *object = json_node_get_object(node);
            JsonNode *name_node = json_object_get_member(object, "name");
              JsonNode *id_node = json_object_get_member(object, "id");
              JsonNode *command_node = json_object_get_member(object, "command");
              const gchar *name = json_node_get_string(name_node);
              const double id = json_node_get_double(id_node);
              const gchar *command = json_node_get_string(command_node);
              //g_print("Element %u: ID=%g -> Name=%s -> COMMAND=%s \n", i, id, name, command);
              build_list_item(box2, id, name, command);


        } else {
            g_printerr("Array element %u is not an object\n", i);
        }
      }
    }else{g_error ("JSON IS NOT AN ARRAY");}
}

void run_command(gchar *buttonText){
  char* token = strtok(buttonText, "_");
  char* command_needed = token;
  token = strtok(NULL, "_");
  char* command_id = token;
  double new_id = strtod(command_id, NULL);
  if (JSON_NODE_HOLDS_ARRAY(jsonArray)) {
        JsonArray *array = json_node_get_array(jsonArray);
        guint len = json_array_get_length(array);
        for (guint i = 0; i < len; i++) {
            JsonNode *node = json_array_get_element(array, i);
            if (JSON_NODE_HOLDS_OBJECT(node)) {
            JsonObject *object = json_node_get_object(node);
            JsonNode *name_node = json_object_get_member(object, "name");
              JsonNode *id_node = json_object_get_member(object, "id");
              JsonNode *command_node = json_object_get_member(object, "command");
              const gchar *name = json_node_get_string(name_node);
              const double id = json_node_get_double(id_node);
              const gchar *command = json_node_get_string(command_node);
              if(strcmp(command_needed, "copy") ==0 ||strcmp(command_needed, "clone") ==0||strcmp(command_needed, "push") ==0||strcmp(command_needed, "pull") ==0){
                if (new_id == id) {
                  gchar *new_command;
                  g_print("\n%s\n", command_needed);
                  if(strcmp(command_needed, "clone")==0){
                    new_command = replace (command, "git push", "git clone");
                    new_command = replace (command, "git pull", "git clone");
                    copy_text_to_clipboard (new_command);
                  }else if(strcmp(command_needed, "push")==0){
                    new_command = replace (command, "git clone", "git push");
                    new_command = replace (command, "git pull", "git push");
                    copy_text_to_clipboard (new_command);
                  }else if(strcmp(command_needed, "pull")==0){
                    new_command = replace (command, "git clone", "git pull");
                    new_command = replace (command, "git push", "git pull");
                    copy_text_to_clipboard (new_command);
                  }
                }
              }

              //g_print("Element %u: ID=%g -> Name=%s -> COMMAND=%s \n", i, id, name, command);


        } else {
            g_printerr("Array element %u is not an object\n", i);
        }
      }
    }else{g_error ("JSON IS NOT AN ARRAY");}
}

void build_json_with_new_command(gchar *title, gchar *command){
JsonNode *node1 = json_node_new (JSON_NODE_OBJECT);
JsonObject *object1 = json_object_new();
json_object_set_int_member(object1, "age", 42);
json_object_set_string_member(object1, "name", "John");
json_node_set_object(node1, object1);

JsonNode *node2 = json_node_new (JSON_NODE_OBJECT);
JsonObject *object2 = json_object_new();
json_object_set_int_member(object2, "age", 27);
json_object_set_string_member(object2, "name", "Jane");
json_node_set_object(node2, object2);

JsonArray *array = json_array_new();
json_array_add_element(array, node1);
json_array_add_element(array, node2);

  // Convert the array to a JsonNode object
JsonNode *node = json_node_new(JSON_NODE_ARRAY);
json_node_set_array(node, array);
// Print the JSON data
char *json_string = json_to_string(node, FALSE);
g_print("JSON array: %s\n", json_string);

  // Free the memory used by the JsonNode and the string
g_free(json_string);
json_node_free(node);

}

void get_json_size(void){
    //JsonArray array = json_node_get_array(jsonArray);
    //guint length = json_array_get_length(array);

    //g_print("Length of array: %u\n", length);

}
