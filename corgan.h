#ifndef __CORGAN_H
#define __CORGAN_H

#include <gtk/gtk.h>

GtkBuilder *builder;

GtkTextBuffer *sched_buf;

GtkListStore *names_list;
GtkComboBox *contacts_combo;
GtkEntry *name_entry, *email_entry, *phone_entry;

int contacts_changed;

int get_active_index();

void window_delete_event();
void combo_changed();
void new_button_clicked();
void delete_button_clicked();
void save_button_clicked();

#endif
