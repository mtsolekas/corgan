#include "corgan_data.h"

int init_data()
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

    if(read_schedule_file()) return -1;
    if(read_contacts_file()) return -1;

    return 0;
}

void free_data()
{
    free(CONTACTS_PATH);
    free(SCHEDULE_PATH);

    for (int i = 0; contacts[i]; ++i) {
        free(contacts[i]);
    }

    free(contacts);
    free(sched);
}

int text_entry_length(char *line)
{
    int i = 0;

    while (line[i]) {
        if (line[i] == '\n') {
            line[i] = '\0';
            return ++i;
        }
        ++i;
    }

    return i;
}

int read_contacts_file()
{
    FILE *fp;
    char *line;
    char *entry;
    unsigned int size;

    fp = fopen(CONTACTS_PATH, "r");
    if(!fp) return -1;

    size = sizeof(char*) * 100;
    contacts = malloc(size);
    if (!contacts) return -1;

    line = malloc(sizeof(char) * 100);
    if(!line) return -1;

    for (int i = 0; (line = fgets(line, 100, fp)); ++i) {
        entry = realloc(strdup(line), text_entry_length(line));
        if (!entry) return -1;

        if (i * sizeof(char*) >= size) {
            size += sizeof(char) * 100;
            contacts = realloc(contacts, size);
            if (!contacts) return -1;
        }

        contacts[i] = strdup(entry);
    }

    free(entry);
    free(line);

    fclose(fp);

    return 0;
}

int write_contacts_file()
{
    FILE *fp;

    fp = fopen(CONTACTS_PATH, "w");
    if (!fp) return -1;

    for (int i = 0; contacts[i]; ++i) {
        fputs(contacts[i], fp);
        fputc('\n', fp);
    }

    fclose(fp);

    return 0;
}

int read_schedule_file()
{
    FILE *fp;
    unsigned int byte_size, size;
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
