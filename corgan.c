#include "corgan.h"
#include "corgan_data.h"

int main(int argc, char **argv)
{
    if(init_data()) return EXIT_FAILURE;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("corgan.glade");
    gtk_builder_connect_signals(builder, NULL);

    sched_buf = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "sched_buf"));

    gtk_text_buffer_set_text(sched_buf, sched, -1);

    gtk_main();

    free_data();

    return EXIT_SUCCESS;
}

void window_delete_event()
{
    gtk_main_quit();
}

void contacts_combo_changed()
{

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

    gtk_text_buffer_get_start_iter(sched_buf, &start);
    gtk_text_buffer_get_end_iter(sched_buf, &end);
    new_sched = gtk_text_buffer_get_text(sched_buf, &start, &end, TRUE);
    
    if (strcmp(sched, new_sched)) {
        free(sched);
        sched = new_sched;
        if (write_schedule_file()) {
            puts("Error writing schedule file");
        }
    }
}
