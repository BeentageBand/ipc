# Initialize
AC_INIT([ipc], [1.0], [beentageband@gmail.com])

# Autotools aux files
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Automake config files
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_FILES([Makefile])

# Safety check
AC_CONFIG_SRCDIR([ipc/ipc.c])

# Compiler flags
AC_PROG_CXX
AM_PROG_AR

# Libtool
LT_INIT
PKG_CHECK_MODULES([libgmock_main], [gmock_main])

# Output
AC_OUTPUT
