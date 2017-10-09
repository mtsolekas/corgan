AM_CPPFLAGS = $(GTK_CFLAGS) -DLOCALEDIR=\"$(localedir)\"
AM_CFLAGS = -Wall -Wextra -std=gnu11
AM_LDFLAGS = $(CORGAN_LIBS) $(GTK_LIBS) @LIBINTL@ -rdynamic

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

nodist_corgan_SOURCES = $(BUILT_SOURCES)
BUILT_SOURCES = resources.c

EXTRA_DIST = corgan.glade
CLEANFILES = $(BUILT_SOURCES)

if DEBUG
AM_CFLAGS += -fsanitize=address -fsanitize=undefined \
		     -fno-omit-frame-pointer -Og
else
AM_CFLAGS += -Ofast -march=native
endif

resources.c:
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $(srcdir)/gresource.xml
