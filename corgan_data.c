#include "corgan_data.h"

void init_data()
{
    FILE *fp;
    int path_size;
    char *config_dir;
    
    path_size = sizeof(char) * 30;
    config_dir = strdup(getenv("HOME"));
    config_dir = strcat(realloc(config_dir, path_size), "/.config/corgan");

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

    read_schedule_file();
}

void free_data()
{
    free(CONTACTS_PATH);
    free(SCHEDULE_PATH);
}

int read_schedule_file()
{
    FILE *fp;
    int byte_size, size;
    char c;

    fp = fopen(SCHEDULE_PATH, "r");
    if(!fp) return -1;

    byte_size = sizeof(char) * 100;
    sched = malloc(byte_size);
    if (!sched) return -1;

    for (size = 0; (c = getc(fp)) != EOF; ++size) {
        if (size >= byte_size / sizeof(char)) {
            byte_size += sizeof(char) * 100;
            sched = realloc(sched, byte_size);
            if (!sched) return -1;
        }
        sched[size] = c;
    }

    fclose(fp);

    sched[size] = '\0';
    sched = realloc(sched, sizeof(char) * (size));
    if (!sched) return -1;

    return 0;
}

int write_schedule_file()
{
    FILE *fp;

    fp = fopen(SCHEDULE_PATH, "w");
    if (!fp) return -1;

    fputs(sched, fp);
    fclose(fp);

    return 0;
}
