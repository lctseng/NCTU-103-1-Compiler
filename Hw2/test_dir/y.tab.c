/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */

#line 76 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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

/* Copy the second part of user declarations.  */

#line 203 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   331

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  128
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  238

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    26,     2,     2,
      48,    49,    24,    22,     2,    23,     2,    25,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    46,     2,    47,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,     2,    45,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    60,    60,    61,    62,    63,    66,    67,    68,    71,
      72,    73,    74,    75,    76,    77,    78,    81,    84,    85,
      86,    87,    88,    92,    93,    97,    98,   101,   102,   103,
     114,   120,   121,   123,   128,   131,   132,   135,   138,   139,
     140,   141,   146,   146,   150,   150,   154,   155,   156,   157,
     161,   161,   165,   165,   169,   170,   173,   173,   174,   174,
     177,   178,   182,   184,   185,   188,   189,   192,   193,   196,
     197,   200,   201,   206,   207,   208,   209,   210,   211,   212,
     216,   217,   221,   224,   229,   230,   235,   238,   239,   240,
     243,   244,   248,   249,   250,   254,   255,   256,   259,   260,
     263,   264,   266,   269,   272,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   297,   300,   301,   305,   306
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "BOOL", "FLOAT", "DOUBLE",
  "STRING", "CONST", "VOID", "END", "LESS_THAN_ELSE", "KW_ELSE", "OP_OR",
  "OP_AND", "OP_NOT", "OP_LT", "OP_LE", "OP_EQ", "OP_GE", "OP_GT", "OP_NE",
  "'+'", "'-'", "'*'", "'/'", "'%'", "KW_RETURN", "KW_BREAK",
  "KW_CONTINUE", "KW_FOR", "KW_WHILE", "KW_DO", "KW_IF", "SEMICOLON", "ID",
  "ASSIGN", "COMMA", "KW_PRINT", "KW_READ", "INTEGER_CONSTANT",
  "FLOATING_CONSTANT", "BOOLEAN_CONSTANT", "STRING_CONSTANT", "'{'", "'}'",
  "'['", "']'", "'('", "')'", "$accept", "program", "decl_and_def_list",
  "declaration_list", "var_decl", "type", "identifier", "var_list",
  "var_entry", "scalar_id", "array_indice", "single_array_indice",
  "const_decl", "const_list", "single_const", "literal_constant",
  "funct_decl", "$@1", "proc_decl", "$@2", "definition_list", "funct_def",
  "$@3", "proc_def", "$@4", "args", "single_arg", "$@5", "$@6", "arg_list",
  "compound_stat", "local_decl_list", "var_decl_list", "var_decl_entry",
  "zero_or_more_stat_list", "stat_list", "stat", "if_stat", "if_only_stat",
  "if_else_stat", "while_stat", "for_stat", "for_expr",
  "one_or_more_for_assignment", "jump_stat", "simple_stat", "var_ref",
  "intexpr_array_indice", "intexpr_single_array_indice", "int_expr",
  "bool_expr", "expr", "funct_call", "zero_or_more_expr_list", "expr_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,    43,    45,    42,    47,    37,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   123,   125,    91,    93,    40,    41
};
# endif

#define YYPACT_NINF -199

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-199)))

#define YYTABLE_NINF -53

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-53)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     175,  -199,  -199,  -199,  -199,  -199,   157,   -22,    18,   175,
    -199,   -22,  -199,  -199,  -199,  -199,  -199,   -22,  -199,   -29,
    -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,   -12,     6,
     -19,   175,   175,    -5,     5,     7,   115,   175,   175,  -199,
     -22,   102,   -10,   115,    11,    27,  -199,  -199,  -199,  -199,
      47,  -199,   -22,   157,  -199,     3,    14,  -199,  -199,    46,
     102,   102,  -199,  -199,  -199,  -199,   102,    59,  -199,  -199,
     270,  -199,    24,    36,    54,  -199,   -22,   -22,  -199,  -199,
    -199,  -199,  -199,   -22,    62,   115,   294,  -199,   176,   102,
     102,  -199,    64,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,  -199,    67,   102,    45,
      60,   -22,  -199,    27,    80,    83,  -199,  -199,    81,   270,
     270,    84,    92,  -199,   283,   294,   305,   305,   305,   305,
     305,   305,    77,    77,  -199,  -199,  -199,    96,    83,   -21,
     115,   115,  -199,  -199,  -199,   216,  -199,  -199,  -199,   102,
    -199,  -199,  -199,    85,    89,  -199,   -22,  -199,   241,  -199,
     216,   270,  -199,  -199,   102,    98,   106,    99,   104,    83,
     107,   102,   -22,  -199,   101,   241,  -199,  -199,  -199,  -199,
    -199,  -199,  -199,  -199,   122,   132,  -199,   192,  -199,  -199,
     102,   102,   136,   102,   214,    64,   134,  -199,  -199,   102,
    -199,  -199,   135,   133,   137,   270,   125,   270,   138,   139,
    -199,  -199,   233,   102,   -22,   102,    83,   102,    83,  -199,
     151,   167,   270,  -199,   155,   217,   102,   102,   207,    83,
     193,   270,  -199,  -199,   199,   241,   200,  -199
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    18,    19,    21,    20,    22,     0,     0,     0,     0,
      14,     0,    13,    15,    16,     8,     8,     0,    30,     0,
       1,    10,     9,    11,    12,     8,     8,    27,     0,    25,
      23,     5,     4,     0,     0,    35,    55,     2,     3,    17,
       0,     0,     0,    55,    24,    32,     6,     7,    48,    49,
       0,    34,     0,     0,    56,     0,    61,    54,    26,    23,
       0,     0,    38,    39,    40,    41,     0,    98,   121,   123,
      28,   122,     0,     0,     0,    31,     0,     0,    46,    47,
      37,    36,    58,     0,    44,     0,   120,   110,     0,     0,
     126,    99,   101,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    33,    42,     0,    23,
       0,     0,    57,    23,     0,     0,    60,   111,     0,   103,
     127,     0,   125,   100,   119,   118,   112,   113,   117,   115,
     114,   116,   105,   106,   107,   108,   109,     0,     0,     0,
      55,    55,    59,    24,    45,    64,    53,   102,   124,     0,
      43,    51,    29,     0,     0,    67,     0,    68,    70,    63,
      66,   128,    42,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    69,    72,    75,    80,    81,
      76,    77,    78,    74,     0,     0,    65,     0,    93,    94,
      89,     0,     0,     0,     0,    98,     0,    62,    71,     0,
      79,    92,     0,    88,   123,    87,     0,   104,     0,     0,
      96,    97,     0,    89,     0,     0,     0,     0,     0,    95,
       0,     0,    90,    84,     0,    82,    89,     0,     0,     0,
       0,    91,    85,    83,     0,    70,     0,    86
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -199,  -199,    70,   244,    -6,    31,   -75,   220,  -199,    -7,
     -38,  -199,    -4,   209,  -199,   212,     0,  -199,     2,  -199,
    -199,    25,  -199,    32,  -199,   -41,  -199,  -199,  -199,   178,
    -103,  -199,   105,  -199,    29,  -199,    91,  -199,  -199,  -199,
    -199,  -199,  -198,  -199,  -199,  -199,   -78,   183,  -199,  -199,
    -179,   -40,  -152,  -199,   169
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,    31,    46,    10,    11,    27,    28,    29,    67,
      44,    45,    12,    34,    35,    68,    13,   137,    14,   114,
      47,    48,   138,    49,   115,    55,    56,    83,   111,    57,
     173,   158,   159,   160,   174,   175,   176,   177,   178,   179,
     180,   181,   202,   203,   182,   183,    69,    91,    92,   118,
     206,   205,    71,   121,   122
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      19,    70,    73,    21,    30,    22,   185,    75,   112,    23,
      33,    24,   146,    18,   209,   220,   149,    41,    20,    36,
      86,    87,    39,   185,   152,    15,    88,    42,   230,    43,
      72,    50,    16,    59,    25,   151,   142,    17,   224,    51,
      21,    26,    22,    40,    52,    33,    23,    74,    24,   119,
     120,    85,    84,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   192,    54,   120,   109,
     110,   106,    78,    42,    54,   143,   113,    76,    77,    79,
     184,    41,    41,   185,    82,   107,    32,    62,    63,    64,
      65,    42,    42,   140,   196,    37,    38,   184,   108,   153,
     154,   103,   104,   105,   113,    89,   -52,    90,   141,   161,
      89,   -50,   204,   223,   144,   225,    54,    60,     1,     2,
       3,     4,     5,    53,   187,    61,   233,   145,   147,   149,
     150,   194,   188,   148,   162,   204,   221,    18,   163,   155,
     189,   157,    62,    63,    64,    65,   197,   190,   204,    59,
      66,   207,   191,   207,   155,   193,   157,   184,   199,   212,
       1,     2,     3,     4,     5,   195,   200,   208,   211,   213,
     214,    54,    54,   215,   216,   222,   156,   207,     1,     2,
       3,     4,     5,     6,     7,   226,   217,   231,   218,    93,
      94,   156,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   227,   228,    93,    94,   195,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,     1,
       2,     3,     4,     5,     6,   117,   201,    93,    94,   229,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   232,   234,   235,     9,   237,    93,    94,   210,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
      58,    81,    80,   116,   236,   186,   198,   219,   164,   165,
     166,   167,   168,   169,   170,   123,    18,   139,     0,   171,
     172,     0,     0,    93,    94,   145,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    94,     0,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   -53,   -53,   -53,   -53,   -53,   -53,   101,   102,   103,
     104,   105
};

static const yytype_int16 yycheck[] =
{
       7,    41,    43,     9,    11,     9,   158,    45,    83,     9,
      17,     9,   115,    35,   193,   213,    37,    36,     0,    48,
      60,    61,    34,   175,    45,     0,    66,    46,   226,    48,
      40,    36,     0,    40,     9,   138,   111,     6,   217,    34,
      46,     9,    46,    37,    37,    52,    46,    36,    46,    89,
      90,    37,    49,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   169,    36,   108,    76,
      77,    47,    47,    46,    43,   113,    83,    46,    47,    47,
     158,    36,    36,   235,    53,    49,    16,    40,    41,    42,
      43,    46,    46,    48,   172,    25,    26,   175,    44,   140,
     141,    24,    25,    26,   111,    46,    44,    48,    48,   149,
      46,    44,   190,   216,    34,   218,    85,    15,     3,     4,
       5,     6,     7,     8,   164,    23,   229,    44,    47,    37,
      34,   171,    34,    49,    49,   213,   214,    35,    49,   145,
      34,   145,    40,    41,    42,    43,    45,    48,   226,   156,
      48,   191,    48,   193,   160,    48,   160,   235,    36,   199,
       3,     4,     5,     6,     7,   172,    34,    31,    34,    34,
      37,   140,   141,    36,    49,   215,   145,   217,     3,     4,
       5,     6,     7,     8,     9,    34,    48,   227,    49,    13,
      14,   160,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    36,    49,    13,    14,   214,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     3,
       4,     5,     6,     7,     8,    49,    34,    13,    14,    12,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    34,    49,    44,     0,    45,    13,    14,    34,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      40,    52,    50,    85,   235,   160,   175,    34,    27,    28,
      29,    30,    31,    32,    33,    92,    35,   108,    -1,    38,
      39,    -1,    -1,    13,    14,    44,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    51,    53,
      54,    55,    62,    66,    68,    71,    73,    55,    35,    59,
       0,    54,    62,    66,    68,    71,    73,    56,    57,    58,
      59,    52,    52,    59,    63,    64,    48,    52,    52,    34,
      37,    36,    46,    48,    60,    61,    53,    70,    71,    73,
      36,    34,    37,     8,    55,    75,    76,    79,    57,    59,
      15,    23,    40,    41,    42,    43,    48,    59,    65,    96,
     101,   102,    40,    75,    36,    60,    55,    55,    71,    73,
      65,    63,    55,    77,    49,    37,   101,   101,   101,    46,
      48,    97,    98,    13,    14,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    47,    49,    44,    59,
      59,    78,    56,    59,    69,    74,    79,    49,    99,   101,
     101,   103,   104,    97,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,    67,    72,   104,
      48,    48,    56,    60,    34,    44,    80,    47,    49,    37,
      34,    80,    45,    75,    75,    54,    55,    62,    81,    82,
      83,   101,    49,    49,    27,    28,    29,    30,    31,    32,
      33,    38,    39,    80,    84,    85,    86,    87,    88,    89,
      90,    91,    94,    95,    96,   102,    82,   101,    34,    34,
      48,    48,    80,    48,   101,    59,    96,    45,    86,    36,
      34,    34,    92,    93,    96,   101,   100,   101,    31,   100,
      34,    34,   101,    34,    37,    36,    49,    48,    49,    34,
      92,    96,   101,    80,   100,    80,    34,    36,    49,    12,
      92,   101,    34,    80,    49,    44,    84,    45
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    51,    51,    51,    52,    52,    52,    53,
      53,    53,    53,    53,    53,    53,    53,    54,    55,    55,
      55,    55,    55,    56,    56,    57,    57,    58,    58,    58,
      59,    60,    60,    61,    62,    63,    63,    64,    65,    65,
      65,    65,    67,    66,    69,    68,    70,    70,    70,    70,
      72,    71,    74,    73,    75,    75,    77,    76,    78,    76,
      79,    79,    80,    81,    81,    82,    82,    83,    83,    84,
      84,    85,    85,    86,    86,    86,    86,    86,    86,    86,
      87,    87,    88,    89,    90,    90,    91,    92,    92,    92,
      93,    93,    94,    94,    94,    95,    95,    95,    96,    96,
      97,    97,    98,    99,   100,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   102,   103,   103,   104,   104
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     2,     2,     2,     2,     0,     2,
       2,     2,     2,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     2,     1,     3,     1,     3,     6,
       1,     2,     1,     3,     4,     1,     3,     3,     1,     1,
       1,     1,     0,     7,     0,     7,     2,     2,     1,     1,
       0,     7,     0,     7,     1,     0,     0,     3,     0,     4,
       3,     1,     4,     1,     0,     2,     1,     1,     1,     1,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     5,     7,     5,     7,    11,     1,     1,     0,
       3,     5,     3,     2,     2,     4,     3,     3,     1,     2,
       2,     1,     3,     1,     1,     3,     3,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     1,     4,     1,     0,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 60 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to program\n");}
#line 1474 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 61 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to program\n");}
#line 1480 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 66 "parser.y" /* yacc.c:1646  */
    {printf("Reduce list&declare to list\n");}
#line 1486 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 67 "parser.y" /* yacc.c:1646  */
    {printf("Reduce list&define to list\n");}
#line 1492 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 71 "parser.y" /* yacc.c:1646  */
    {printf("Reduce decl_list&const to decl_list\n");}
#line 1498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 72 "parser.y" /* yacc.c:1646  */
    {printf("Reduce decl_list&variable to decl_list\n");}
#line 1504 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 73 "parser.y" /* yacc.c:1646  */
    {printf("Reduce decl_list&function to decl_list\n");}
#line 1510 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 74 "parser.y" /* yacc.c:1646  */
    {printf("Reduce decl_list&function to decl_list\n");}
#line 1516 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 75 "parser.y" /* yacc.c:1646  */
    {printf("Reduce const to decl_list\n");}
#line 1522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 76 "parser.y" /* yacc.c:1646  */
    {printf("Reduce variable to decl_list\n");}
#line 1528 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 77 "parser.y" /* yacc.c:1646  */
    {printf("Reduce function to decl_list\n");}
#line 1534 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 78 "parser.y" /* yacc.c:1646  */
    {printf("Reduce prodecure to decl_list\n");}
#line 1540 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 81 "parser.y" /* yacc.c:1646  */
    {printf("Reduce type id SEMI to variable_declare\n");}
#line 1546 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 84 "parser.y" /* yacc.c:1646  */
    {printf("Reduce INT to type\n");}
#line 1552 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 85 "parser.y" /* yacc.c:1646  */
    {printf("Reduce BOOL to type\n");}
#line 1558 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 86 "parser.y" /* yacc.c:1646  */
    {printf("Reduce DOUBLE to type\n");}
#line 1564 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 87 "parser.y" /* yacc.c:1646  */
    {printf("Reduce FLOAT to type\n");}
#line 1570 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 88 "parser.y" /* yacc.c:1646  */
    {printf("Reduce STRING to type\n");}
#line 1576 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 92 "parser.y" /* yacc.c:1646  */
    {printf("Reduce scalar_id to identifier \n");}
#line 1582 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 93 "parser.y" /* yacc.c:1646  */
    {printf("Reduce Array to identifier \n");}
#line 1588 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 120 "parser.y" /* yacc.c:1646  */
    {printf("Reduce singe_array to array\n");}
#line 1594 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 121 "parser.y" /* yacc.c:1646  */
    {printf("Reduce singe to array\n");}
#line 1600 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 123 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to single indice\n");}
#line 1606 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 128 "parser.y" /* yacc.c:1646  */
    {printf("Reduce CONST type identifier SEMI to const_decl\n");}
#line 1612 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 146 "parser.y" /* yacc.c:1646  */
    {printf("funct_decl seen ()\n");}
#line 1618 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 146 "parser.y" /* yacc.c:1646  */
    {printf("Reduce  to funct_decl \n");}
#line 1624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 150 "parser.y" /* yacc.c:1646  */
    {printf("proc_decl seen ()\n");}
#line 1630 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 150 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to proc_decl \n");}
#line 1636 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 154 "parser.y" /* yacc.c:1646  */
    {printf("Reduce def_list&funct_def to def_list \n");}
#line 1642 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 155 "parser.y" /* yacc.c:1646  */
    {printf("Reduce def_list&proc_def to def_list \n");}
#line 1648 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 156 "parser.y" /* yacc.c:1646  */
    {printf("Reduce NORMAL funct_def to def_list \n");}
#line 1654 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 157 "parser.y" /* yacc.c:1646  */
    {printf("Reduce proc_def to def_list \n");}
#line 1660 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 161 "parser.y" /* yacc.c:1646  */
    {printf("funct_def seen ()\n");}
#line 1666 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 161 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to funct_def \n");}
#line 1672 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 165 "parser.y" /* yacc.c:1646  */
    {printf("proc_def seen ()\n");}
#line 1678 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 165 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to proc_def \n");}
#line 1684 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 173 "parser.y" /* yacc.c:1646  */
    {printf("seen arg type\n");}
#line 1690 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 173 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to single_arg\n");}
#line 1696 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 174 "parser.y" /* yacc.c:1646  */
    {printf("seen const arg type\n");}
#line 1702 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 174 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to const single_arg\n");}
#line 1708 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 177 "parser.y" /* yacc.c:1646  */
    {printf("Reduce single_arg,arg_list to arg_list\n");}
#line 1714 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 178 "parser.y" /* yacc.c:1646  */
    {printf("Reduce single_arg to arg_list\n");}
#line 1720 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 182 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to compund stat\n");}
#line 1726 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 216 "parser.y" /* yacc.c:1646  */
    {printf("Reduce if-only to if\n");}
#line 1732 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 217 "parser.y" /* yacc.c:1646  */
    {printf("Reduce if-else to if\n");}
#line 1738 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 229 "parser.y" /* yacc.c:1646  */
    {printf("Reduce n-while to while\n");}
#line 1744 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 230 "parser.y" /* yacc.c:1646  */
    {printf("Reduce do-while to while\n");}
#line 1750 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 235 "parser.y" /* yacc.c:1646  */
    {printf("Redce to for\n");}
#line 1756 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 254 "parser.y" /* yacc.c:1646  */
    {printf("Reduce  to simple_stat\n");}
#line 1762 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 263 "parser.y" /* yacc.c:1646  */
    {printf("int:Reduce singe_array to array\n");}
#line 1768 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 264 "parser.y" /* yacc.c:1646  */
    {printf("int:Reduce singe to array\n");}
#line 1774 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 266 "parser.y" /* yacc.c:1646  */
    {printf("int:Reduce to single indice\n");}
#line 1780 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 281 "parser.y" /* yacc.c:1646  */
    {printf("Reduce with negation operator\n");}
#line 1786 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 297 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to function call\n");}
#line 1792 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 300 "parser.y" /* yacc.c:1646  */
    {printf("Reduce to expr list\n");}
#line 1798 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1802 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 310 "parser.y" /* yacc.c:1906  */


int yyerror( char *msg )
{
  fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
  fprintf( stderr, "|--------------------------------------------------------------------------\n" );
  exit(-1);
}

int  main( int argc, char **argv )
{
	if( argc != 2 ) {
		fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
		exit(0);
	}

	FILE *fp = fopen( argv[1], "r" );
	
	if( fp == NULL )  {
		fprintf( stdout, "Open  file  error\n" );
		exit(-1);
	}
	
	yyin = fp;
	yyparse();

	fprintf( stdout, "\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	fprintf( stdout, "|  There is no syntactic error!  |\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	exit(0);
}

