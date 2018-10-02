/*
 * Copyright (C) 2018 Marios Tsolekas <marios.tsolekas@gmail.com>
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
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "paths.h"

int test_init_noxdg();
int test_init_xdg();

int main()
{
    mkdir("test_home", 0777);
    assert(!test_init_noxdg());

    mkdir("test_home/.local/data", 0777);
    assert(!test_init_xdg());

    return 0;
}

int test_init_noxdg()
{
    struct stat sb;

    unsetenv("XDG_DATA_HOME");
    setenv("HOME", "test_home", 1);

    if (init_paths())
        return -1;

    if (strcmp(DATA_DIR, "test_home/.local/share/corgan/")
        || strcmp(CONTACTS_PATH, "test_home/.local/share/corgan/contacts")
        || strcmp(SCHEDULE_PATH, "test_home/.local/share/corgan/schedule")
        || strcmp(EXPORT_PATH, "test_home/.local/share/corgan/contacts.vcf"))
        return -1;

    if (stat(DATA_DIR, &sb))
        return -1;

    if (!S_ISDIR(sb.st_mode))
        return -1;

    if (free_paths()) return -1;

    return 0;
}

int test_init_xdg()
{
    struct stat sb;

    setenv("XDG_DATA_HOME", "test_home/.local/data", 1);

    if (init_paths())
        return -1;

    if (strcmp(DATA_DIR, "test_home/.local/data/corgan/")
        || strcmp(CONTACTS_PATH, "test_home/.local/data/corgan/contacts")
        || strcmp(SCHEDULE_PATH, "test_home/.local/data/corgan/schedule")
        || strcmp(EXPORT_PATH, "test_home/.local/data/corgan/contacts.vcf"))
        return -1;

    if (stat(DATA_DIR, &sb))
        return -1;

    if (!S_ISDIR(sb.st_mode))
        return -1;

    if (free_paths()) return -1;

    return 0;
}
