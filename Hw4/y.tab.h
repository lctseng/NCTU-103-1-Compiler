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
    ARITHMETIC_PARENTHESES = 268,
    LOWER_THEN_ARITHMETIC_PARENTHESES = 269,
    L_OP_OR = 270,
    L_OP_AND = 271,
    L_OP_NOT = 272,
    L_OP_LT = 273,
    L_OP_LE = 274,
    L_OP_EQ = 275,
    L_OP_GE = 276,
    L_OP_GT = 277,
    L_OP_NE = 278,
    KW_RETURN = 279,
    KW_BREAK = 280,
    KW_CONTINUE = 281,
    KW_FOR = 282,
    KW_WHILE = 283,
    KW_DO = 284,
    KW_IF = 285,
    SEMICOLON = 286,
    ID = 287,
    ASSIGN = 288,
    COMMA = 289,
    KW_PRINT = 290,
    KW_READ = 291,
    INTEGER_CONSTANT = 292,
    FLOATING_CONSTANT = 293,
    BOOLEAN_CONSTANT = 294,
    STRING_CONSTANT = 295
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
#define ARITHMETIC_PARENTHESES 268
#define LOWER_THEN_ARITHMETIC_PARENTHESES 269
#define L_OP_OR 270
#define L_OP_AND 271
#define L_OP_NOT 272
#define L_OP_LT 273
#define L_OP_LE 274
#define L_OP_EQ 275
#define L_OP_GE 276
#define L_OP_GT 277
#define L_OP_NE 278
#define KW_RETURN 279
#define KW_BREAK 280
#define KW_CONTINUE 281
#define KW_FOR 282
#define KW_WHILE 283
#define KW_DO 284
#define KW_IF 285
#define SEMICOLON 286
#define ID 287
#define ASSIGN 288
#define COMMA 289
#define KW_PRINT 290
#define KW_READ 291
#define INTEGER_CONSTANT 292
#define FLOATING_CONSTANT 293
#define BOOLEAN_CONSTANT 294
#define STRING_CONSTANT 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 1035 "parser.y" /* yacc.c:1909  */

    int value;
    char* text;
    int type;
    typeStruct_t typeStruct;
    typeList_t typeList;

#line 142 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
