#include <json-glib/json-glib.h>
#include <glib/gi18n.h>
#include <stdint.h>
#include <unistd.h>
#include <gtk/gtk.h>

extern JsonNode *jsonArray;
const guint8* get_commands_file(char *path_to_file) {
    GFile *file;
    GBytes *bytes;
    gsize size;

    GError *error = NULL;
    // Load file by name
    file = g_file_new_for_path("./Documents/ez_commands.json");
    char *file_path = g_file_get_path(file);
  if (access(file_path, F_OK) != -1) {
        g_print("The file path is: %s\n", file_path);
    bytes = g_file_load_bytes(file, NULL, NULL, NULL);
    if (error != NULL) {
        g_print("Error loading file: %s\n", error->message);
        g_error_free(error);
        //return;
    }
    const guint8 *data = g_bytes_get_data(bytes, &size);

    return data;

  // Cleanup
    g_bytes_unref(bytes);
    } else {

    }
return 0;
    g_object_unref(file);

}

void create_ez_commands_json(void){
  jsonArray = json_node_new(JSON_NODE_ARRAY);
  JsonArray* array = json_array_new();
  json_node_take_array(jsonArray, array);
  gchar* filename = g_strdup_printf("%s/Documents/ez_commands.json", g_get_home_dir());
    GError* error = NULL;
    gboolean success = g_file_set_contents(filename, "[]", -1, &error);
    if (!success) {
        g_print("Error saving file: %s\n", error->message);
        g_error_free(error);
    }else{
      g_print ("%s\n", "File Created!");
    }
    g_free(filename);
}
