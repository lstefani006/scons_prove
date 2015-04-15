/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_RR_TAB_HPP_INCLUDED
# define YY_YY_RR_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    VAR = 258,
    FUN = 259,
    ENUM = 260,
    CLASS = 261,
    IF = 262,
    ELSE = 263,
    WHILE = 264,
    FOR = 265,
    IN = 266,
    CAST = 267,
    NEW = 268,
    NUM = 269,
    ID = 270,
    STR = 271,
    INC = 272,
    DEC = 273,
    LS = 274,
    RS = 275,
    EQ = 276,
    NE = 277,
    GE = 278,
    LE = 279,
    AA = 280,
    OO = 281,
    PRE = 282,
    UMINUS = 283,
    POST = 284
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 10 "rr.yy" /* yacc.c:1909  */

	int   i;
	float f;
	char *s;

	Pgm      *p;
	Decl     *d;
	DeclVar  *dv;
	DeclEnum *de;
	DeclList *dd;

	Type *t;
	Expr *e;
	ExprList *ee;

	Stmt     *m;
	StmtList *mm;

	VarList  *vv;

#line 105 "rr_tab.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (Pgm *&ret);

#endif /* !YY_YY_RR_TAB_HPP_INCLUDED  */
