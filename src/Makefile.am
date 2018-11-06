AM_CPPFLAGS = $(GTK_CFLAGS) -DLOCALEDIR=\"$(localedir)\"
AM_CFLAGS = -Wall -Wextra -Wpedantic -std=gnu11 -O3 -march=native
AM_LDFLAGS = $(CORGAN_LIBS) $(GTK_LIBS) @LIBINTL@ -rdynamic

if DEBUG
AM_CFLAGS += -fsanitize=address -fsanitize=undefined \
             -fno-omit-frame-pointer -g3
endif

TESTS = $(check_PROGRAMS) test_cleanup
check_PROGRAMS = test_paths test_schedule test_contacts
test_paths_SOURCES = test_paths.c \
                     paths.c \
                     paths.h \
                     util.c \
                     util.h
test_schedule_SOURCES = test_schedule.c \
                        schedule.c \
                        schedule.h \
                        paths.c \
                        paths.h \
                        util.c \
                        util.h
test_contacts_SOURCES = test_contacts.c \
                        contacts.c \
                        contacts.h \
                        paths.c \
                        paths.h \
                        util.c \
                        util.h

bin_PROGRAMS = corgan
corgan_SOURCES = corgan.c \
                 corgan.h \
                 contacts.c \
                 contacts.h \
                 paths.c \
                 paths.h \
                 schedule.c \
                 schedule.h \
                 signals.c \
                 signals.h \
                 util.c \
                 util.h

nodist_corgan_SOURCES = $(BUILT_SOURCES)
BUILT_SOURCES = resources.c

EXTRA_DIST = window.ui gresource.xml test_cleanup
CLEANFILES = $(BUILT_SOURCES)

resources.c:
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $(srcdir)/gresource.xml
