/*
 * Copyright (C) 2017, 2018 Marios Tsolekas <marios.tsolekas@gmail.com>
 *
 * This file is part of Corgan.
 *
 * Corgan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Corgan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Corgan.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Marios Tsolekas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "paths.h"
#include "contacts.h"

static int compare_contacts(const void *, const void *);
static int sort_contacts();

int init_contacts()
{
    FILE *fp;

    if (access(CONTACTS_PATH, F_OK | R_OK | W_OK)) {
        fp = fopen(CONTACTS_PATH, "w");
        if (!fp) return -1;

        fprintf(fp, "1\nNEW CONTACT\n%s\n%s\n", "\0", "\0");

        fclose(fp);
    }

    if (read_contacts_file()) return -1;

    return 0;
}

int free_contacts()
{
    for (int i = 0; i < contacts_size; ++i) {
        free(contacts[i]->name);
        free(contacts[i]->email);
        free(contacts[i]->phone);
        free(contacts[i]);
    }

    free(contacts);

    return 0;
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

static int compare_contacts(const void *p1, const void *p2)
{
    return strcasecmp((*(contact_t**) p1)->name, (*(contact_t**) p2)->name);
}

static int sort_contacts()
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
        idx = strcasecmp(name, contacts[pos]->name);

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
    char line[128];

    fp = fopen(CONTACTS_PATH, "r");
    if (!fp) return -1;

    fscanf(fp, "%d\n", &contacts_size);
    contacts = malloc(sizeof(contact_t*) * contacts_size);
    if (!contacts) return -1;

    for (int i = 0; i < contacts_size; ++i) {

        contacts[i] = malloc(sizeof(contact_t));
        if (!contacts[i]) return -1;

        fgets(line, 128, fp);
        contacts[i]->name = strndup(line, sizeof(char) * (strlen(line) - 1));
        if (!contacts[i]->name) return -1;

        fgets(line, 128, fp);
        contacts[i]->email = strndup(line, sizeof(char) * (strlen(line) - 1));
        if (!contacts[i]->email) return -1;

        fgets(line, 128, fp);
        contacts[i]->phone = strndup(line, sizeof(char) * (strlen(line) - 1));
        if (!contacts[i]->email) return -1;
    }

    fclose(fp);

    sort_contacts();

    return 0;
}

int write_contacts_file()
{
    FILE *fp;

    fp = fopen(CONTACTS_PATH, "w");
    if (!fp) return -1;

    sort_contacts();

    fprintf(fp, "%d\n", contacts_size);
    for (int i = 0; i < contacts_size; ++i) {
        fprintf(fp, "%s\n%s\n%s\n", contacts[i]->name, contacts[i]->email,
                contacts[i]->phone);
    }

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
