%{
#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */
%}



%nonassoc INT BOOL FLOAT DOUBLE STRING CONST VOID
%nonassoc END

%nonassoc LESS_THAN_ELSE
%nonassoc KW_ELSE

%left OP_OR
%left OP_AND
%left OP_NOT
%nonassoc OP_LT OP_LE OP_EQ OP_GE OP_GT OP_NE



%left '+' '-'
%left '*' '/' '%'

%token KW_RETURN
%token KW_BREAK
%token KW_CONTINUE

%token KW_FOR
%token KW_WHILE
%token KW_DO
%token KW_IF
%token SEMICOLON    /* ; */
%token ID           /* identifier */
%token INT          /* keyword */
%token BOOL
%token FLOAT
%token DOUBLE
%token STRING
%token ASSIGN
%token COMMA
%token KW_PRINT
%token KW_READ


%token INTEGER_CONSTANT
%token FLOATING_CONSTANT
%token BOOLEAN_CONSTANT
%token STRING_CONSTANT
%token VOID
%token CONST


%%

program : declaration_list funct_def decl_and_def_list  {printf("Reduce to program\n");} 
        | declaration_list proc_def decl_and_def_list  {printf("Reduce to program\n");} 
	| proc_def decl_and_def_list 
	| funct_def decl_and_def_list 
	;

decl_and_def_list	: decl_and_def_list declaration_list %prec END  {printf("Reduce list&declare to list\n");}
			| decl_and_def_list definition_list %prec END {printf("Reduce list&define to list\n");}
                        |
                        ;

declaration_list : declaration_list const_decl {printf("Reduce decl_list&const to decl_list\n");}
                 | declaration_list var_decl {printf("Reduce decl_list&variable to decl_list\n");}
                 | declaration_list funct_decl {printf("Reduce decl_list&function to decl_list\n");}
                 | declaration_list proc_decl {printf("Reduce decl_list&function to decl_list\n");}
                 | const_decl {printf("Reduce const to decl_list\n");}
                 | var_decl {printf("Reduce variable to decl_list\n");}
                 | funct_decl {printf("Reduce function to decl_list\n");}
                 | proc_decl {printf("Reduce prodecure to decl_list\n");}
		 ;

var_decl : type var_list SEMICOLON {printf("Reduce type id SEMI to variable_declare\n");}
         ;

type : INT {printf("Reduce INT to type\n");}
     | BOOL {printf("Reduce BOOL to type\n");} 
     | DOUBLE {printf("Reduce DOUBLE to type\n");} 
     | FLOAT {printf("Reduce FLOAT to type\n");} 
     | STRING {printf("Reduce STRING to type\n");} 
     ; 

 /* scalar id or arrray id */
identifier : scalar_id {printf("Reduce scalar_id to identifier \n");}
           | scalar_id array_indice {printf("Reduce Array to identifier \n");}
	   ;
 /* A list of variable entry, one or more */

var_list : var_entry
         | var_entry COMMA var_list
         ;
 /* Variable Entry, either "identifier" or "identifier = decl_expr" */
var_entry : identifier 
          | scalar_id ASSIGN expr
          | scalar_id array_indice ASSIGN '{' expr_list '}'
          ;

 /* List of variable declaration expr list, embeded by { } */
 /*
decl_expr : expr 
          | '{' expr_list '}'
          ;
*/

 /* Pure ID */
scalar_id : ID
          ;




array_indice : single_array_indice array_indice {printf("Reduce singe_array to array\n");}
             | single_array_indice {printf("Reduce singe to array\n");}
             ;
single_array_indice : '[' INTEGER_CONSTANT ']' {printf("Reduce to single indice\n");}
                    ;


 /* constant variable */
const_decl : CONST type const_list SEMICOLON {printf("Reduce CONST type identifier SEMI to const_decl\n");}
           ;

const_list : single_const
           | single_const COMMA const_list
           ;

single_const : scalar_id ASSIGN literal_constant 
             ;

literal_constant : INTEGER_CONSTANT
                 | FLOATING_CONSTANT
                 | BOOLEAN_CONSTANT
                 | STRING_CONSTANT
                 ;


 /* function declaration */
funct_decl : type scalar_id '(' args ')' {printf("funct_decl seen ()\n");} SEMICOLON {printf("Reduce  to funct_decl \n");}
           ;

 /* procedure declaration */
proc_decl : VOID scalar_id '(' args ')' {printf("proc_decl seen ()\n");} SEMICOLON {printf("Reduce to proc_decl \n");}
          ;

 /* definition list */
definition_list : definition_list funct_def  {printf("Reduce def_list&funct_def to def_list \n");}
                | definition_list proc_def   {printf("Reduce def_list&proc_def to def_list \n");}
                | funct_def {printf("Reduce NORMAL funct_def to def_list \n");}
                | proc_def {printf("Reduce proc_def to def_list \n");}
                ;

 /* function definition */
funct_def : type scalar_id '(' args ')' {printf("funct_def seen ()\n");} compound_stat {printf("Reduce to funct_def \n");}
          ;
 
 /* procedure definition */
proc_def : VOID scalar_id '(' args ')' {printf("proc_def seen ()\n");} compound_stat {printf("Reduce to proc_def \n");}
         ;

 /* Arguments, zero or more */
args :  arg_list
     |  /* empty list */ 
     ;
 /* Single argument */
single_arg :  type {printf("seen arg type\n");} identifier {printf("Reduce to single_arg\n");}
           |  CONST type {printf("seen const arg type\n");} identifier {printf("Reduce to const single_arg\n");}
           ;
 /* A list of args */
arg_list : single_arg COMMA arg_list {printf("Reduce single_arg,arg_list to arg_list\n");}
         | single_arg     {printf("Reduce single_arg to arg_list\n");}
         ;

 /* compound statment */
compound_stat : '{' local_decl_list zero_or_more_stat_list '}' {printf("Reduce to compund stat\n");}
              ;
local_decl_list : var_decl_list
                |
                ;

var_decl_list : var_decl_entry  var_decl_list
              | var_decl_entry
              ;

var_decl_entry : var_decl
               | const_decl
               ;

zero_or_more_stat_list : stat_list
                       | 
                       ;

stat_list : stat_list stat
          | stat
          ;

 /* statement */

stat : compound_stat
     | simple_stat
     | if_stat
     | while_stat
     | for_stat
     | jump_stat
     | funct_call SEMICOLON
     ;

 /* if-statment */
if_stat : if_only_stat {printf("Reduce if-only to if\n");}
        | if_else_stat {printf("Reduce if-else to if\n");}
        ;


if_only_stat : KW_IF '(' bool_expr ')' compound_stat %prec LESS_THAN_ELSE
             ;

if_else_stat : KW_IF '(' bool_expr ')' compound_stat KW_ELSE compound_stat 
             ;

 /* while-statment */

while_stat : KW_WHILE '(' bool_expr ')' compound_stat {printf("Reduce n-while to while\n");} 
           | KW_DO compound_stat KW_WHILE '(' bool_expr ')' SEMICOLON {printf("Reduce do-while to while\n");}
           ;


 /* For-statment */
for_stat : KW_FOR '(' for_expr SEMICOLON for_expr SEMICOLON for_expr ')' '{' zero_or_more_stat_list '}' {printf("Redce to for\n");}
         ;

for_expr : expr
         | one_or_more_for_assignment 
         | 
         ;

one_or_more_for_assignment : var_ref ASSIGN expr
                           | one_or_more_for_assignment COMMA var_ref ASSIGN expr
                           ;

 /* Jump statment */
jump_stat : KW_RETURN expr SEMICOLON
          | KW_BREAK SEMICOLON
          | KW_CONTINUE SEMICOLON
          ;

 /* Simple statment */
simple_stat : var_ref ASSIGN expr SEMICOLON {printf("Reduce  to simple_stat\n");}
            | KW_PRINT expr SEMICOLON
            | KW_READ var_ref SEMICOLON
            ;

var_ref : scalar_id
        | scalar_id intexpr_array_indice
        ;

intexpr_array_indice : intexpr_single_array_indice intexpr_array_indice {printf("int:Reduce singe_array to array\n");}
             | intexpr_single_array_indice {printf("int:Reduce singe to array\n");}
             ;
intexpr_single_array_indice : '[' int_expr ']' {printf("int:Reduce to single indice\n");}
                    ;

int_expr : expr
         ;

bool_expr : expr
          ;


expr : expr '+' expr
     | expr '-' expr
     | expr '*' expr
     | expr '/' expr
     | expr '%' expr
     | '-' expr %prec '*' {printf("Reduce with negation operator\n");}
     | '(' expr ')'
     | expr OP_LT expr
     | expr OP_LE expr
     | expr OP_GT expr
     | expr OP_GE expr
     | expr OP_NE expr
     | expr OP_EQ expr
     | expr OP_AND expr
     | expr OP_OR expr
     | OP_NOT expr
     | literal_constant
     | funct_call
     | var_ref
     ;

funct_call : scalar_id '(' zero_or_more_expr_list ')' {printf("Reduce to function call\n");}
           ;

zero_or_more_expr_list :  expr_list {printf("Reduce to expr list\n");}
                       | /* empty */
                       ;

 /* one or more expr */
expr_list : expr
          | expr_list COMMA expr
          ;


%%

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

