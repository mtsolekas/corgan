#ifndef __CORGAN_PATHS_H
#define __CORGAN_PATHS_H

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

char *DATA_DIR;
char *CONTACTS_PATH, *SCHEDULE_PATH, *EXPORT_PATH;

int init_paths();
int free_paths();

#endif
