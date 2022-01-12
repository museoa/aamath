/* aamath.cc -- part of aamath
 *
 * This program is copyright (C) 2005 Mauro Persano, and is free
 * software which is freely distributable under the terms of the
 * GNU public license, included as the file COPYING in this
 * distribution.  It is NOT public domain software, and any
 * redistribution not permitted by the GNU General Public License is
 * expressly forbidden without prior written permission from
 * the author.
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#ifdef USE_READLINE
#include <stdlib.h> // free()
#include <readline/readline.h>
#include <readline/history.h>
#endif
#include "version.h"

static char *aamath_prompt = "aamath> ";

#ifdef USE_READLINE
static char *line_read;
static char *cur_line_pos;
static bool eol_read;
#endif

extern int yyparse();

bool interactive = false;

int
get_input(char *buf, int max_size)
{
	int rv;

	if (interactive) {
#ifdef USE_READLINE
		int line_len = strlen(cur_line_pos);

		// XXX: fix the grammar so we don't need this ugly hack

		if (line_len == 0) {
			if (eol_read) {
				rv = 0;
			} else {
				// send a terminating '\n'
				rv = 1;
				buf[0] = '\n';
				eol_read = true;
			}
		} else {
			if (max_size > line_len)
				max_size = line_len;

			memcpy(buf, cur_line_pos, max_size);

			cur_line_pos += max_size;

			rv = max_size;
		}
#else
		int c;

		if ((c = getchar()) == EOF) {
			rv = 0;
		} else {
			rv = 1;
			buf[0] = c;
		}
#endif
	} else {
		rv = fread(buf, 1, max_size, stdin);
	}

	return rv;
}

void
prompt()
{
#ifndef USE_READLINE
	if (interactive) {
		printf("%s", aamath_prompt);
		fflush(stdout);
	}
#endif
}

void
banner()
{
	puts(VERSION);
	fflush(stdout);
}

void
yyerror(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	vprintf(str, args);

	putchar('\n');
}

int
main(int argc, char *argv[])
{
	interactive = isatty(0);

	int rv = 0;

	if (interactive) {
		banner();

#ifndef USE_READLINE
		prompt();

		rv = yyparse();
#else
		while ((line_read = readline(aamath_prompt)) != NULL) {
			if (*line_read) {
				if (!strcmp(line_read, "quit")) {
					free(line_read);
					break;
				}

				add_history(line_read);

				cur_line_pos = line_read;
				eol_read = false;

				rv = yyparse();
			}

			free(line_read);
		}
#endif
	} else {
		rv = yyparse();
	}

	return rv;
}
