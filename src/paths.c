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

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "util.h"
#include "paths.h"

int init_paths()
{
    if (!getenv("XDG_DATA_HOME") || !strlen(getenv("XDG_DATA_HOME"))) {
        DATA_DIR = xrealloc(xstrdup(getenv("HOME")),
                           sizeof(char) * (strlen(getenv("HOME"))
                                           + strlen("/.local/share/corgan/")
                                           + 1));

        DATA_DIR = strcat(DATA_DIR, "/.local/");
        mkdir(DATA_DIR, 0700);

        DATA_DIR = strcat(DATA_DIR, "share/");
        mkdir(DATA_DIR, 0700);

        DATA_DIR = strcat(DATA_DIR, "corgan/");
        mkdir(DATA_DIR, 0700);
    } else {
        DATA_DIR = xrealloc(xstrdup(getenv("XDG_DATA_HOME")),
                           sizeof(char) * (strlen(getenv("XDG_DATA_HOME"))
                                           + strlen("/corgan/") + 1));

        DATA_DIR = strcat(DATA_DIR, "/corgan/");
        mkdir(DATA_DIR, 0700);
    }

    CONTACTS_PATH = xrealloc(xstrdup(DATA_DIR),
                            sizeof(char) * (strlen(DATA_DIR)
                                            + strlen("contacts") + 1));
    SCHEDULE_PATH = xrealloc(xstrdup(DATA_DIR),
                            sizeof(char) * (strlen(DATA_DIR)
                                            + strlen("contacts") + 1));
    EXPORT_PATH = xrealloc(xstrdup(DATA_DIR),
                          sizeof(char) * (strlen(DATA_DIR)
                                          + strlen("contacts.vcf") + 1));

    CONTACTS_PATH = strcat(CONTACTS_PATH, "contacts");
    SCHEDULE_PATH = strcat(SCHEDULE_PATH, "schedule");
    EXPORT_PATH = strcat(EXPORT_PATH, "contacts.vcf");

    return 0;
}

int free_paths()
{
    free(DATA_DIR);
    free(CONTACTS_PATH);
    free(SCHEDULE_PATH);
    free(EXPORT_PATH);

    return 0;
}
