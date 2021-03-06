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

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

void *xmalloc(size_t);
void *xrealloc(void *, size_t);

char *xstrdup(const char *);
char *xstrndup(const char *, size_t);

char *freadline(FILE *);

int mkpath(const char *, mode_t);

#endif
