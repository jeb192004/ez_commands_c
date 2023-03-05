#include <json-glib/json-glib.h>
#include <glib/gi18n.h>
#include <stdint.h>

const guint8* get_commands_file(char *path_to_file) {
    GFile *file;
    GBytes *bytes;
    gsize size;

    GError *error = NULL;
    // Load file by name
    file = g_file_new_for_path("./Documents/ez_commands.json");
    char *file_path = g_file_get_path(file);
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
    g_object_unref(file);

}

