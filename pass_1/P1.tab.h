/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_P1_TAB_H_INCLUDED
# define YY_YY_P1_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    INTEGER = 259,
    RELOP = 260,
    LOGOP = 261,
    ARITHOP = 262,
    MACRO = 263,
    LBRACE = 264,
    LBRACE2 = 265,
    LBRACE3 = 266,
    RBRACE3 = 267,
    RBRACE2 = 268,
    RBRACE = 269,
    CLASS = 270,
    PUBLIC = 271,
    STATIC = 272,
    tVOID = 273,
    MAIN = 274,
    STRING = 275,
    PRINT = 276,
    EXTENDS = 277,
    RETURN = 278,
    tBOOLEAN = 279,
    tINT = 280,
    IF = 281,
    ELSE = 282,
    WHILE = 283,
    LENGTH = 284,
    tTRUE = 285,
    tFALSE = 286,
    THIS = 287,
    NEW = 288,
    SEMICOLON = 289,
    DOT = 290,
    COMMA = 291,
    EQUALS = 292,
    NOT = 293,
    NO_ELSE = 294,
    NO_EXPR = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 215 "P1.y" /* yacc.c:1909  */

	int	int_val;
	char* name;
	char op;
	struct list *tokenList;

#line 102 "P1.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_P1_TAB_H_INCLUDED  */
