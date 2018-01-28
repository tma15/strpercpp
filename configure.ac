#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([strpercpp], [0.0.0], [takuyamakino15@gmail.com])
AM_INIT_AUTOMAKE()

AC_PROG_LIBTOOL

# Checks for programs.
#AC_PROG_CXX([-std=c++0x])
#AC_PROG_CXX([-std=c++11 -g -pg])
AC_PROG_CXX([])

# Checks for libraries.
AC_ARG_WITH([gtest],
	    AS_HELP_STRING([--with-gtest=PATH], [path to googletest]),
	    [GTEST_PATH=$withval],
	    [GTEST_PATH=no]
	    )
AC_SUBST(GTEST_PATH)
AM_CONDITIONAL(HAS_GTEST, test x"${GTEST_PATH}" != xno)
AC_MSG_NOTICE([GTEST_PATH: ${GTEST_PATH}])
AC_MSG_NOTICE([HAS_GTEST: ${HAS_GTEST}])

AC_ARG_WITH([gperf],
	    AS_HELP_STRING([--with-gperf=PATH], [path to gperf-tools]),
	    [GPERF_PATH=$withval],
	    [GPERF_PATH=no]
	    )
AC_SUBST(GPERF_PATH)
AM_CONDITIONAL(HAS_GPERF, test x"${GPERF_PATH}" != xno)
AC_MSG_NOTICE([GPERF_PATH: ${GPERF_PATH}])
AC_MSG_NOTICE([HAS_GPERF: ${HAS_GPERF}])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.


AC_CONFIG_FILES([
		 Makefile
		 src/Makefile
		 tests/Makefile
		 ])

AC_OUTPUT
