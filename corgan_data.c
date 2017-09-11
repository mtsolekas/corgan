#include "corgan_data.h"

int init_data()
{
    FILE *fp;
    char *app_dir;

    app_dir = realloc(strdup(getenv("HOME")),
                            sizeof(char) * (strlen(getenv("HOME")) +
                                            strlen("/.corgan/") + 1));
    if (!app_dir) return -1;
    app_dir = strcat(app_dir, "/.corgan/");

    CONTACTS_PATH = realloc(strdup(app_dir),
                            sizeof(char) * (strlen(app_dir) +
                                            strlen("contacts") + 1));
    SCHEDULE_PATH = realloc(strdup(app_dir),
                            sizeof(char) * (strlen(app_dir) +
                                            strlen("contacts") + 1));
    EXPORT_PATH = realloc(strdup(app_dir),
                            sizeof(char) * (strlen(app_dir) +
                                            strlen("contacts.vcf") + 1));
    if (!CONTACTS_PATH || !SCHEDULE_PATH || !EXPORT_PATH) return -1;

    CONTACTS_PATH = strcat(CONTACTS_PATH, "contacts");
    SCHEDULE_PATH = strcat(SCHEDULE_PATH, "schedule");
    EXPORT_PATH = strcat(EXPORT_PATH, "contacts.vcf");

    mkdir(app_dir, 0700);
    free(app_dir);

    if (access(CONTACTS_PATH, F_OK | R_OK | W_OK)) {
        fp = fopen(CONTACTS_PATH, "w");
        if (!fp) return -1;
        fclose(fp);
    }

    if (access(SCHEDULE_PATH, F_OK | R_OK | W_OK)) {
        fp = fopen(SCHEDULE_PATH, "w");
        if (!fp) return -1;
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
    free(EXPORT_PATH);

    for (int i = 0; i < contacts_size; ++i) {
        free(contacts[i]->name);
        free(contacts[i]->email);
        free(contacts[i]->phone);
        free(contacts[i]);
    }

    free(contacts);
    free(sched);
}

int new_contact()
{
    ++contacts_size;
    contacts = realloc(contacts, sizeof(contact_t*) * contacts_size);
    if (!contacts) return -1;

    contacts[contacts_size-1] = malloc(sizeof(contact_t));
    if (!contacts[contacts_size-1]) return -1;

    contacts[contacts_size-1]->name = strdup("NEW CONTACT");
    contacts[contacts_size-1]->email = strdup("\0");
    contacts[contacts_size-1]->phone = strdup("\0");

    sort_contacts();

    return 0;
}

int del_contact(int idx)
{
    free(contacts[idx]->name);
    free(contacts[idx]->email);
    free(contacts[idx]->phone);
    free(contacts[idx]);

    --contacts_size;
    contacts[idx] = contacts[contacts_size];
    contacts[contacts_size] = NULL;

    contacts = realloc(contacts, sizeof(contact_t*) * contacts_size);
    if (!contacts) return -1;

    sort_contacts();

    return 0;
}

int compare_contacts(const void *p1, const void *p2)
{
    return strcmp((*(contact_t**) p1)->name, (*(contact_t**) p2)->name);
}

int sort_contacts()
{

    if (!contacts_size) return 0;

    qsort(contacts, contacts_size, sizeof(contact_t*), compare_contacts);

    return 0;
}

int search_contacts(const char *name)
{
    int idx, prev_pos, pos, lbound, ubound;

    lbound = 0;
    ubound = contacts_size;
    prev_pos = -1;
    pos = (ubound - lbound) / 2;

    while (prev_pos != pos) {
        prev_pos = pos;
        idx = strcmp(name, contacts[pos]->name);

        if (idx < 0)
            ubound = pos;
        else if (idx > 0)
            lbound = pos;
        else
            return pos;
        pos = ((ubound - lbound) / 2) + lbound;
    }

    return -1;
}

int read_contacts_file()
{
    FILE *fp;
    char *line, *pointer_save;
    int i;

    fp = fopen(CONTACTS_PATH, "r");
    if(!fp) return -1;

    contacts_size = 100;
    contacts = malloc(sizeof(contact_t*) * contacts_size);
    if (!contacts) return -1;

    line = malloc(sizeof(char) * 100);
    pointer_save = line;
    if(!line) return -1;

    for (i = 0; (line = fgets(line, 100, fp)); ++i) {
        if (i >= contacts_size) {
            contacts_size += 100;
            contacts = realloc(contacts, sizeof(contact_t*) * contacts_size);
            if (!contacts) return -1;
        }

        contacts[i] = malloc(sizeof(contact_t));
        if (!contacts[i]) return -1;

        contacts[i]->name = strndup(line, sizeof(char) * (strlen(line) - 1));
        if (!contacts[i]->name) return -1;

        line = fgets(line, 100, fp);
        contacts[i]->email = strndup(line, sizeof(char) * (strlen(line) - 1));
        if (!contacts[i]->email) return -1;

        line = fgets(line, 100, fp);
        contacts[i]->phone = strndup(line, sizeof(char) * (strlen(line) - 1));
        if (!contacts[i]->email) return -1;
    }

    free(pointer_save);

    fclose(fp);

    contacts_size = i;
    contacts = realloc(contacts, sizeof(contact_t*) * contacts_size);
    if (!contacts && contacts_size > 0) return -1;

    sort_contacts();

    return 0;
}

int write_contacts_file()
{
    FILE *fp;

    fp = fopen(CONTACTS_PATH, "w");
    if (!fp) return -1;

    sort_contacts();
    for (int i = 0; i < contacts_size; ++i) {
        fprintf(fp, "%s\n%s\n%s\n", contacts[i]->name, contacts[i]->email,
                contacts[i]->phone);
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

int export_contacts_vcard()
{
    FILE *fp;
    char *name, *fname, *lname, *tmp;

    fp = fopen(EXPORT_PATH, "w");
    if (!fp) return -1;

    for (int i = 0; i < contacts_size; ++i) {
        fprintf(fp, "BEGIN:VCARD\nVERSION:3.0");
        
        name = strdup(contacts[i]->name);
        fname = strtok(name, " ");
        lname = strtok(NULL, " ");
        while ((tmp = strtok(NULL, " "))) {
            lname = tmp;
        }
        fprintf(fp, "\nN:%s;%s;", lname, fname); 
        free(name);

        fprintf(fp, "\nFN:%s\nTEL;TYPE=VOICE:%s\nEMAIL:%s\nEND:VCARD\n",
                contacts[i]->name, contacts[i]->phone, contacts[i]->email);
    }

    fclose(fp);
    return 0;
}
