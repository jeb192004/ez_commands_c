#include <gtk/gtk.h>

#include "json_parser.h"
// Function to open a dialog box with a message
extern GtkWidget *window;

typedef struct {
    GtkWidget *value1;
    double value2;

} MyValues;

void ok_button_clicked(GtkButton *button, gint response_id, gpointer user_data) {
  MyValues *values = user_data;

  GtkWidget *parent = values->value1;
  if (response_id == GTK_RESPONSE_OK) {
  if (parent != NULL) {
      g_print("%s", "parent exist");
      remove_command_from_json(values->value2);
      gtk_widget_unparent(parent);

  }
    } else if (response_id == GTK_RESPONSE_CANCEL) {
        g_print("Cancel button clicked\n");
    }

}

void warning_dialog (GtkWidget *parent, char *message, double new_id){
 GtkWidget *dialog, *label, *content_area;
 GtkDialogFlags flags;

 // Create the widgets
 flags = GTK_DIALOG_DESTROY_WITH_PARENT;
 dialog = gtk_dialog_new_with_buttons ("Warning",
                                       GTK_WINDOW (window),
                                       flags,
                                       "OK",
                                       GTK_RESPONSE_OK,
                                       "Cancel",
                                       GTK_RESPONSE_CANCEL,
                                       NULL);

    MyValues *values = g_new(MyValues, 1);
    values->value1 = parent;
    values->value2 = new_id;

  // Connect the "response" signal to the callback function
    g_signal_connect(dialog, "response", G_CALLBACK(ok_button_clicked), values);

 content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
 label = gtk_label_new (message);

  gtk_widget_set_margin_top (content_area, 10);
  gtk_widget_set_margin_start (content_area, 10);
  gtk_widget_set_margin_end (content_area, 10);
  gtk_widget_set_margin_bottom (content_area, 10);


 // Ensure that the dialog box is destroyed when the user responds

 g_signal_connect_swapped (dialog,
                           "response",
                           G_CALLBACK (gtk_window_destroy),
                           dialog);

 // Add the label, and show everything we’ve added

 gtk_box_append (GTK_BOX (content_area), label);
 gtk_widget_show (dialog);
}


typedef struct {
    GtkWidget *value1;
    double value2;
    GtkWidget *TitleEntry;
    GtkWidget *CommandEntry;
    GtkWidget *WindowWidget;
} editValues;

void edit_button_clicked(GtkButton *button, gint response_id, gpointer user_data) {
  editValues *valuesEdit = user_data;

  GtkWidget *parent = valuesEdit->value1;
  if (response_id == GTK_RESPONSE_OK) {
  if (parent != NULL) {
      g_print("%s", "parent exist");
      const char *text1 = gtk_editable_get_text(GTK_EDITABLE(valuesEdit->TitleEntry));
      const char *text2 = gtk_editable_get_text(GTK_EDITABLE(valuesEdit->CommandEntry));

      if (text1 == NULL || strlen(text1) == 0) {
        g_print("Title is empty or null.\n");
      }else if (text2 == NULL || strlen(text2) == 0) {
        g_print("Command is empty or null.\n");
      } else {
        double id = valuesEdit->value2;
        double id_new;
        if(id == 0000){
          id_new = generate_new_id();
        }else{
          id_new = id;
        }
        g_print ("%f\n", id_new);
        //edit_json_with_new_command(text1, text2, id_new, valuesEdit->WindowWidget);
        //gtk_widget_unparent(parent);
      }

      //remove_command_from_json(valuesEdit->value2);


  }
    } else if (response_id == GTK_RESPONSE_CANCEL) {
        g_print("Cancel button clicked\n");
    }

}

void edit_dialog (GtkWidget *parent, double new_id, GtkWidget *window1, char *message){
 GtkWidget *dialog, *edit_title_entry, *edit_command_entry, *content_area;
 GtkDialogFlags flags;

 // Create the widgets
 flags = GTK_DIALOG_DESTROY_WITH_PARENT;
 dialog = gtk_dialog_new_with_buttons (message,
                                       GTK_WINDOW (window1),
                                       flags,
                                       "Submit",
                                       GTK_RESPONSE_OK,
                                       "Cancel",
                                       GTK_RESPONSE_CANCEL,
                                       NULL);



  content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  // set the default width of the dialog window to 400 pixels
  gtk_window_set_default_size(GTK_WINDOW(dialog), 350, -1);

  //ADD THE TITLE FOR THE COMMAND
  edit_title_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(edit_title_entry), "Title");
  gtk_widget_set_hexpand(GTK_WIDGET(edit_title_entry), TRUE);
  gtk_widget_set_margin_bottom (edit_title_entry, 5);
  //ADD THE TITLE ENTRY TO THE MAIN CONTAINER
  gtk_box_append (GTK_BOX(content_area), edit_title_entry);
  //CREATE THE ENTRY FOR THE COMMAND
  edit_command_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(edit_command_entry), "Command");
  gtk_widget_set_hexpand(GTK_WIDGET(edit_command_entry), TRUE);
  gtk_widget_set_margin_bottom (edit_command_entry, 5);

  editValues *valuesEdit = g_new(editValues, 1);
    valuesEdit->value1 = parent;
    valuesEdit->value2 = new_id;
    valuesEdit->TitleEntry = edit_title_entry;
    valuesEdit->CommandEntry = edit_command_entry;
    valuesEdit->WindowWidget = window1;
  // Connect the "response" signal to the callback function
    g_signal_connect(dialog, "response", G_CALLBACK(edit_button_clicked), valuesEdit);
  //ADD THE COMMAND ENTRY TO THE MAIN CONTAINER
  gtk_box_append (GTK_BOX(content_area),edit_command_entry);


  gtk_widget_set_margin_top (content_area, 10);
  gtk_widget_set_margin_start (content_area, 10);
  gtk_widget_set_margin_end (content_area, 10);
  gtk_widget_set_margin_bottom (content_area, 10);


 // Ensure that the dialog box is destroyed when the user responds

 g_signal_connect_swapped (dialog,
                           "response",
                           G_CALLBACK (gtk_window_destroy),
                           dialog);

 // Add the label, and show everything we’ve added


 gtk_widget_show(dialog);
}


