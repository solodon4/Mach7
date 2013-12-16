/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     xOp = 258,
     xInstBin = 259,
     xInstUn = 260,
     xInstMove = 261,
     xExt = 262,
     xTmp = 263,
     xLit = 264,
     xTyChar = 265,
     xTyUChar = 266,
     xTyShort = 267,
     xTyUShort = 268,
     xTyInt = 269,
     xTyUInt = 270,
     xTyLong = 271,
     xTyULong = 272,
     xTyLLong = 273,
     xTyULLong = 274,
     xTyFloat = 275,
     xTyDouble = 276,
     xTyCharN = 277,
     xTyUCharN = 278,
     xTyShortN = 279,
     xTyUShortN = 280,
     xTyIntN = 281,
     xTyUIntN = 282,
     xTyLongN = 283,
     xTyULongN = 284,
     xTyLLongN = 285,
     xTyULLongN = 286,
     xTyFloatN = 287,
     xTyDoubleN = 288,
     xValDouble = 289,
     xValFloat = 290,
     xValChar = 291,
     xValUChar = 292,
     xValShort = 293,
     xValUShort = 294,
     xValInt = 295,
     xValUInt = 296,
     xValLong = 297,
     xValULong = 298,
     xValLLong = 299,
     xValULLong = 300,
     xValDoubleN = 301,
     xValFloatN = 302,
     xValCharN = 303,
     xValUCharN = 304,
     xValShortN = 305,
     xValUShortN = 306,
     xValIntN = 307,
     xValUIntN = 308,
     xValLongN = 309,
     xValULongN = 310,
     xValLLongN = 311,
     xValULLongN = 312,
     xString = 313,
     xFloat = 314,
     xChar = 315,
     xInt = 316
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 28 "versity.y"

    std::string*        m_str;
    double              m_flt;
    int                 m_int;
    TyPrim*             m_ty_prim;
    Val*                m_val;
    Reg*                m_reg;
    Inst*               m_inst;
    std::vector<Inst*>* m_instructions;



/* Line 1685 of yacc.c  */
#line 125 "versity.tab.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


