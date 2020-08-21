AC_DEFUN([PDNS_CHECK_BENCHMARK], [
  PKG_CHECK_MODULES([BENCHMARK], [benchmark],
    [AC_DEFINE([HAVE_BENCHMARK], [1], [Define to 1 if you have google benchmark])],
    [AC_MSG_ERROR([Could not find google benchmark])]
  )
])
