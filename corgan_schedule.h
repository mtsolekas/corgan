#ifndef __CORGAN_SCHEDULE_H
#define __CORGAN_SCHEDULE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *sched;

int init_schedule();
int free_schedule();

int read_schedule_file();
int write_schedule_file();

#endif
