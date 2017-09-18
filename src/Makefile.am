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

corgan_CPPFLAGS = $(GTK_CFLAGS)
DBGFLAGS = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
DBGFLAGS += -Og -ggdb3
corgan_CFLAGS = $(DBGFLAGS) -Wall -Werror -Wpedantic -std=gnu11
corgan_LDFLAGS = $(CORGAN_LIBS) $(GTK_LIBS) -rdynamic
