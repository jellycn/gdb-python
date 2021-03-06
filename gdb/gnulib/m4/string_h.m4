# Configure a GNU-like replacement for <string.h>.

# Copyright (C) 2007, 2008, 2009, 2010 Free Software Foundation, Inc.
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 4

# Written by Paul Eggert.

AC_DEFUN([gl_HEADER_STRING_H],
[
  dnl Use AC_REQUIRE here, so that the default behavior below is expanded
  dnl once only, before all statements that occur in other macros.
  AC_REQUIRE([gl_HEADER_STRING_H_BODY])
])

AC_DEFUN([gl_HEADER_STRING_H_BODY],
[
  AC_REQUIRE([AC_C_RESTRICT])
  AC_REQUIRE([gl_HEADER_STRING_H_DEFAULTS])
  gl_CHECK_NEXT_HEADERS([string.h])
])

AC_DEFUN([gl_STRING_MODULE_INDICATOR],
[
  dnl Use AC_REQUIRE here, so that the default settings are expanded once only.
  AC_REQUIRE([gl_HEADER_STRING_H_DEFAULTS])
  GNULIB_[]m4_translit([$1],[abcdefghijklmnopqrstuvwxyz./-],[ABCDEFGHIJKLMNOPQRSTUVWXYZ___])=1
])

AC_DEFUN([gl_HEADER_STRING_H_DEFAULTS],
[
  GNULIB_MEMMEM=0;      AC_SUBST([GNULIB_MEMMEM])
  GNULIB_MEMPCPY=0;     AC_SUBST([GNULIB_MEMPCPY])
  GNULIB_MEMRCHR=0;     AC_SUBST([GNULIB_MEMRCHR])
  GNULIB_STPCPY=0;      AC_SUBST([GNULIB_STPCPY])
  GNULIB_STPNCPY=0;     AC_SUBST([GNULIB_STPNCPY])
  GNULIB_STRCHRNUL=0;   AC_SUBST([GNULIB_STRCHRNUL])
  GNULIB_STRDUP=0;      AC_SUBST([GNULIB_STRDUP])
  GNULIB_STRNDUP=0;     AC_SUBST([GNULIB_STRNDUP])
  GNULIB_STRNLEN=0;     AC_SUBST([GNULIB_STRNLEN])
  GNULIB_STRPBRK=0;     AC_SUBST([GNULIB_STRPBRK])
  GNULIB_STRSEP=0;      AC_SUBST([GNULIB_STRSEP])
  GNULIB_STRSTR=0;      AC_SUBST([GNULIB_STRSTR])
  GNULIB_STRCASESTR=0;  AC_SUBST([GNULIB_STRCASESTR])
  GNULIB_STRTOK_R=0;    AC_SUBST([GNULIB_STRTOK_R])
  GNULIB_MBSLEN=0;      AC_SUBST([GNULIB_MBSLEN])
  GNULIB_MBSNLEN=0;     AC_SUBST([GNULIB_MBSNLEN])
  GNULIB_MBSCHR=0;      AC_SUBST([GNULIB_MBSCHR])
  GNULIB_MBSRCHR=0;     AC_SUBST([GNULIB_MBSRCHR])
  GNULIB_MBSSTR=0;      AC_SUBST([GNULIB_MBSSTR])
  GNULIB_MBSCASECMP=0;  AC_SUBST([GNULIB_MBSCASECMP])
  GNULIB_MBSNCASECMP=0; AC_SUBST([GNULIB_MBSNCASECMP])
  GNULIB_MBSPCASECMP=0; AC_SUBST([GNULIB_MBSPCASECMP])
  GNULIB_MBSCASESTR=0;  AC_SUBST([GNULIB_MBSCASESTR])
  GNULIB_MBSCSPN=0;     AC_SUBST([GNULIB_MBSCSPN])
  GNULIB_MBSPBRK=0;     AC_SUBST([GNULIB_MBSPBRK])
  GNULIB_MBSSPN=0;      AC_SUBST([GNULIB_MBSSPN])
  GNULIB_MBSSEP=0;      AC_SUBST([GNULIB_MBSSEP])
  GNULIB_MBSTOK_R=0;    AC_SUBST([GNULIB_MBSTOK_R])
  GNULIB_STRERROR=0;    AC_SUBST([GNULIB_STRERROR])
  GNULIB_STRSIGNAL=0;   AC_SUBST([GNULIB_STRSIGNAL])
  dnl Assume proper GNU behavior unless another module says otherwise.
  HAVE_DECL_MEMMEM=1;		AC_SUBST([HAVE_DECL_MEMMEM])
  HAVE_MEMPCPY=1;		AC_SUBST([HAVE_MEMPCPY])
  HAVE_DECL_MEMRCHR=1;		AC_SUBST([HAVE_DECL_MEMRCHR])
  HAVE_STPCPY=1;		AC_SUBST([HAVE_STPCPY])
  HAVE_STPNCPY=1;		AC_SUBST([HAVE_STPNCPY])
  HAVE_STRCHRNUL=1;		AC_SUBST([HAVE_STRCHRNUL])
  HAVE_DECL_STRDUP=1;		AC_SUBST([HAVE_DECL_STRDUP])
  HAVE_STRNDUP=1;		AC_SUBST([HAVE_STRNDUP])
  HAVE_DECL_STRNDUP=1;		AC_SUBST([HAVE_DECL_STRNDUP])
  HAVE_DECL_STRNLEN=1;		AC_SUBST([HAVE_DECL_STRNLEN])
  HAVE_STRPBRK=1;		AC_SUBST([HAVE_STRPBRK])
  HAVE_STRSEP=1;		AC_SUBST([HAVE_STRSEP])
  HAVE_STRCASESTR=1;		AC_SUBST([HAVE_STRCASESTR])
  HAVE_DECL_STRTOK_R=1;		AC_SUBST([HAVE_DECL_STRTOK_R])
  HAVE_DECL_STRERROR=1;		AC_SUBST([HAVE_DECL_STRERROR])
  HAVE_DECL_STRSIGNAL=1;	AC_SUBST([HAVE_DECL_STRSIGNAL])
  REPLACE_STRERROR=0;		AC_SUBST([REPLACE_STRERROR])
  REPLACE_STRSIGNAL=0;		AC_SUBST([REPLACE_STRSIGNAL])
  REPLACE_MEMMEM=0;		AC_SUBST([REPLACE_MEMMEM])
  REPLACE_STRCASESTR=0;		AC_SUBST([REPLACE_STRCASESTR])
  REPLACE_STRSTR=0;		AC_SUBST([REPLACE_STRSTR])
])
