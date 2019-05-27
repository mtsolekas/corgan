AC_INIT([corgan], [0.1], [https://github.com/mtsolekas/corgan/issues])
AM_INIT_AUTOMAKE([-Wall -Werror foreign serial-tests])
AC_CONFIG_SRCDIR([src/corgan.c])
AC_CONFIG_HEADERS([config.h])
AM_SILENT_RULES([yes])

AC_PROG_CC

AC_ARG_ENABLE([debug],
[ --enable-debug[[=ARG]] Turn on debugging [[ARG=yes]]],
[case "${enableval}" in
 yes) debug=true ;;
 NULL) debug=true ;;
 no) debug=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac], [debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

PKG_PROG_PKG_CONFIG
AC_PATH_PROG([GLIB_COMPILE_RESOURCES], [glib-compile-resources])

PKG_CHECK_MODULES(GTK, gtk+-3.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

AC_CHECK_HEADERS([stdlib.h string.h unistd.h locale.h libintl.h])

AC_TYPE_SIZE_T

AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit mkdir setenv setlocale strdup strndup])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.7])

AC_CONFIG_FILES([Makefile src/Makefile data/Makefile po/Makefile.in])

AC_OUTPUT
