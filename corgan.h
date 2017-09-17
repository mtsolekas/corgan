#ifndef __CORGAN_H
#define __CORGAN_H

#include <gtk/gtk.h>

GtkBuilder *builder;

GtkTextBuffer *sched_buf;

GtkListStore *names_list;
GtkTreeView *contacts_view;
GtkTreeSelection *selection;
GtkEntry *name_entry, *email_entry, *phone_entry;

extern int contacts_changed;

int init_glade();

#endif
