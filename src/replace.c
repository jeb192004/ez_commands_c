#include <gtk/gtk.h>

gchar* replace(gchar* text, const gchar* old_str, const gchar* new_str) {
    int old_len = strlen(old_str);
    int new_len = strlen(new_str);
    gchar* pos = strstr(text, old_str);
    while (pos != NULL) {
        memmove(pos + new_len, pos + old_len, strlen(pos + old_len) + 1);
        memcpy(pos, new_str, new_len);
        pos = strstr(pos + new_len, old_str);
    }

    return text;
}
