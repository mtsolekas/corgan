AC_INIT([corgan], [0.10], [https://github.com/mtsolekas/corgan/issues])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([src/corgan.c])
AC_CONFIG_HEADERS([config.h])
AM_SILENT_RULES([yes])

AC_PROG_CC

AC_ARG_ENABLE([debug],
[ --enable-debug Turn on debugging],
[case "${enableval}" in
 yes) debug=true ;;
 no) debug=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac], [debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

PKG_PROG_PKG_CONFIG
AC_PATH_PROG([GLIB_COMPILE_RESOURCES], [glib-compile-resources])

PKG_CHECK_MODULES(GTK, gtk+-3.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strdup strndup])

AC_CONFIG_FILES([Makefile src/Makefile data/Makefile])

AC_OUTPUT
