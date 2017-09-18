#include "corgan.h"
#include "corgan_paths.h"
#include "corgan_contacts.h"
#include "corgan_schedule.h"

static int init_glade();

int main(int argc, char **argv)
{
    GObject *gobj;
    GtkTreeSortable *names_sort;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col;

    gtk_init(&argc, &argv);

    if (init_paths()) return EXIT_FAILURE;
    if (init_contacts()) return EXIT_FAILURE;
    if (init_schedule()) return EXIT_FAILURE;
    if (init_glade()) return EXIT_FAILURE;

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

    contacts_changed = 0;

    gtk_main();

    if (free_paths()) return EXIT_FAILURE;
    if (free_contacts()) return EXIT_FAILURE;
    if (free_schedule()) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

static int init_glade()
{
    char *glade_path;

    glade_path = realloc(strdup(APP_DIR), sizeof(char)
                                          * (strlen(APP_DIR)
                                             + strlen("corgan.glade") + 1));
    if (!glade_path) return -1;
    glade_path = strcat(glade_path, "corgan.glade");
    
    builder = gtk_builder_new_from_file(glade_path);
    gtk_builder_connect_signals(builder, NULL);

    free(glade_path);

    return 0;
}
