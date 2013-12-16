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

#include "versity-defs.hpp"

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

Op          return xOp        ;
InstBin     return xInstBin   ; 
InstUn      return xInstUn    ; 
InstMove    return xInstMove  ; 
Ext         return xExt       ; 
Tmp         return xTmp       ; 
Lit         return xLit       ; 
TyChar      return xTyChar    ; 
TyUChar     return xTyUChar   ; 
TyShort     return xTyShort   ; 
TyUShort    return xTyUShort  ; 
TyInt       return xTyInt     ; 
TyUInt      return xTyUInt    ; 
TyLong      return xTyLong    ; 
TyULong     return xTyULong   ; 
TyLLong     return xTyLLong   ; 
TyULLong    return xTyULLong  ; 
TyFloat     return xTyFloat   ; 
TyDouble    return xTyDouble  ; 
TyCharN     return xTyCharN   ; 
TyUCharN    return xTyUCharN  ; 
TyShortN    return xTyShortN  ; 
TyUShortN   return xTyUShortN ; 
TyIntN      return xTyIntN    ; 
TyUIntN     return xTyUIntN   ; 
TyLongN     return xTyLongN   ; 
TyULongN    return xTyULongN  ; 
TyLLongN    return xTyLLongN  ; 
TyULLongN   return xTyULLongN ; 
TyFloatN    return xTyFloatN  ; 
TyDoubleN   return xTyDoubleN ; 
ValDouble   return xValDouble ; 
ValFloat    return xValFloat  ; 
ValChar     return xValChar   ; 
ValUChar    return xValUChar  ; 
ValShort    return xValShort  ; 
ValUShort   return xValUShort ; 
ValInt      return xValInt    ; 
ValUInt     return xValUInt   ; 
ValLong     return xValLong   ; 
ValULong    return xValULong  ; 
ValLLong    return xValLLong  ; 
ValULLong   return xValULLong ; 
ValDoubleN  return xValDoubleN; 
ValFloatN   return xValFloatN ; 
ValCharN    return xValCharN  ; 
ValUCharN   return xValUCharN ; 
ValShortN   return xValShortN ; 
ValUShortN  return xValUShortN; 
ValIntN     return xValIntN   ; 
ValUIntN    return xValUIntN  ; 
ValLongN    return xValLongN  ; 
ValULongN   return xValULongN ; 
ValLLongN   return xValLLongN ; 
ValULLongN  return xValULLongN; 

{dec}               yylval.m_int = atoi(yytext);            return xInt;
{num}               yylval.m_flt = atof(yytext);            return xFloat;
{str}               yylval.m_str = new std::string(yytext); return xString;

\n                  g_line++;              /* ignore end of line */;
[ \t]+                                     /* ignore whitespace  */;
.                   return (int)yytext[0]; /* pass other chars as tokens */

%% //======================================================================// %%
