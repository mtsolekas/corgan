/*
 * Copyright (C) 2017,2021 Marios Tsolekas <marios.tsolekas@gmail.com>
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

#ifndef CORGAN_H
#define CORGAN_H

#include <gtk/gtk.h>

extern GtkBuilder *builder;

extern GtkTextBuffer *sched_buf;

extern GtkListStore *names_list;
extern GtkTreeView *contacts_view;
extern GtkTreeSelection *selection;
extern GtkEntry *name_entry, *email_entry, *phone_entry;

#endif
