AM_CPPFLAGS = $(GTK_CFLAGS) -DLOCALEDIR=\"$(localedir)\"
AM_CFLAGS = -Wall -Wextra -Wpedantic -std=gnu11 -O3 -march=native
AM_LDFLAGS = $(CORGAN_LIBS) $(GTK_LIBS) @LIBINTL@ -rdynamic

if DEBUG
AM_CFLAGS += -fsanitize=address -fsanitize=undefined \
		     -fno-omit-frame-pointer -g3
endif

TESTS = $(check_PROGRAMS) test_cleanup
test_cleanup: $(check_PROGRAMS)
check_PROGRAMS = test_paths
test_paths_SOURCES = test_paths.c paths.c paths.h

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
				 signals.h

nodist_corgan_SOURCES = $(BUILT_SOURCES)
BUILT_SOURCES = resources.c

EXTRA_DIST = window.ui gresource.xml
CLEANFILES = $(BUILT_SOURCES)

resources.c:
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $(srcdir)/gresource.xml
