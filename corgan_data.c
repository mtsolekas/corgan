#include "corgan_data.h"

void init_data()
{
    FILE *fp;
    char *config_dir;
    
    config_dir = strdup(getenv("HOME"));
    config_dir = strcat(realloc(config_dir, 30), "/.config/corgan");

    CONTACTS_PATH = strcat(strdup(config_dir), "/contacts");
    SCHEDULE_PATH = strcat(strdup(config_dir), "/schedule");

    mkdir(config_dir, 0700);
    free(config_dir);

    if (access(CONTACTS_PATH, F_OK | R_OK | W_OK)) {
        fp = fopen(CONTACTS_PATH, "w");
        fclose(fp);
    }

    if (access(SCHEDULE_PATH, F_OK | R_OK | W_OK)) {
        fp = fopen(SCHEDULE_PATH, "w");
        fclose(fp);
    }
}

void free_data()
{
    free(CONTACTS_PATH);
    free(SCHEDULE_PATH);
}

int read_schedule_file()
{
    FILE *fp;

    fp = fopen(SCHEDULE_PATH, "r");
    if(!fp) {
        return -1;
    }

    fclose(fp);

    return 0;
}
