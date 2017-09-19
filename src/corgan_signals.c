#include <stdlib.h>
#include <string.h>

#include "corgan.h"
#include "corgan_signals.h"
#include "corgan_contacts.h"
#include "corgan_schedule.h"

static int get_active_index();

static int get_active_index()
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    char *name;
    int idx;

    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) return -1;
    gtk_tree_model_get(model, &iter, 0, &name, -1);

    idx = search_contacts(name);
    free(name);

    return idx;
}

void window_delete_event()
{
    gtk_main_quit();
}

void selection_changed()
{
    GtkTreeIter iter;
    int idx;

    idx = get_active_index();
    if (idx < 0 && contacts_size > 0) {
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(names_list), &iter);
        gtk_tree_selection_select_iter(selection, &iter);
        return;
    }
    else if (idx < 0) {
        new_button_clicked();
        return;
    }

    gtk_entry_set_text(name_entry, contacts[idx]->name);
    gtk_entry_set_text(email_entry, contacts[idx]->email);
    gtk_entry_set_text(phone_entry, contacts[idx]->phone);
}

void new_button_clicked()
{
    GtkTreeIter iter;

    new_contact();

    gtk_list_store_append(names_list, &iter);
    gtk_list_store_set(names_list, &iter, 0, "NEW CONTACT", -1);

    gtk_tree_selection_select_iter(selection, &iter);
}

void delete_button_clicked()
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    int idx;

    idx = get_active_index();

    del_contact(idx);

    gtk_tree_selection_get_selected(selection, &model, &iter);
    gtk_list_store_remove(names_list, &iter);

    contacts_changed = 1;
}

void export_button_clicked()
{
    export_contacts_vcard();
}

void save_button_clicked()
{
    GtkTextIter start, end;
    GtkTreeModel *model;
    GtkTreeIter tree_iter;
    char *new_sched, *new_name, *new_email, *new_phone;
    int idx;

    gtk_text_buffer_get_start_iter(sched_buf, &start);
    gtk_text_buffer_get_end_iter(sched_buf, &end);
    new_sched = gtk_text_buffer_get_text(sched_buf, &start, &end, TRUE);
    
    if (strcmp(sched, new_sched)) {
        free(sched);
        sched = new_sched;
        write_schedule_file();
    }
    else {
        free(new_sched);
    }

    idx = get_active_index();
    if (idx < 0) return;

    new_name = strdup(gtk_entry_get_text(name_entry));
    new_email = strdup(gtk_entry_get_text(email_entry));
    new_phone = strdup(gtk_entry_get_text(phone_entry));

    if (!strcmp(contacts[idx]->name, "NEW CONTACT")
        || strcmp(contacts[idx]->name, new_name)
        || strcmp(contacts[idx]->email, new_email)
        || strcmp(contacts[idx]->phone, new_phone)) {
        
        free(contacts[idx]->name);
        free(contacts[idx]->email);
        free(contacts[idx]->phone);

        contacts[idx]->name = new_name;
        contacts[idx]->email = new_email;
        contacts[idx]->phone = new_phone;
        contacts_changed = 1;

        gtk_tree_selection_get_selected(selection, &model, &tree_iter);
        gtk_list_store_set(names_list, &tree_iter, 0,
                           contacts[idx]->name, -1);
    }
    else {
        free(new_name);
        free(new_email);
        free(new_phone);
    }

    if (contacts_changed) {
        write_contacts_file();
        contacts_changed = 0;
    }
}
