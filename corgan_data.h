#ifndef __CORGAN_DATA_H
#define __CORGAN_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

char *APP_DIR, *CONTACTS_PATH, *SCHEDULE_PATH, *EXPORT_PATH;

typedef struct contact_type {
    char *name;
    char *email;
    char *phone;
} contact_t;

contact_t **contacts;
int contacts_size;

char *sched;

int init_data();
void free_data();

int new_contact();
int del_contact(int pos);

int compare_contacts(const void *p1, const void *p2);
int sort_contacts();
int search_contacts(const char *name);

int line_length(char *line);

int read_contacts_file();
int write_contacts_file();

int read_schedule_file();
int write_schedule_file();

int export_contacts_vcard();

#endif
