%{
/* lexer.l -- part of aamath
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
#include "expr.h"
#include "parser.h"

#define YY_NO_UNPUT 1

#undef YY_INPUT

extern int get_input(char *buf, int max_size);

#define YY_INPUT(buf, result, max_size) { result = get_input(buf, max_size); }

%}

%option nounput

%%

[0-9]+			{
				yylval.constval = new Integer(yytext);
				return INTEGER;
			}
"..."			{ return ELLIPSIS; }
[0-9]*"."?[0-9]*([eE][+-]?[0-9]+)?("...")? {
				yylval.constval = new Float(yytext); 
				return FLOAT;
			}
quit			{ return QUIT; }
inf			{ return INF; }
pi			{ return PI; }
sqrt			{ return SQRT; }
root			{ return ROOT; }
int			{ return INT; }
sum			{ return SUM; }
prod			{ return PROD; }
lim			{ return LIM; }
a?(sin|cos|tan|sinh|cosh|tanh)		{
				yylval.symval = new Symbol(yytext);
				return TRIGFN;
			}
[a-zA-Z][a-zA-Z0-9]*	{
				yylval.symval = new Symbol(yytext);
				return IDENTIFIER;
			}
"<="			{ return LESSEQ; }
">="			{ return GTEQ; }
"->"			{ return ARROW; }
".."			{ return RANGE; }
[#].*			{ }
[()+\-*/;^!=<>,_'~\\\n\[\]]	{ return *yytext; }
[ \t\r]+		{ /* eat up whitespace */ }
.			{
				printf("bad character '%c'\n", *yytext);
				return *yytext;
			}

%%

int
yywrap()
{
	return 1;
}
