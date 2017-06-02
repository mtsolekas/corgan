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

char *schedule;

void init_data();
void free_data();

int read_contacts_file();
int write_contacts_file();

int read_schedule_file();
int write_schedule_file();

#endif
