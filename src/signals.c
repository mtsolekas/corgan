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

int delflag = 0, newflag = 0;

int idx_save;
GtkTreeIter iter_save;

static int get_active_index();
static int save_contact();

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

    idx_save = idx;
    iter_save = iter;

    return idx;
}

static int save_contact()
{
    char *new_name;

    if (contacts_changed) {

        if (name_changed) {
            new_name = xstrdup(gtk_entry_get_text(name_entry));
            while (strcmp(contacts[idx_save]->name, new_name)
                   && search_contacts(new_name) >= 0) {
                new_name = xrealloc(new_name, strlen(new_name) + 5);
                new_name = strcat(new_name, " Alt");
                gtk_entry_set_text(name_entry, new_name);
            }

            free(contacts[idx_save]->name);
            contacts[idx_save]->name = new_name;
        }

        if (email_changed) {
            free(contacts[idx_save]->email);
            contacts[idx_save]->email =
                xstrdup(gtk_entry_get_text(email_entry));
        }

        if (phone_changed) {
            free(contacts[idx_save]->phone);
            contacts[idx_save]->phone =
                xstrdup(gtk_entry_get_text(phone_entry));
        }

        gtk_list_store_set(names_list, &iter_save, 0,
                           contacts[idx_save]->name, -1);

        sort_contacts();
    }

    return 0;
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

    if (!newflag && !delflag)
        save_contact();

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

    name_changed = 0;
    email_changed = 0;
    phone_changed = 0;
}

void new_button_clicked()
{
    GtkTreeIter iter;
    GtkTreePath *path;
    int idx;

    if (!delflag && !newflag)
        save_contact();

    newflag = 1;

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

    newflag = 0;
}

void delete_button_clicked()
{
    GtkTreeIter iter;
    GtkTreeModel *model;

    del_contact(idx_save);

    delflag = 1;

    gtk_tree_selection_get_selected(selection, &model, &iter);
    gtk_list_store_remove(names_list, &iter);

    contacts_changed = 1;

    delflag = 0;
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

    if (schedule_changed) {
        gtk_text_buffer_get_start_iter(sched_buf, &start);
        gtk_text_buffer_get_end_iter(sched_buf, &end);

        free(sched);
        sched = gtk_text_buffer_get_text(sched_buf, &start, &end, TRUE);

        schedule_changed = 0;

        write_schedule_file();
    }

    if (contacts_changed) {

        save_contact();

        gtk_tree_selection_get_selected(selection, &model, &tree_iter);
        path = gtk_tree_model_get_path(model, &iter_save);
        gtk_tree_view_scroll_to_cell(contacts_view, path, NULL, 0, 0, 0);
        gtk_tree_path_free(path);

        contacts_changed = 0;
        name_changed = 0;
        email_changed = 0;
        phone_changed = 0;

        write_contacts_file();
    }
}
