#include "corgan_paths.h"

int init_paths()
{
    if (!getenv("XDG_DATA_HOME") || !strlen(getenv("XDG_DATA_HOME"))) {
        DATA_DIR = realloc(strdup(getenv("HOME")),
                           sizeof(char) * (strlen(getenv("HOME"))
                                           + strlen("/.local/share/corgan/")
                                           + 1));
        if (!DATA_DIR) return -1;

        DATA_DIR = strcat(DATA_DIR, "/.local/");
        mkdir(DATA_DIR, 0700);

        DATA_DIR = strcat(DATA_DIR, "share/");
        mkdir(DATA_DIR, 0700);

        DATA_DIR = strcat(DATA_DIR, "corgan/");
        mkdir(DATA_DIR, 0700);
    }
    else {
        DATA_DIR = realloc(strdup(getenv("XDG_DATA_HOME")),
                           sizeof(char) * (strlen(getenv("XDG_DATA_HOME"))
                                           + strlen("/corgan/") + 1));
        if (!DATA_DIR) return -1;

        DATA_DIR = strcat(DATA_DIR, "/corgan/");
        mkdir(DATA_DIR, 0700);
    }

    CONTACTS_PATH = realloc(strdup(DATA_DIR),
                            sizeof(char) * (strlen(DATA_DIR)
                                            + strlen("contacts") + 1));
    SCHEDULE_PATH = realloc(strdup(DATA_DIR),
                            sizeof(char) * (strlen(DATA_DIR)
                                            + strlen("contacts") + 1));
    EXPORT_PATH = realloc(strdup(DATA_DIR), 
                          sizeof(char) * (strlen(DATA_DIR)
                                          + strlen("contacts.vcf") + 1));
    if (!CONTACTS_PATH || !SCHEDULE_PATH || !EXPORT_PATH) return -1;

    CONTACTS_PATH = strcat(CONTACTS_PATH, "contacts");
    SCHEDULE_PATH = strcat(SCHEDULE_PATH, "schedule");
    EXPORT_PATH = strcat(EXPORT_PATH, "contacts.vcf");

    return 0;
}

int free_paths()
{
    free(DATA_DIR);
    free(CONTACTS_PATH);
    free(SCHEDULE_PATH);
    free(EXPORT_PATH);

    return 0;
}
