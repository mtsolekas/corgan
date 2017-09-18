AM_CPPFLAGS = $(GTK_CFLAGS) $(DBGFLAGS) $(OPTFLAGS)
AM_CFLAGS = $(DBGFLAGS) -Wall -Werror -Wpedantic -std=gnu11
AM_LDFLAGS = $(CORGAN_LIBS) $(GTK_LIBS) -rdynamic

bin_PROGRAMS = corgan
corgan_SOURCES = corgan.c \
				 corgan.h \
				 corgan_contacts.c \
				 corgan_contacts.h \
				 corgan_paths.c \
				 corgan_paths.h \
				 corgan_schedule.c \
				 corgan_schedule.h \
				 corgan_signals.c \
				 corgan_signals.h

if DEBUG
DBGFLAGS = -fsanitize=address -fsanitize=undefined \
		   -fno-omit-frame-pointer -Og
OPTFLAGS =
else
DBGFLAGS =
OPTFLAGS = -O3 -march=native
endif

