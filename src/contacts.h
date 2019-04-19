/*
 * Copyright (C) 2017-2019 Marios Tsolekas <marios.tsolekas@gmail.com>
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

#ifndef CONTACTS_H
#define CONTACTS_H

typedef struct contact_type {
    char *name;
    char *email;
    char *phone;
} contact_t;

contact_t **contacts;
int contacts_size;

int init_contacts();
int free_contacts();

int new_contact();
int del_contact(int);

int search_contacts(const char *);
int sort_contacts();

int read_contacts_file();
int write_contacts_file();

int export_contacts_vcard();

#endif
