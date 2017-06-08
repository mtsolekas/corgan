#include "corgan.h"
#include "corgan_data.h"

int main(int argc, char **argv)
{
    GObject *gobj;
    GtkTreeSortable *names_sort;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col;

    if(init_data()) return EXIT_FAILURE;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file(argv[1]);
    gtk_builder_connect_signals(builder, NULL);

    gobj = gtk_builder_get_object(builder, "sched_buf");
    sched_buf = GTK_TEXT_BUFFER(gobj);
    gtk_text_buffer_set_text(sched_buf, sched, -1);

    gobj = gtk_builder_get_object(builder, "names_list");
    names_list = GTK_LIST_STORE(gobj);

    names_sort = GTK_TREE_SORTABLE(names_list);
    gtk_tree_sortable_set_sort_column_id(names_sort, 0, GTK_SORT_ASCENDING);

    for (int i = 0; contacts[i]; i += 3) {
        gtk_list_store_append(names_list, &iter);
        gtk_list_store_set(names_list, &iter, 0, contacts[i], -1);
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

    if (!contacts[0]) {
        new_button_clicked();
        write_contacts_file();
    }
    else {
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(names_list), &iter);
        gtk_tree_selection_select_iter(selection, &iter);
    }
    contacts_changed = 0;

    gtk_main();

    free_data();

    return EXIT_SUCCESS;
}

int get_active_index()
{
    char *name;
    GtkTreeIter iter;
    GtkTreeModel *model;

    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) return -1;
    gtk_tree_model_get(model, &iter, 0, &name, -1);

    for (int i = 0; contacts[i]; i += 3) {
        if (!strcmp(contacts[i], name)) {
            free(name);
            return i;
        }
    }

    free(name);
    return -1;
}

void window_delete_event()
{
    gtk_main_quit();
}

void selection_changed()
{
    GtkTreeIter iter;
    int index;

    index = get_active_index();
    if (index < 0 && contacts[0]) {
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(names_list), &iter);
        gtk_tree_selection_select_iter(selection, &iter);
        return;
    }
    else if (index < 0) {
        new_button_clicked();
        return;
    }

    gtk_entry_set_text(name_entry, contacts[index]);
    gtk_entry_set_text(email_entry, contacts[++index]);
    gtk_entry_set_text(phone_entry, contacts[++index]);
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
    int index;

    index = get_active_index();

    del_contact(index);

    gtk_tree_selection_get_selected(selection, &model, &iter);
    gtk_list_store_remove(names_list, &iter);

    contacts_changed = 1;
}

void save_button_clicked()
{
    GtkTextIter start;
    GtkTextIter end;
    char *new_sched;

    GtkTreeModel *model;
    GtkTreeIter tree_iter;
    int index;
    char *new_name, *new_email, *new_phone;

    gtk_text_buffer_get_start_iter(sched_buf, &start);
    gtk_text_buffer_get_end_iter(sched_buf, &end);
    new_sched = gtk_text_buffer_get_text(sched_buf, &start, &end, TRUE);
    
    if (strcmp(sched, new_sched)) {
        free(sched);
        sched = new_sched;
        write_schedule_file();
    }

    index = get_active_index();
    if (index < 0) return;

    new_name = strdup(gtk_entry_get_text(name_entry));
    new_email = strdup(gtk_entry_get_text(email_entry));
    new_phone = strdup(gtk_entry_get_text(phone_entry));

    if (strcmp(contacts[index], new_name) ||
            !strcmp(contacts[index], "NEW CONTACT") ||
            strcmp(contacts[index+1], new_email) ||
            strcmp(contacts[index+2], new_phone)) {

        contacts[index] = new_name;
        contacts[index+1] = new_email;
        contacts[index+2] = new_phone;
        contacts_changed = 1;

        gtk_tree_selection_get_selected(selection, &model, &tree_iter);
        gtk_list_store_set(names_list, &tree_iter, 0, contacts[index], -1);
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
