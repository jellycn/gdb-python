/* General utility routines for the remote server for GDB.
   Copyright (C) 1986, 1989, 1993, 1995, 1996, 1997, 1999, 2000, 2002, 2003,
   2007, 2008, 2009, 2010 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "server.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_MALLOC_H
#include <malloc.h>
#endif

/* Generally useful subroutines used throughout the program.  */

static void malloc_failure (size_t size) ATTR_NORETURN;

static void
malloc_failure (size_t size)
{
  fprintf (stderr, "gdbserver: ran out of memory while trying to allocate %lu bytes\n",
	   (unsigned long) size);
  exit (1);
}

/* Allocate memory without fail.
   If malloc fails, this will print a message to stderr and exit.  */

void *
xmalloc (size_t size)
{
  void *newmem;

  if (size == 0)
    size = 1;
  newmem = malloc (size);
  if (!newmem)
    malloc_failure (size);

  return newmem;
}

/* Reallocate memory without fail.  This works like xmalloc. */

void *
xrealloc (void *ptr, size_t size)
{
  void *val;

  if (size == 0)
    size = 1;

  if (ptr != NULL)
    val = realloc (ptr, size);	/* OK: realloc */
  else
    val = malloc (size);	/* OK: malloc */
  if (val == NULL)
    malloc_failure (size);

  return val;
}

/* Allocate memory without fail and set it to zero.
   If malloc fails, this will print a message to stderr and exit.  */

void *
xcalloc (size_t nelem, size_t elsize)
{
  void *newmem;

  if (nelem == 0 || elsize == 0)
    nelem = elsize = 1;

  newmem = calloc (nelem, elsize);
  if (!newmem)
    malloc_failure (nelem * elsize);

  return newmem;
}

/* Copy a string into a memory buffer.
   If malloc fails, this will print a message to stderr and exit.  */

char *
xstrdup (const char *s)
{
  char *ret = strdup (s);
  if (ret == NULL)
    malloc_failure (strlen (s) + 1);
  return ret;
}

/* Free a standard argv vector.  */

void
freeargv (char **vector)
{
  char **scan;

  if (vector != NULL)
    {
      for (scan = vector; *scan != NULL; scan++)
	{
	  free (*scan);
	}
      free (vector);
    }
}

/* Print the system error message for errno, and also mention STRING
   as the file name for which the error was encountered.
   Then return to command level.  */

void
perror_with_name (const char *string)
{
  const char *err;
  char *combined;

  err = strerror (errno);
  if (err == NULL)
    err = "unknown error";

  combined = (char *) alloca (strlen (err) + strlen (string) + 3);
  strcpy (combined, string);
  strcat (combined, ": ");
  strcat (combined, err);

  error ("%s.", combined);
}

/* Print an error message and return to command level.
   STRING is the error message, used as a fprintf string,
   and ARG is passed as an argument to it.  */

void
error (const char *string,...)
{
  extern jmp_buf toplevel;
  va_list args;
  va_start (args, string);
  fflush (stdout);
  vfprintf (stderr, string, args);
  fprintf (stderr, "\n");
  longjmp (toplevel, 1);
}

/* Print an error message and exit reporting failure.
   This is for a error that we cannot continue from.
   STRING and ARG are passed to fprintf.  */

/* VARARGS */
void
fatal (const char *string,...)
{
  va_list args;
  va_start (args, string);
  fprintf (stderr, "gdbserver: ");
  vfprintf (stderr, string, args);
  fprintf (stderr, "\n");
  va_end (args);
  exit (1);
}

/* VARARGS */
void
warning (const char *string,...)
{
  va_list args;
  va_start (args, string);
  fprintf (stderr, "gdbserver: ");
  vfprintf (stderr, string, args);
  fprintf (stderr, "\n");
  va_end (args);
}

/* Report a problem internal to GDBserver, and exit.  */

void
internal_error (const char *file, int line, const char *fmt, ...)
{
  va_list args;
  va_start (args, fmt);

  fprintf (stderr,  "\
%s:%d: A problem internal to GDBserver has been detected.\n", file, line);
  vfprintf (stderr, fmt, args);
  fprintf (stderr, "\n");
  va_end (args);
  exit (1);
}

/* Temporary storage using circular buffer.  */
#define NUMCELLS 4
#define CELLSIZE 50

/* Return the next entry in the circular buffer.  */

static char *
get_cell (void)
{
  static char buf[NUMCELLS][CELLSIZE];
  static int cell = 0;
  if (++cell >= NUMCELLS)
    cell = 0;
  return buf[cell];
}

/* Stdarg wrapper around vsnprintf.
   SIZE is the size of the buffer pointed to by STR.  */

static int
xsnprintf (char *str, size_t size, const char *format, ...)
{
  va_list args;
  int ret;

  va_start (args, format);
  ret = vsnprintf (str, size, format, args);
  va_end (args);

  return ret;
}

/* Convert a CORE_ADDR into a HEX string, like %lx.
   The result is stored in a circular static buffer, NUMCELLS deep.  */

char *
paddress (CORE_ADDR addr)
{
  char *str = get_cell ();
  xsnprintf (str, CELLSIZE, "%lx", (long) addr);
  return str;
}

static char *
decimal2str (char *sign, ULONGEST addr, int width)
{
  /* Steal code from valprint.c:print_decimal().  Should this worry
     about the real size of addr as the above does? */
  unsigned long temp[3];
  char *str = get_cell ();

  int i = 0;
  do
    {
      temp[i] = addr % (1000 * 1000 * 1000);
      addr /= (1000 * 1000 * 1000);
      i++;
      width -= 9;
    }
  while (addr != 0 && i < (sizeof (temp) / sizeof (temp[0])));

  width = 9;
  if (width < 0)
    width = 0;

  switch (i)
    {
    case 1:
      xsnprintf (str, CELLSIZE, "%s%0*lu", sign, width, temp[0]);
      break;
    case 2:
      xsnprintf (str, CELLSIZE, "%s%0*lu%09lu", sign, width,
		 temp[1], temp[0]);
      break;
    case 3:
      xsnprintf (str, CELLSIZE, "%s%0*lu%09lu%09lu", sign, width,
		 temp[2], temp[1], temp[0]);
      break;
    default:
      internal_error (__FILE__, __LINE__,
		      "failed internal consistency check");
    }

  return str;
}

/* %u for ULONGEST.  The result is stored in a circular static buffer,
   NUMCELLS deep.  */

char *
pulongest (ULONGEST u)
{
  return decimal2str ("", u, 0);
}

/* %d for LONGEST.  The result is stored in a circular static buffer,
   NUMCELLS deep.  */

char *
plongest (LONGEST l)
{
  if (l < 0)
    return decimal2str ("-", -l, 0);
  else
    return decimal2str ("", l, 0);
}

/* Eliminate warning from compiler on 32-bit systems.  */
static int thirty_two = 32;

/* Convert a ULONGEST into a HEX string, like %lx.  The result is
   stored in a circular static buffer, NUMCELLS deep.  */

char *
phex_nz (ULONGEST l, int sizeof_l)
{
  char *str;

  switch (sizeof_l)
    {
    case 8:
      {
	unsigned long high = (unsigned long) (l >> thirty_two);
	str = get_cell ();
	if (high == 0)
	  xsnprintf (str, CELLSIZE, "%lx",
		     (unsigned long) (l & 0xffffffff));
	else
	  xsnprintf (str, CELLSIZE, "%lx%08lx", high,
		     (unsigned long) (l & 0xffffffff));
	break;
      }
    case 4:
      str = get_cell ();
      xsnprintf (str, CELLSIZE, "%lx", (unsigned long) l);
      break;
    case 2:
      str = get_cell ();
      xsnprintf (str, CELLSIZE, "%x", (unsigned short) (l & 0xffff));
      break;
    default:
      str = phex_nz (l, sizeof (l));
      break;
    }

  return str;
}
