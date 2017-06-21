#include "corgan_data.h"

int init_data()
{
    FILE *fp;
    char *config_dir;
    int path_size;
    
    path_size = sizeof(char) * 30;
    config_dir = strdup(getenv("HOME"));
    config_dir = strcat(realloc(config_dir, path_size), "/.config/corgan/");

    CONTACTS_PATH = strcat(strdup(config_dir), "contacts");
    SCHEDULE_PATH = strcat(strdup(config_dir), "schedule");

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

    if (read_contacts_file()) return -1;
    if (read_schedule_file()) return -1;

    return 0;
}

void free_data()
{
    free(CONTACTS_PATH);
    free(SCHEDULE_PATH);

    for (int i = 0; i < contacts_size; ++i) {
        free(contacts[i]);
    }

    free(contacts);
    free(sched);
}

int new_contact()
{
    contacts_size += 3;
    contacts = realloc(contacts, sizeof(char*) * contacts_size);
    if (!contacts) return -1;

    contacts[contacts_size-3] = strdup("NEW CONTACT");
    contacts[contacts_size-2] = strdup(" ");
    contacts[contacts_size-1] = strdup(" ");

    if (sort_contacts()) return -1;

    return 0;
}

int del_contact(int idx)
{
    free(contacts[idx]);
    free(contacts[++idx]);
    free(contacts[++idx]);

    contacts[idx] = contacts[contacts_size-1];
    contacts[--idx] = contacts[contacts_size-2];
    contacts[--idx] = contacts[contacts_size-3];

    contacts[contacts_size-1] = NULL;
    contacts[contacts_size-2] = NULL;
    contacts[contacts_size-3] = NULL;
    contacts_size -= 3;

    contacts = realloc(contacts, sizeof(char*) * contacts_size);
    if (!contacts) return -1;

    if (sort_contacts()) return -1;

    return 0;
}

int search_contacts(const char *name)
{
    int pos, new_pos, direction, lbound, ubound;

    lbound = 0;
    ubound = contacts_size / 3;

    pos = -1;
    new_pos = (lbound + ((ubound  - lbound) / 2)) * 3;

    while (pos != new_pos) {
        pos = new_pos;

        direction = strcmp(name, contacts[pos]);
        if (direction < 0) {
            ubound = (pos / 3);
        }
        else if (direction > 0) {
            lbound = (pos / 3);
        }
        else {
            return pos;
        }

        new_pos = (lbound + ((ubound - lbound) / 2)) * 3;
    }

    return -1;
}

int sort_contacts()
{
    char *tmp_name, *tmp_email, *tmp_phone;

    if (!contacts_size) return 0;

    for (int i = 0; i < contacts_size; i += 3) {
        for (int j = i + 3; j < contacts_size; j += 3) {
            if (strcmp(contacts[i], contacts[j]) > 0) {
                tmp_name = contacts[i];
                tmp_email = contacts[i+1];
                tmp_phone = contacts[i+2];

                contacts[i] = contacts[j];
                contacts[i+1] = contacts[j+1];
                contacts[i+2] = contacts[j+2];

                contacts[j] = tmp_name;
                contacts[j+1] = tmp_email;
                contacts[j+2] = tmp_phone;
            }
        }
    }

    return 0;
}

int entry_length(char *line)
{
    int i;

    for (i = 0; line[i]; ++i) {
        if (line[i] == '\n') {
            line[i] = '\0';
            return ++i;
        }
    }

    return i;
}

int read_contacts_file()
{
    FILE *fp;
    char *line, *entry;
    int i;

    fp = fopen(CONTACTS_PATH, "r");
    if(!fp) return -1;

    contacts_size = 100;
    contacts = malloc(sizeof(char*) * contacts_size);
    if (!contacts) return -1;

    line = malloc(sizeof(char) * 100);
    if(!line) return -1;

    for (i = 0; (line = fgets(line, 100, fp)); ++i) {
        entry = strndup(line, sizeof(char) * entry_length(line));
        if (!entry) return -1;

        if (i >= contacts_size) {
            contacts_size += 100;
            contacts = realloc(contacts,sizeof(char*) * contacts_size);
            if (!contacts) return -1;
        }

        contacts[i] = entry;
    }

    free(line);

    fclose(fp);

    contacts_size = i;
    contacts = realloc(contacts, sizeof(char*) * contacts_size);
    if (!contacts && contacts_size > 0) return -1;

    if (sort_contacts()) return -1;

    return 0;
}

int write_contacts_file()
{
    FILE *fp;

    fp = fopen(CONTACTS_PATH, "w");
    if (!fp) return -1;

    if (sort_contacts()) return -1;
    for (int i = 0; i < contacts_size; ++i) {
        fputs(contacts[i], fp);
        fputc('\n', fp);
    }

    fclose(fp);

    return 0;
}

int read_schedule_file()
{
    FILE *fp;
    char c;
    int size, i;

    fp = fopen(SCHEDULE_PATH, "r");
    if(!fp) return -1;

    size = 100;
    sched = malloc(sizeof(char) * size);
    if (!sched) return -1;

    for (i = 0; (c = getc(fp)) != EOF; ++i) {
        if (i >= size) {
            size += 100;
            sched = realloc(sched, sizeof(char) * size);
            if (!sched) return -1;
        }
        sched[i] = c;
    }

    fclose(fp);

    sched[i] = '\0';
    sched = realloc(sched, sizeof(char) * (++i));
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
