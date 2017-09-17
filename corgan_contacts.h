#ifndef __CORGAN_CONTACTS_H
#define __CORGAN_CONTACTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
int del_contact(int pos);

int compare_contacts(const void *p1, const void *p2);
int sort_contacts();
int search_contacts(const char *name);

int read_contacts_file();
int write_contacts_file();

int export_contacts_vcard();

#endif
