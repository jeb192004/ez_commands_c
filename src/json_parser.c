#include <gtk/gtk.h>
#include <json-glib/json-glib.h>

#include "build_command_list_item.h"
#include "replace.h"
#include "my_clipboard.h"

extern JsonNode *jsonArray;
extern GtkWidget *commands_container;
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

void build_list(void){

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
              build_list_item(id, name, command);


        } else {
            g_printerr("Array element %u is not an object\n", i);
        }
      }
    }else{g_error ("JSON IS NOT AN ARRAY");}
}

void run_command(gchar *buttonText){
  /*
  // get how long the text is, will never be bigger than this.
int txtlen = strlen(buttonText);

// Now lets allocate..
char *copy = (char *)malloc(txtlen+1);
if (copy == NULL) {
 // .. do usual error handling
}

// Now lets copy over the string.
strcpy(copy, buttonText);

// Print it out to see that it worked..
// replace this with what you use.
printf("Copy of buttonText: %s\n", copy);

// Declare stuff we need
char* command_needed = NULL;
char* command_id = NULL;
int new_id = 0;

//  char* token = strtok(buttonText, "_");
//  char* command_needed = token;
//  token = strtok(NULL, "_");
//  char* command_id = token;
//  double new_id = strtod(command_id, NULL);

command_needed = strtok(copy, "_");
command_id = strtok(NULL, "_");
new_id = atoi(command_id);
g_print ("command_needed: before loop:%s\n", command_needed);
free(copy);
copy = NULL;
*/
    gchar *gstr = buttonText;
    char str[strlen(gstr) + 1]; // allocate memory for C string
    strcpy(str, gstr); // copy gchar string to C string
    printf("C string: %s\n", str);

    char *pos = strstr(str, "_");
    char *substr, *aft_delimiter;
    double new_id;
    if (pos) {
        *pos = '\0'; // replace delimiter with null character
        aft_delimiter = pos + 1;
        new_id = atoi(aft_delimiter);
        substr = str; // substring starts from the beginning
        printf("First half of string: %s\n", aft_delimiter);
    } else {
        printf("Delimiter not found.\n");
    }

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
              if(strcmp(substr, "copy") ==0 ||strcmp(substr, "clone") == 0||strcmp(substr, "push") ==0||strcmp(substr, "pull") ==0){
                if (new_id == id) {
                  gchar *new_command;
                  if(strcmp(substr, "clone")==0){
                    new_command = replace (command, "git push", "git clone");
                    new_command = replace (command, "git pull", "git clone");
                    copy_text_to_clipboard (new_command);
                  }else if(strcmp(substr, "push")==0){
                    new_command = replace (command, "git clone", "git push");
                    new_command = replace (command, "git pull", "git push");
                    copy_text_to_clipboard (new_command);
                  }else if(strcmp(substr, "pull")==0){
                    new_command = replace (command, "git clone", "git pull");
                    new_command = replace (command, "git push", "git pull");
                    copy_text_to_clipboard (new_command);

                  }else if(strcmp(substr, "copy")==0){
                    copy_text_to_clipboard (command);
                  }
                }
              }else{g_print ("%s\n", "command needed doesn't match copy, clone, push, or pull");}

              //g_print("Element %u: ID=%g -> Name=%s -> COMMAND=%s \n", i, id, name, command);


        } else {
            g_printerr("Array element %u is not an object\n", i);
        }
      }
    }else{g_error ("JSON IS NOT AN ARRAY");}

}

guint get_json_size(void){
    JsonArray *array = json_node_get_array(jsonArray);
    guint length = json_array_get_length(array);

    g_print("Length of array: %u\n", length);
    return length;
}


void save_commands_file(const gchar* json_string) {
    gchar* filename = g_strdup_printf("%s/Documents/ez_commands.json", g_get_home_dir());
    GError* error = NULL;

    gboolean success = g_file_set_contents(filename, json_string, strlen(json_string), &error);
    if (!success) {
        g_print("Error saving file: %s\n", error->message);
        g_error_free(error);
    }
    g_free(filename);
}



void build_json_with_new_command(gchar *title, gchar *command){
  guint length = get_json_size();
  JsonNode *node1 = json_node_new (JSON_NODE_OBJECT);
  JsonObject *object1 = json_object_new();
  json_object_set_int_member(object1, "id", length+1);
  json_object_set_string_member(object1, "name", title);
  json_object_set_string_member (object1, "command", command);
  json_node_set_object(node1, object1);

  JsonArray *array = json_node_get_array (jsonArray);
  json_array_add_element(array, node1);

  // Convert the array to a JsonNode object
  JsonNode *node = json_node_new(JSON_NODE_ARRAY);
  json_node_set_array(node, array);

  // Print the JSON data
  char *json_string = json_to_string(node, FALSE);
  g_print("JSON array: %s\n", json_string);
  save_commands_file(json_string);
  build_list_item(length+1, title, command);
  // Free the memory used by the JsonNode and the string
  g_free(json_string);
  json_node_free(node);

}


