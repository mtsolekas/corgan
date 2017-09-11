#include "corgan_data.h"

int init_data()
{
    APP_DIR = g_string_append(g_string_new(getenv("HOME")), "/.corgan/");
    CONTACTS_PATH = g_string_append(g_string_new(APP_DIR->str), "contacts");
    SCHEDULE_PATH = g_string_append(g_string_new(APP_DIR->str), "schedule");
    EXPORT_PATH = g_string_append(g_string_new(APP_DIR->str), "contacts.vcf");

    g_mkdir(APP_DIR->str, 0700);

    if (g_access(CONTACTS_PATH->str, F_OK | R_OK | W_OK)) {
        if (!g_file_set_contents(CONTACTS_PATH->str, "", -1, NULL)) return -1;
    }

    if (g_access(SCHEDULE_PATH->str, F_OK | R_OK | W_OK)) {
        if (!g_file_set_contents(SCHEDULE_PATH->str, "", -1, NULL)) return -1;
    }

    if (read_contacts_file()) return -1;
    if (read_schedule_file()) return -1;

    return 0;
}

void free_data()
{
    g_string_free(APP_DIR, TRUE);
    g_string_free(CONTACTS_PATH, TRUE);
    g_string_free(SCHEDULE_PATH, TRUE);
    g_string_free(EXPORT_PATH, TRUE);

    for (int i = 0; i < contacts_size; ++i) {
        free(contacts[i]->name);
        free(contacts[i]->email);
        free(contacts[i]->phone);
        free(contacts[i]);
    }

    free(contacts);
    g_string_free(sched, TRUE);
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

        if (idx < 0) {
            ubound = pos;
        }
        else if (idx > 0) {
            lbound = pos;
        } else {
            return pos;
        }
        pos = ((ubound - lbound) / 2) + lbound;
    }

    return -1;
}

int line_length(char *line)
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
    char *line, *pointer_save;
    int i;

    fp = fopen(CONTACTS_PATH->str, "r");
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

        contacts[i]->name = strndup(line, sizeof(char) * line_length(line));
        if (!contacts[i]->name) return -1;

        line = fgets(line, 100, fp);
        contacts[i]->email = strndup(line, sizeof(char) * line_length(line));
        if (!contacts[i]->email) return -1;

        line = fgets(line, 100, fp);
        contacts[i]->phone = strndup(line, sizeof(char) * line_length(line));
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

    fp = fopen(CONTACTS_PATH->str, "w");
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
    sched = g_string_new(NULL);
    if (!g_file_get_contents(SCHEDULE_PATH->str, &sched->str, NULL, NULL))
        return -1;

    return 0;
}

int write_schedule_file()
{
    if (!g_file_set_contents(SCHEDULE_PATH->str, sched->str, -1, NULL))
        return -1;

    return 0;
}

int export_contacts_vcard()
{
    FILE *fp;
    char *name, *fname, *lname, *tmp;

    fp = fopen(EXPORT_PATH->str, "w");
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
                                contacts[i]->name, contacts[i]->phone,
                                contacts[i]->email);
    }

    fclose(fp);
    return 0;
}
