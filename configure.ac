AC_INIT([corgan], [0.10], [https://github.com/mtsolekas/corgan/issues])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/corgan.c])
AC_CONFIG_HEADERS([config.h])
AM_SILENT_RULES([yes])

AC_PROG_CC

PKG_CHECK_MODULES(GTK, gtk+-3.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

AC_CHECK_HEADERS([stdlib.h strings.h unistd.h])

AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strdup strndup])

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
