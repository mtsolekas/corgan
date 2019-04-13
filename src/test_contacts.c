/*
 * Copyright (C) 2018, 2019 Marios Tsolekas <marios.tsolekas@gmail.com>
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

#include "util.h"
#include "paths.h"
#include "contacts.h"

int main()
{
    unsetenv("XDG_DATA_HOME");
    setenv("HOME", "test_home", 1);

    assert(!init_paths());

    assert(!init_contacts());

    free(contacts[0]->name);
    contacts[0]->name = xstrdup("aaa");

    assert(!new_contact());

    assert(!write_contacts_file());
    assert(!free_contacts());

    assert(!read_contacts_file());

    assert(search_contacts("aaa") == 0);
    assert(search_contacts("NEW CONTACT") == 1);

    assert(!export_contacts_vcard());

    assert(contacts_size == 2);
    assert(!del_contact(1));

    assert(contacts_size == 1);
    assert(!del_contact(0));

    assert(contacts_size == 0);
    assert(!new_contact());
    assert(search_contacts("NEW CONTACT") == 0);

    assert(!free_paths());
    assert(!free_contacts());

    return EXIT_SUCCESS;
}
