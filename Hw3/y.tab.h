#ifndef YYERRCODE
#define YYERRCODE 256
#endif

#define INT 257
#define BOOL 258
#define FLOAT 259
#define DOUBLE 260
#define STRING 261
#define CONST 262
#define VOID 263
#define END 264
#define LESS_THAN_ELSE 265
#define KW_ELSE 266
#define ARITHMETIC_PARENTHESES 267
#define LOWER_THEN_ARITHMETIC_PARENTHESES 268
#define OP_OR 269
#define OP_AND 270
#define OP_NOT 271
#define OP_LT 272
#define OP_LE 273
#define OP_EQ 274
#define OP_GE 275
#define OP_GT 276
#define OP_NE 277
#define KW_RETURN 278
#define KW_BREAK 279
#define KW_CONTINUE 280
#define KW_FOR 281
#define KW_WHILE 282
#define KW_DO 283
#define KW_IF 284
#define SEMICOLON 285
#define ID 286
#define ASSIGN 287
#define COMMA 288
#define KW_PRINT 289
#define KW_READ 290
#define INTEGER_CONSTANT 291
#define FLOATING_CONSTANT 292
#define BOOLEAN_CONSTANT 293
#define STRING_CONSTANT 294
typedef union {
    int value;
    char* text;
    int type;
    typeStruct_t typeStruct;
    typeList_t typeList;
} YYSTYPE;
extern YYSTYPE yylval;
