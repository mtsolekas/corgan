/*
 * Copyright (C) 2017-2019 Marios Tsolekas <marios.tsolekas@gmail.com>
 *
 * This file is part of Corgan.
 *
 * Corgan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Corgan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Corgan.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Marios Tsolekas
 */

#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "corgan.h"
#include "signals.h"
#include "contacts.h"
#include "schedule.h"

int schedule_changed = 0,
    contacts_changed = 0,
    name_changed = 0, email_changed = 0, phone_changed = 0;

static int get_active_index()
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    char *name;
    int idx;

    if (!gtk_tree_selection_get_selected(selection, &model, &iter))
        return -1;
    gtk_tree_model_get(model, &iter, 0, &name, -1);

    idx = search_contacts(name);
    free(name);

    return idx;
}

void schedule_buffer_changed()
{
    schedule_changed = 1;
}

void name_entry_changed()
{
    contacts_changed = 1;
    name_changed = 1;
}

void email_entry_changed()
{
    contacts_changed = 1;
    email_changed = 1;
}

void phone_entry_changed()
{
    contacts_changed = 1;
    phone_changed = 1;
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
    } else if (idx < 0) {
        new_button_clicked();
        return;
    }

    gtk_entry_set_text(name_entry, contacts[idx]->name);
    gtk_entry_set_text(email_entry, contacts[idx]->email);
    gtk_entry_set_text(phone_entry, contacts[idx]->phone);

    contacts_changed = 0;
    name_changed = 0;
    email_changed = 0;
    phone_changed = 0;
}

void new_button_clicked()
{
    GtkTreeIter iter;
    GtkTreePath *path;
    int idx;

    idx = 0;
    if (!contacts_size || (idx = search_contacts("NEW CONTACT")) < 0) {
        new_contact();
        gtk_list_store_append(names_list, &iter);
        gtk_list_store_set(names_list, &iter, 0, "NEW CONTACT", -1);
        idx = search_contacts("NEW CONTACT");
    }

    path = gtk_tree_path_new_from_indices(idx, -1);
    gtk_tree_selection_select_path(selection, path);
    gtk_tree_view_scroll_to_cell(contacts_view, path, NULL, 0, 0, 0);
    gtk_tree_path_free(path);
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
    GtkTreePath *path;
    GtkTreeIter tree_iter;
    char *new_name;
    int idx;

    if (schedule_changed) {
        gtk_text_buffer_get_start_iter(sched_buf, &start);
        gtk_text_buffer_get_end_iter(sched_buf, &end);

        free(sched);
        sched = gtk_text_buffer_get_text(sched_buf, &start, &end, TRUE);

        schedule_changed = 0;

        write_schedule_file();
    }

    if (contacts_changed) {
        if ((idx = get_active_index()) < 0)
            return;

        if (name_changed) {
            new_name = xstrdup(gtk_entry_get_text(name_entry));
            while (strcmp(contacts[idx]->name, new_name)
                   && search_contacts(new_name) >= 0) {
                new_name = xrealloc(new_name, strlen(new_name) + 5);
                new_name = strcat(new_name, " Alt");
                gtk_entry_set_text(name_entry, new_name);
            }

            free(contacts[idx]->name);
            contacts[idx]->name = new_name;
        }

        if (email_changed) {
            free(contacts[idx]->email);
            contacts[idx]->email = xstrdup(gtk_entry_get_text(email_entry));
        }

        if (phone_changed) {
            free(contacts[idx]->phone);
            contacts[idx]->phone = xstrdup(gtk_entry_get_text(phone_entry));
        }

        gtk_tree_selection_get_selected(selection, &model, &tree_iter);
        gtk_list_store_set(names_list, &tree_iter, 0, contacts[idx]->name, -1);

        path = gtk_tree_model_get_path(model, &tree_iter);
        gtk_tree_view_scroll_to_cell(contacts_view, path, NULL, 0, 0, 0);
        gtk_tree_path_free(path);

        contacts_changed = 0;
        name_changed = 0;
        email_changed = 0;
        phone_changed = 0;

        write_contacts_file();
    }
}
