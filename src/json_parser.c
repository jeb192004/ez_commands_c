#include <gtk/gtk.h>
#include <json-glib/json-glib.h>
#include <time.h>


//MY LOCAL FILES TO INCLUDE
#include "build_command_list_item.h"
#include "replace.h"
#include "my_clipboard.h"

extern JsonNode *jsonArray;
extern GtkWidget *commands_container;

gchar* generate_new_id(void){
  gchar *new_id;

  time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char id[32];
    strftime(id, sizeof(id), "%Y%m%d%H%M%S", t);
    new_id = g_strdup_printf("%s", id);

  return new_id;
}
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



void build_list(GtkWidget *window){

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
              const gchar *id = json_node_get_string(id_node);
              const gchar *command = json_node_get_string(command_node);
              //g_print("Element %u: ID=%s -> Name=%s -> COMMAND=%s \n", i, id, name, command);
              build_list_item(id, name, command, window);


        } else {
            g_printerr("Array element %u is not an object\n", i);
        }
      }
    }else{g_error ("JSON IS NOT AN ARRAY");}
}

void run_command(gchar *buttonText){

    gchar *gstr = buttonText;
    char str[strlen(gstr) + 1]; // allocate memory for C string
    strcpy(str, gstr); // copy gchar string to C string
    printf("C string: %s\n", str);

    char *pos = strstr(str, "_");
    char *substr, *aft_delimiter;
    //double new_id;
    if (pos) {
        *pos = '\0'; // replace delimiter with null character
        aft_delimiter = pos + 1;
        //new_id = atoi(aft_delimiter);
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
              const gchar *id = json_node_get_string(id_node);
              const gchar *command = json_node_get_string(command_node);
              if(strcmp(substr, "copy") ==0 ||strcmp(substr, "clone") == 0||strcmp(substr, "push") ==0||strcmp(substr, "pull") ==0){
                if (strcmp(id, aft_delimiter)==0) {
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



void build_json_with_new_command(gchar *title, gchar *command, GtkWidget *window){
  gchar *id = generate_new_id();
  JsonNode *node1 = json_node_new (JSON_NODE_OBJECT);
  JsonObject *object1 = json_object_new();
  json_object_set_string_member(object1, "id", id);
  json_object_set_string_member(object1, "name", title);
  json_object_set_string_member (object1, "command", command);
  json_node_set_object(node1, object1);

  JsonArray *array = json_node_get_array (jsonArray);
  json_array_add_element(array, node1);

  // Convert the array to a JsonNode object
  JsonNode *node = json_node_new(JSON_NODE_ARRAY);
  json_node_set_array(node, array);
  jsonArray = node;
  // Print the JSON data
  char *json_string = json_to_string(node, FALSE);
  g_print("JSON array: %s\n", json_string);
  save_commands_file(json_string);
  build_list_item(id, title, command, window);
  // Free the memory used by the JsonNode and the string
  g_free(json_string);
  json_node_free(node);

}


void remove_command_from_json(gchar *id_to_remove){

  JsonArray *new_array = json_array_new ();

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
              const gchar *id = json_node_get_string(id_node);
              const gchar *command = json_node_get_string(command_node);
              if(strcmp(id, id_to_remove)!=0){

                JsonNode *node1 = json_node_new (JSON_NODE_OBJECT);
              JsonObject *object1 = json_object_new();
              json_object_set_string_member(object1, "id", id);
              json_object_set_string_member(object1, "name", name);
              json_object_set_string_member (object1, "command", command);
              json_node_set_object(node1, object1);
              json_array_add_element(new_array, node1);
              }




        } else {
            g_printerr("Array element %u is not an object\n", i);
        }
      }
      // Convert the array to a JsonNode object
      JsonNode *node = json_node_new(JSON_NODE_ARRAY);
      json_node_set_array(node, new_array);
      jsonArray = node;
      // Print the JSON data
      char *json_string = json_to_string(node, FALSE);
      //g_print("JSON array: %s\n", json_string);
      save_commands_file (json_string);
    }else{g_error ("JSON IS NOT AN ARRAY");}

}

void edit_json_with_new_command(gchar *new_title, gchar *new_command, gchar *new_id, GtkWidget *window, bool isNewCommand){
  g_print ("\n%s: %s: %s: %s\n", "edit_json_with_new_command", new_id, new_title, new_command);
  JsonArray *new_array = json_array_new ();

  if (JSON_NODE_HOLDS_ARRAY(jsonArray)) {
        JsonArray *array = json_node_get_array(jsonArray);
        guint len = json_array_get_length(array);
        for (guint i = 0; i < len; i++) {
          g_print ("%d\n", i);
            JsonNode *node = json_array_get_element(array, i);
            if (JSON_NODE_HOLDS_OBJECT(node)) {
            JsonObject *object = json_node_get_object(node);
            JsonNode *name_node = json_object_get_member(object, "name");
              JsonNode *id_node = json_object_get_member(object, "id");
              JsonNode *command_node = json_object_get_member(object, "command");
              const gchar *name = json_node_get_string(name_node);
              const gchar *id = json_node_get_string(id_node);
              const gchar *command = json_node_get_string(command_node);
              g_print("%s: %s\n", id, name);
              if(strcmp(id, new_id)!=0){
                g_print("%s: %s %s\n","add old command", id, new_id);
                JsonNode *node1 = json_node_new (JSON_NODE_OBJECT);
              JsonObject *object1 = json_object_new();
              json_object_set_string_member(object1, "id", id);
              json_object_set_string_member(object1, "name", name);
              json_object_set_string_member (object1, "command", command);
              json_node_set_object(node1, object1);
              json_array_add_element(new_array, node1);
              }else if(strcmp(id, new_id)==0){
                g_print("%s: %s %s\n","add new command", id, new_id);
                JsonNode *node1 = json_node_new (JSON_NODE_OBJECT);
              JsonObject *object1 = json_object_new();
              json_object_set_string_member(object1, "id", id);
              json_object_set_string_member(object1, "name", new_title);
              json_object_set_string_member (object1, "command", new_command);
              json_node_set_object(node1, object1);
              json_array_add_element(new_array, node1);
              }




        } else {
            g_printerr("Array element %u is not an object\n", i);
        }
      }
      if(isNewCommand){
        JsonNode *node1 = json_node_new (JSON_NODE_OBJECT);
        JsonObject *object1 = json_object_new();
        json_object_set_string_member(object1, "id", new_id);
        json_object_set_string_member(object1, "name", new_title);
        json_object_set_string_member (object1, "command", new_command);
        json_node_set_object(node1, object1);
        json_array_add_element(new_array, node1);
      }
      // Convert the array to a JsonNode object
      JsonNode *node = json_node_new(JSON_NODE_ARRAY);
      json_node_set_array(node, new_array);
      jsonArray = node;
      // Print the JSON data
      char *json_string = json_to_string(node, FALSE);
      g_print("JSON array: %s\n", json_string);
      save_commands_file (json_string);
      build_list_item(new_id, new_title, new_command, window);
    }else{g_error ("JSON IS NOT AN ARRAY");}

}




