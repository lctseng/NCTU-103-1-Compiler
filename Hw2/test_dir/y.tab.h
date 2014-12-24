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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    INT = 258,
    BOOL = 259,
    FLOAT = 260,
    DOUBLE = 261,
    STRING = 262,
    CONST = 263,
    VOID = 264,
    END = 265,
    LESS_THAN_ELSE = 266,
    KW_ELSE = 267,
    OP_OR = 268,
    OP_AND = 269,
    OP_NOT = 270,
    OP_LT = 271,
    OP_LE = 272,
    OP_EQ = 273,
    OP_GE = 274,
    OP_GT = 275,
    OP_NE = 276,
    KW_RETURN = 277,
    KW_BREAK = 278,
    KW_CONTINUE = 279,
    KW_FOR = 280,
    KW_WHILE = 281,
    KW_DO = 282,
    KW_IF = 283,
    SEMICOLON = 284,
    ID = 285,
    ASSIGN = 286,
    COMMA = 287,
    KW_PRINT = 288,
    KW_READ = 289,
    INTEGER_CONSTANT = 290,
    FLOATING_CONSTANT = 291,
    BOOLEAN_CONSTANT = 292,
    STRING_CONSTANT = 293
  };
#endif
/* Tokens.  */
#define INT 258
#define BOOL 259
#define FLOAT 260
#define DOUBLE 261
#define STRING 262
#define CONST 263
#define VOID 264
#define END 265
#define LESS_THAN_ELSE 266
#define KW_ELSE 267
#define OP_OR 268
#define OP_AND 269
#define OP_NOT 270
#define OP_LT 271
#define OP_LE 272
#define OP_EQ 273
#define OP_GE 274
#define OP_GT 275
#define OP_NE 276
#define KW_RETURN 277
#define KW_BREAK 278
#define KW_CONTINUE 279
#define KW_FOR 280
#define KW_WHILE 281
#define KW_DO 282
#define KW_IF 283
#define SEMICOLON 284
#define ID 285
#define ASSIGN 286
#define COMMA 287
#define KW_PRINT 288
#define KW_READ 289
#define INTEGER_CONSTANT 290
#define FLOATING_CONSTANT 291
#define BOOLEAN_CONSTANT 292
#define STRING_CONSTANT 293

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
