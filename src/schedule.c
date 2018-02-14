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
#include "schedule.h"

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
    char line[128];

    fp = fopen(SCHEDULE_PATH, "r");
    if (!fp) return -1;

    fseek(fp, 0, SEEK_END);

    sched = malloc(ftell(fp) + sizeof(char));
    if (!sched) return -1;

    fseek(fp, 0, SEEK_SET);
    sched[0] = '\0';

    while (fgets(line,128,fp)) {
        sched = strcat(sched, line);
    }

    fclose(fp);

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
