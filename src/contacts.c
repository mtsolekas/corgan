/*
 * Copyright (C) 2017-2019,2021 Marios Tsolekas <marios.tsolekas@gmail.com>
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
#include <gmodule.h>

#include "util.h"
#include "paths.h"
#include "contacts.h"

contact_t **contacts;
int contacts_size;

static int compare_contacts(const void *, const void *);

int init_contacts()
{
    FILE *fp;

    if (access(CONTACTS_PATH, F_OK | R_OK | W_OK)) {
        if (!(fp = fopen(CONTACTS_PATH, "w")))
            return -1;

        fprintf(fp, "1\nNEW CONTACT\n%s\n%s\n", "\0", "\0");

        fclose(fp);
    }

    if (read_contacts_file())
        return -1;

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
    contacts = xrealloc(contacts, sizeof(contact_t *) * contacts_size);

    contacts[contacts_size - 1] = xmalloc(sizeof(contact_t));

    contacts[contacts_size - 1]->name = xstrdup("NEW CONTACT");
    contacts[contacts_size - 1]->email = xstrdup("\0");
    contacts[contacts_size - 1]->phone = xstrdup("\0");

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

    contacts = xrealloc(contacts, sizeof(contact_t *) * contacts_size);

    sort_contacts();

    return 0;
}

static int compare_contacts(const void *p1, const void *p2)
{
    int res;
    char *s1, *s2;

    s1 = g_utf8_casefold((*(contact_t **) p1)->name, -1);
    s2 = g_utf8_casefold((*(contact_t **) p2)->name, -1);
    res = strcmp(s1, s2);

    free(s1);
    free(s2);

    return res;
}

int sort_contacts()
{
    if (!contacts_size)
        return 0;

    qsort(contacts, contacts_size, sizeof(contact_t *), compare_contacts);

    return 0;
}

int search_contacts(const char *name)
{
    int idx, prev_pos, pos, lbound, ubound;
    char *s1, *s2;

    lbound = 0;
    ubound = contacts_size;
    prev_pos = -1;
    pos = (ubound - lbound) / 2;

    while (prev_pos != pos) {
        prev_pos = pos;

        s1 = g_utf8_casefold(name, -1);
        s2 = g_utf8_casefold(contacts[pos]->name, -1);

        idx = strcmp(s1, s2);

        free(s1);
        free(s2);

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

    if (!(fp = fopen(CONTACTS_PATH, "r")))
        return -1;

    if ((fscanf(fp, "%d\n", &contacts_size)) <= 0)
        return -1;

    contacts = xmalloc(sizeof(contact_t *) * contacts_size);

    for (int i = 0; i < contacts_size; ++i) {

        contacts[i] = xmalloc(sizeof(contact_t));

        contacts[i]->name = freadline(fp);
        contacts[i]->email = freadline(fp);
        contacts[i]->phone = freadline(fp);
    }

    fclose(fp);

    sort_contacts();

    return 0;
}

int write_contacts_file()
{
    FILE *fp;

    if (!(fp = fopen(CONTACTS_PATH, "w")))
        return -1;

    sort_contacts();

    fprintf(fp, "%d\n", contacts_size);
    for (int i = 0; i < contacts_size; ++i)
        fprintf(fp, "%s\n%s\n%s\n", contacts[i]->name, contacts[i]->email,
                contacts[i]->phone);

    fclose(fp);

    return 0;
}

int export_contacts_vcard()
{
    FILE *fp;
    char *name, *fname, *lname, *tmp;

    if (!(fp = fopen(EXPORT_PATH, "w")))
        return -1;

    for (int i = 0; i < contacts_size; ++i) {
        fprintf(fp, "BEGIN:VCARD\nVERSION:2.1\n");

        name = xstrdup(contacts[i]->name);
        fname = strtok(name, " ");
        lname = strtok(NULL, " ");
        while ((tmp = strtok(NULL, " ")))
            lname = tmp;

        fprintf(fp, "FN:%s\nN:%s;%s\n",
                contacts[i]->name, lname, fname);

        if (contacts[i]->email[0] != '\0')
            fprintf(fp, "EMAIL:%s\n", contacts[i]->email);
        if (contacts[i]->phone[0] != '\0')
            fprintf(fp, "TEL;CELL;VOICE:%s\n", contacts[i]->phone);

        fprintf(fp, "END:VCARD\n");
        free(name);
    }

    fclose(fp);
    return 0;
}
