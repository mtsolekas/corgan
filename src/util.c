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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "util.h"

void *xmalloc(size_t size)
{
    void *ret;

    ret = malloc(size);
    if (!ret && size) {
        fprintf(stderr, "%s:%d out of memory, ", __FILE__, __LINE__);
        abort();
    }

    return ret;
}

void *xrealloc(void *ptr, size_t size)
{
    void *ret;

    ret = realloc(ptr, size);
    if (!ret && size) {
        fprintf(stderr, "%s:%d out of memory, ", __FILE__, __LINE__);
        abort();
    }

    return ret;
}


char *xstrdup(const char *s)
{
    char *ret;

    ret = strdup(s);
    if (!ret) {
        fprintf(stderr, "%s:%d out of memory, ", __FILE__, __LINE__);
        abort();
    }

    return ret;
}

char *xstrndup(const char *s, size_t n)
{
    char *ret;

    ret = strndup(s, n);
    if (!ret) {
        fprintf(stderr, "%s:%d out of memory, ", __FILE__, __LINE__);
        abort();
    }

    return ret;
}

char *freadline(FILE * fp)
{
    char c;
    size_t lsize, i;
    char *line;

    lsize = 32;
    line = xmalloc(lsize);

    for (i = 0; (c = fgetc(fp)) != EOF && c != '\n'; ++i) {
        if (i >= lsize - 1) {
            lsize += 32;
            line = xrealloc(line, lsize);
        }
        line[i] = c;
    }

    line = xrealloc(line, i + 1);
    line[i] = '\0';

    return line;
}

int mkpath(const char *path, mode_t mode)
{
    int retcode, ret;
    char *pathincr, *token, *tmp;

    retcode = 0;

    pathincr = xmalloc(strlen(path) + 1);
    pathincr[0] = '\0';
    if (path[0] == '/')
        pathincr = strcat(pathincr, "/");

    tmp = xstrdup(path);
    token = strtok(tmp, "/");

    pathincr = strcat(pathincr, token);
    retcode = mkdir(pathincr, mode);

    while ((token = strtok(NULL, "/"))) {
        pathincr = strcat(pathincr, "/");
        pathincr = strcat(pathincr, token);
        ret = mkdir(pathincr, mode);
        retcode = !retcode ? ret : -1;
    }

    free(tmp);
    free(pathincr);

    return retcode;
}
