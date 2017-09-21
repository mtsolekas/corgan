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

#ifndef __CORGAN_H
#define __CORGAN_H

#include <gtk/gtk.h>

GtkBuilder *builder;

GtkTextBuffer *sched_buf;

GtkListStore *names_list;
GtkTreeView *contacts_view;
GtkTreeSelection *selection;
GtkEntry *name_entry, *email_entry, *phone_entry;

#endif
