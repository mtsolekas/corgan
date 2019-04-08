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
#include <locale.h>
#include <libintl.h>

#include "config.h"

#include "corgan.h"
#include "signals.h"
#include "paths.h"
#include "contacts.h"
#include "schedule.h"

static void activate(GtkApplication * app)
{
    GList *list;
    GObject *gobj;
    GtkTreeSortable *names_sort;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col;

    list = gtk_application_get_windows(app);

    if (list) {
        gtk_window_present(GTK_WINDOW(list->data));
        return;
    }

    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(PACKAGE, "UTF-8");
    textdomain(PACKAGE);

    if (init_paths())
        exit(EXIT_FAILURE);
    if (init_contacts())
        exit(EXIT_FAILURE);
    if (init_schedule())
        exit(EXIT_FAILURE);

    builder = gtk_builder_new_from_resource("/org/corgan/window.ui");
    gtk_builder_connect_signals(builder, NULL);

    gobj = gtk_builder_get_object(builder, "sched_buf");
    sched_buf = GTK_TEXT_BUFFER(gobj);
    gtk_text_buffer_set_text(sched_buf, sched, -1);

    gobj = gtk_builder_get_object(builder, "names_list");
    names_list = GTK_LIST_STORE(gobj);

    names_sort = GTK_TREE_SORTABLE(names_list);
    gtk_tree_sortable_set_sort_column_id(names_sort, 0, GTK_SORT_ASCENDING);

    for (int i = 0; i < contacts_size; ++i) {
        gtk_list_store_append(names_list, &iter);
        gtk_list_store_set(names_list, &iter, 0, contacts[i]->name, -1);
    }

    gobj = gtk_builder_get_object(builder, "selection");
    selection = GTK_TREE_SELECTION(gobj);

    gobj = gtk_builder_get_object(builder, "contacts_view");
    contacts_view = GTK_TREE_VIEW(gobj);

    renderer = gtk_cell_renderer_text_new();

    col = gtk_tree_view_column_new_with_attributes("Name", renderer,
                                                   "text", 0, NULL);
    gtk_tree_view_append_column(contacts_view, col);

    gobj = gtk_builder_get_object(builder, "name_entry");
    name_entry = GTK_ENTRY(gobj);
    gobj = gtk_builder_get_object(builder, "email_entry");
    email_entry = GTK_ENTRY(gobj);
    gobj = gtk_builder_get_object(builder, "phone_entry");
    phone_entry = GTK_ENTRY(gobj);

    gtk_tree_model_get_iter_first(GTK_TREE_MODEL(names_list), &iter);
    gtk_tree_selection_select_iter(selection, &iter);

    gobj = gtk_builder_get_object(builder, "window");
    gtk_application_add_window(app, GTK_WINDOW(gobj));
}

int main(int argc, char **argv)
{
    GtkApplication *app;

    app = gtk_application_new("org.corgan", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_application_run(G_APPLICATION(app), argc, argv);
    if (builder)
        g_object_unref(builder);
    g_object_unref(app);

    if (free_paths())
        exit(EXIT_FAILURE);
    if (free_contacts())
        exit(EXIT_FAILURE);
    if (free_schedule())
        exit(EXIT_FAILURE);

    return EXIT_SUCCESS;
}
