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

#include <util.h>

int main()
{
    char *mem;

    mem = xmalloc(10);
    assert(mem);
    free(mem);

    mem = xmalloc(0);
    if (mem)
        free(mem);

    mem = xrealloc(NULL, 10);
    assert(mem);

    mem = xrealloc(mem, 20);
    assert(mem);

    mem = xrealloc(mem, 0);
    if (mem)
        free(mem);

    mem = xstrdup("TEST_STRING");
    assert(mem);
    assert(!strcmp(mem, "TEST_STRING"));
    free(mem);

    mem = xstrndup("TEST_STRING", 4);
    assert(mem);
    assert(!strcmp(mem, "TEST"));
    free(mem);

    return EXIT_SUCCESS;
}
