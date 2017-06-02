#include "corgan.h"
#include "corgan_data.h"

int main(int argc, char **argv)
{
    init_corgan(argc, argv);

    free_corgan();

    return EXIT_SUCCESS;
}

void init_corgan(int argc, char **argv)
{
    init_data();

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("corgan.glade");
    gtk_builder_connect_signals(builder, NULL);

    gtk_main();
}

void free_corgan()
{
    free_data();
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

}
