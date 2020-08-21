AC_DEFUN([PDNS_ENABLE_BENCHMARKS], [
  AC_REQUIRE([PDNS_CHECK_BENCHMARK])
  AC_MSG_CHECKING([whether we will be building benchmark tools])
  AC_ARG_ENABLE([benchmarks],
    [AS_HELP_STRING([--enable-benchmarks], [if we should build benchmark tools @<:@default=no@:>@])],
    [enable_benchmarks=$enableval],
    [enable_benchmarks=no]
    )
  AC_MSG_RESULT([$enable_benchmarks])

  AM_CONDITIONAL([BENCHMARKS], [test "x$enable_benchmarks" != "xno"])
])
