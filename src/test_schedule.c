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
#include "schedule.h"

int main()
{
    char *test_sched;

    unsetenv("XDG_DATA_HOME");
    setenv("HOME", "test_home", 1);

    mkdir("test_home", 0777);
    assert(!init_paths());

    assert(!init_schedule());
    assert(!strcmp(sched, "\0"));
    free(sched);

    test_sched = "TEST\n";
    sched = test_sched;

    assert(!write_schedule_file());

    assert(!read_schedule_file());
    assert(!strcmp(sched, test_sched));

    assert(!free_paths());
    assert(!free_schedule());

    return 0;
}
