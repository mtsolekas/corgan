#ifndef __CORGAN_DATA_H
#define __CORGAN_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

char *CONTACTS_PATH;
char *SCHEDULE_PATH;

char **contacts;
unsigned int contacts_size;

char *sched;

int init_data();
void free_data();

int new_contact();
int del_contact(int pos);

int entry_length(char *line);

int read_contacts_file();
int write_contacts_file();

int read_schedule_file();
int write_schedule_file();

#endif
