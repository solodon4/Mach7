%{
// This fixes bug with compiling flex generated file under MS VC++
#ifdef _MSC_VER
#define YY_NO_UNISTD_H
// Fix for MSVC 2003 as fileno was deprecated
#if _MSC_VER > 1300
#define fileno _fileno
#endif

#include <io.h>
#define isatty _isatty
#endif

#include "coudefs.hpp"

int g_line = 1;
%}

id    [a-zA-Z_][a-zA-Z_0-9]*
str   \"[^\n"]+\"
chr   \'([^\\]|(\\[nrftvba\\]))\'
dec   [0-9]+
bol   (true)|(false)
ws    [ \t]
fl1   [-+]?[0-9]+\.?([eE][-+]?[0-9]+)?
fl2   [-+]?[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?
num   {fl1}|{fl2}

%option noyywrap nounput batch debug

%% //======================================================================// %%

{id}                yylval.m_str = new std::string(yytext); return ID;
{id}-class          yylval.m_str = new std::string(yytext); return ID;

(:{id}?)+                                  /* ignore parameters */;
;.*$                                       /* ignore comment */;
\n                  g_line++;              /* ignore end of line */;
[ \t]+                                     /* ignore whitespace  */;
.                   return (int)yytext[0]; /* pass other chars as tokens */

%% //======================================================================// %%
