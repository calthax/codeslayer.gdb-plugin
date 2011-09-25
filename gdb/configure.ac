AC_PREREQ(2.59)
AC_INIT([GdbCodeSlayerPlugin], [1.0.0], [jeff.johnston.mn@gmail.com])
AM_INIT_AUTOMAKE

AC_CONFIG_MACRO_DIR([m4])

#Initialize libtool
LT_PREREQ([2.2.6])
LT_INIT([dlopen])

# Check for programs
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O

# Dependencies
GTK_REQUIRED_VERSION=2.20.0

AC_SUBST(GTK_REQUIRED_VERSION)

PKG_CHECK_MODULES(GDBCODESLAYERPLUGIN, [
    glib-2.0 >= 2.24.0
    gtk+-2.0 >= $GTK_REQUIRED_VERSION
    gtksourceview-2.0 >= 2.10.0
    codeslayer >= 1.2.0
])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT

echo "

Configuration:

	Source code location:   ${srcdir}
	Compiler:               ${CC}
	Prefix:			${prefix}
"
