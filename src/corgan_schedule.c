/*
 * Copyright (C) 2017 Marios Tsolekas <marios.tsolekas@gmail.com>
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

#include "corgan_paths.h"
#include "corgan_schedule.h"

int init_schedule()
{
    FILE *fp;

    if (access(SCHEDULE_PATH, F_OK | R_OK | W_OK)) {
        fp = fopen(SCHEDULE_PATH, "w");
        if (!fp) return -1;

        fprintf(fp, "%s", "\0");

        fclose(fp);
    }

    if (read_schedule_file()) return -1;

    return 0;
}

int free_schedule()
{
    free(sched);

    return 0;
}

int read_schedule_file()
{
    FILE *fp;
    char *line, *pointer_save;
    unsigned int size;

    fp = fopen(SCHEDULE_PATH, "r");
    if (!fp) return -1;

    size = 100;
    sched = malloc(sizeof(char) * size);
    if (!sched) return -1;

    line = malloc(sizeof(char) * 100);
    pointer_save = line;
    if (!line) return -1;

    sched[0] = '\0';

    while ((line = fgets(line,100,fp))) {
        if (strlen(sched) + strlen(line) >= size) {
            size += 100;
            sched = realloc(sched, sizeof(char) * size);
            if (!sched) return -1;
        }

        sched = strcat(sched, strdup(line));
    }

    free(pointer_save);

    fclose(fp);

    sched = realloc(sched, sizeof(char) * strlen(sched)+1);
    if (!sched) return -1;

    return 0;
}

int write_schedule_file()
{
    FILE *fp;

    fp = fopen(SCHEDULE_PATH, "w");
    if (!fp) return -1;

    fputs(sched, fp);
    fclose(fp);

    return 0;
}
