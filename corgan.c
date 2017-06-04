#include "corgan.h"
#include "corgan_data.h"

int main(int argc, char **argv)
{
    GObject *gobj;
    GtkTreeSortable *names_sort;

    GtkCellLayout *cell_layout;
    GtkCellRenderer *renderer;

    GtkTreeIter tree_iter;

    if(init_data()) return EXIT_FAILURE;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("corgan.glade");
    gtk_builder_connect_signals(builder, NULL);

    gobj = gtk_builder_get_object(builder, "sched_buf");
    sched_buf = GTK_TEXT_BUFFER(gobj);
    gtk_text_buffer_set_text(sched_buf, sched, -1);

    gobj = gtk_builder_get_object(builder, "names_list");
    names_list = GTK_LIST_STORE(gobj);

    names_sort = GTK_TREE_SORTABLE(names_list);
    gtk_tree_sortable_set_sort_column_id(names_sort, 0, GTK_SORT_ASCENDING);

    for (int i = 0; contacts[i]; i += 3) {
        gtk_list_store_append(names_list, &tree_iter);
        gtk_list_store_set(names_list, &tree_iter, 0, contacts[i], -1);
    }

    gobj = gtk_builder_get_object(builder, "contacts_combo");
    contacts_combo = GTK_COMBO_BOX(gobj);

    cell_layout = GTK_CELL_LAYOUT(contacts_combo);
    renderer = GTK_CELL_RENDERER(gtk_cell_renderer_text_new());

    gtk_cell_layout_pack_start(cell_layout, renderer, TRUE);
    gtk_cell_layout_add_attribute(cell_layout, renderer, "text", 0);

    gobj = gtk_builder_get_object(builder, "name_entry");
    name_entry = GTK_ENTRY(gobj);
    gobj = gtk_builder_get_object(builder, "email_entry");
    email_entry = GTK_ENTRY(gobj);
    gobj = gtk_builder_get_object(builder, "phone_entry");
    phone_entry = GTK_ENTRY(gobj);

    gtk_combo_box_set_active(contacts_combo, 0);

    gtk_main();

    free_data();

    return EXIT_SUCCESS;
}

int get_active_index()
{
    char *name;
    GtkTreeIter iter;
    GtkTreeModel *model;

    gtk_combo_box_get_active_iter(contacts_combo, &iter);
    model = gtk_combo_box_get_model(contacts_combo);
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

void contacts_combo_changed()
{
    int index;

    index = get_active_index();

    gtk_entry_set_text(name_entry, contacts[index]);
    gtk_entry_set_text(email_entry, contacts[++index]);
    gtk_entry_set_text(phone_entry, contacts[++index]);
}

void new_button_clicked()
{

}

void delete_button_clicked()
{

}

void save_button_clicked()
{
    GtkTextIter start;
    GtkTextIter end;
    char *new_sched;

    GtkTreeIter tree_iter;
    int index, contact_changed;
    char *new_name, *new_email, *new_phone;

    gtk_text_buffer_get_start_iter(sched_buf, &start);
    gtk_text_buffer_get_end_iter(sched_buf, &end);
    new_sched = gtk_text_buffer_get_text(sched_buf, &start, &end, TRUE);
    
    if (strcmp(sched, new_sched)) {
        free(sched);
        sched = new_sched;
        write_schedule_file();
    }

    contact_changed = 0;
    index = get_active_index();

    new_name = strdup(gtk_entry_get_text(name_entry));
    new_email = strdup(gtk_entry_get_text(email_entry));
    new_phone = strdup(gtk_entry_get_text(phone_entry));

    if (strcmp(contacts[index+1], new_email)) {
        contacts[index+1] = new_email;
        contact_changed = 1;
    }
    else {
        free(new_email);
    }

    if (strcmp(contacts[index+2], new_phone)) {
        contacts[index+2] = new_phone;
        contact_changed = 1;
    }
    else {
        free(new_phone);
    }

    if (strcmp(contacts[index], new_name)) {
        contacts[index] = new_name;
        contact_changed = 1;

        gtk_combo_box_get_active_iter(contacts_combo, &tree_iter);
        gtk_list_store_set(names_list, &tree_iter, 0, contacts[index], -1);
    }
    else {
        free(new_name);
    }

    if (contact_changed) {
        write_contacts_file();
    }
}
