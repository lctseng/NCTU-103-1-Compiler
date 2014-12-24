%{
#define LIST                { strncat(buf, yytext, sizeof(buf) - strlen(buf) - 1); }
#define SADD(s)                { strncat(strbuf, s, sizeof(strbuf) - strlen(strbuf) - 1); }
#define token(t)            { LIST; if (Opt_Token) printf("<%s>\n", #t); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define tokenStringN(t, s)   {if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define MAX_LINE_LENGTH 257
#define MAX_ID_LENGTH 257
#define MAX_ID_COUNT 1024
#ifndef ERR_FD
#define ERR_FD stderr
#endif
#include "string.h"
#include "y.tab.h"
int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
int* ctrl_target = NULL;
char buf[MAX_LINE_LENGTH];
char strbuf[MAX_LINE_LENGTH];
void line_process(void);
void register_id(const char* );
void dispose_id_table();
%}

letter [a-zA-Z]
digit [0-9]
blank [ \t]
%Start COMMENT SPAN_COMMENT STRING_STATE PRAGMA_TYPE PRAGMA_OP PRAGMA_COMMENT PRAGMA_SPAN_COMMENT NEW_LINE_NOW
%%

 /**** One line comment ****/
<INITIAL>"//" {
   LIST;
   BEGIN COMMENT;
}
 /**** Multiple line comment ****/
<INITIAL>"/*" {
   LIST;
   BEGIN SPAN_COMMENT;
}
 /**** One line comment ****/
<PRAGMA_COMMENT>"//" {
   LIST;
   BEGIN COMMENT;
}
 /**** Multiple line comment ****/
<PRAGMA_COMMENT>"/*" {
   LIST;
   BEGIN PRAGMA_SPAN_COMMENT;
}

<PRAGMA_SPAN_COMMENT>"*/" {
   LIST;
   BEGIN NEW_LINE_NOW;
}
<SPAN_COMMENT>"*/" {
   LIST;
   BEGIN INITIAL;
}
<NEW_LINE_NOW>\n |
<PRAGMA_SPAN_COMMENT>\n |
<SPAN_COMMENT>\n {
   LIST;
   line_process();
}
 /**** Eat all character when COMMENT or SPAN_COMMENT ****/
<COMMENT>. {
    LIST;
}
<PRAGMA_SPAN_COMMENT>. |
<SPAN_COMMENT>. {
    LIST;
}

 /**** Pragma Directives ****/
 /* Enter pragma mode , try to find type */
<INITIAL>^#pragma{blank}+ {
    LIST;
    BEGIN PRAGMA_TYPE;
}
 /* Filter the type */
<PRAGMA_TYPE>"source"{blank}+ {
    LIST;
    ctrl_target = &Opt_Source;    
    BEGIN PRAGMA_OP;
}
<PRAGMA_TYPE>"token"{blank}+ {
    LIST;
    ctrl_target = &Opt_Token;    
    BEGIN PRAGMA_OP;
}
<PRAGMA_TYPE>"statistic"{blank}+ {
    LIST;
    ctrl_target = &Opt_Statistic;    
    BEGIN PRAGMA_OP;
}

 /* Filter option */
<PRAGMA_OP>"on" {
    LIST;
    *ctrl_target = 1;
    BEGIN PRAGMA_COMMENT;
}
<PRAGMA_OP>"off" {
    LIST;
    *ctrl_target = 0;
    BEGIN PRAGMA_COMMENT;
}





 /**** delimoters ****/
<INITIAL>"," {
    tokenString("delim", ","); 
    return COMMA;
}
<INITIAL>";" {
    tokenString("delim", ";"); 
    return SEMICOLON;
}
<INITIAL>"(" {
    tokenString("delim", "("); 
    return '(';
}
<INITIAL>")" {
    tokenString("delim", ")"); 
    return ')';
}
<INITIAL>"[" {
    tokenString("delim", "["); 
    return '[';
}
<INITIAL>"]" {
    tokenString("delim", "]"); 
    return ']';

}
<INITIAL>"{" {
    tokenString("delim", "{"); 
    return '{';
}
<INITIAL>"}" {
    tokenString("delim", "}"); 
    return '}';
}




 /**** new line ****/

<COMMENT>\n         |
<PRAGMA_TYPE>\n     |
<PRAGMA_OP>\n       |
<PRAGMA_COMMENT>\n  |
<INITIAL>\n {
    BEGIN INITIAL;
    LIST;
    line_process();
}

 /**** keyword ****/
<INITIAL>"while" {
    tokenString("KW", "while"); 
    return KW_WHILE;   
}
<INITIAL>"do" {
    tokenString("KW", "do"); 
    return KW_DO;
}
<INITIAL>"if" {
    tokenString("KW", "if"); 
    return KW_IF;
}
<INITIAL>"else" {
    tokenString("KW", "else"); 
    return KW_ELSE;
}
<INITIAL>"true" {
    tokenString("KW", "true"); 
    return BOOLEAN_CONSTANT;
}
<INITIAL>"false" {
    tokenString("KW", "false"); 
    return BOOLEAN_CONSTANT;
}
<INITIAL>"for" {
    tokenString("KW", "for"); 
    return KW_FOR;
}
<INITIAL>"int" {
    tokenString("KW", "int"); 
    return INT;
}
<INITIAL>"print" {
    tokenString("KW", "print"); 
    return KW_PRINT;
}
<INITIAL>"const" {
    tokenString("KW", "const"); 
    return CONST;
}
<INITIAL>"read" {
    tokenString("KW", "read"); 
    return KW_READ;
}
<INITIAL>"boolean" {
    tokenString("KW", "boolean"); 
    return BOOL;
}
<INITIAL>"bool" {
    tokenString("KW", "bool"); 
    return BOOL;
}
<INITIAL>"void" {
    tokenString("KW", "void"); 
    return VOID;
}
<INITIAL>"float" {
    tokenString("KW", "float"); 
    return FLOAT;
}
<INITIAL>"double" {
    tokenString("KW", "double"); 
    return DOUBLE;
}
<INITIAL>"string" {
    tokenString("KW", "string"); 
    return STRING;
}
<INITIAL>"continue" {
    tokenString("KW", "continue"); 
    return KW_CONTINUE;
}
<INITIAL>"break" {
    tokenString("KW", "break"); 
    return KW_BREAK;
}
<INITIAL>"return" {
    tokenString("KW", "return"); 
    return KW_RETURN;
}

 
 /**** Identifier ****/
<INITIAL>{letter}({digit}|{letter})* {
    tokenString("id", yytext);
    //register_id(yytext);
    return ID;
}


 /**** Operator ****/
<INITIAL>"+" {
    token("+");
    return '+';
}
<INITIAL>"-" {
    token("-");
    return '-';
}
<INITIAL>"*" {
    token("*");
    return '*';
}
<INITIAL>"/" {
    token("/");
    return '/';
}
<INITIAL>"%" {
    token("%");
    return '%';
}
<INITIAL>"=" {
    token("=");
    return ASSIGN;
}
<INITIAL>"<" {
    token("<");
    return OP_LT;
}
<INITIAL>"<=" {
    token("<=");
    return OP_LE;
}
<INITIAL>"!=" {
    token("!=");
    return OP_NE;
}
<INITIAL>">=" {
    token(">=");
    return OP_GE;
}
<INITIAL>">" {
    token(">");
    return OP_GT;
}
<INITIAL>"==" {
    token("==");
    return OP_EQ;
}
<INITIAL>"&&" {
    token("&&");
    return OP_AND;
}
<INITIAL>"||" {
    token("||");
    return OP_OR;
}
<INITIAL>"!" {
    token("!");
    return OP_NOT;
}

 /**** Interger Constant ****/
<INITIAL>0 |
<INITIAL>[1-9]{digit}* {
    tokenString("integer", yytext);
    return INTEGER_CONSTANT;
}
 
 /**** Floating-Point Constant ****/
<INITIAL>{digit}+"."{digit}+ {
    tokenString("float", yytext);
    return FLOATING_CONSTANT;
}

 /**** Scientic Notations ****/
<INITIAL>({digit}*("."{digit}+)?)(E|e)[+-]?{digit}+ { 
    tokenString("scientific", yytext);
    return FLOATING_CONSTANT;
}


 /*** String Constant ***/
 /* Enter String Mode */
<INITIAL>\" {
    LIST;
    strbuf[0] = '\0';
    BEGIN STRING_STATE;
}
 /* End of String */
<STRING_STATE>\" {
    LIST;   
    tokenStringN("string", strbuf);
    strbuf[0] = '\0';
    BEGIN INITIAL;
    return STRING_CONSTANT;
}
 /* escape:" */
<STRING_STATE>\\\" {
    LIST;
    SADD("\"");
}
 /* escape:\t */
<STRING_STATE>\\t {
    LIST;
    SADD("\t");
}
 /* escape:\n */
<STRING_STATE>\\n {
    LIST;
    SADD("\n");
}
 /* escape:\ */
<STRING_STATE>\\\\ {
    LIST;
    SADD("\\");
}
 /* Rest of character in string */
<STRING_STATE>. {
    LIST;
    SADD(yytext);
}


 /* Whitespace */
<INITIAL>[ \t] {
    LIST;
}

<STRING_STATE>\n          |
. { // Unrecognized character
    fprintf(ERR_FD, "Error at line %d: %s\n", linenum, yytext);
    dispose_id_table();
    exit(1);
}



%%

/* Declare ID-table related structs */

struct TableEntry{
    const char* id;
    int count;
};

typedef struct TableEntry ENTRY;

/* ID pool , for saving space */
char** id_pool = NULL;
int id_count = 0; // how many IDs in the pool

/* ID table , for sorting */
ENTRY* id_table  = NULL;
int table_count = 0;


/* new line process */
void line_process(void){
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}

/* Add new id entry in pool */
/* when full, return NULL */
char* add_new_id(const char* new_id){
    char* ptr = NULL;
    if(id_count < MAX_ID_COUNT){
        // if there is a space
        strncpy(id_pool[id_count],new_id,MAX_ID_LENGTH);
        ptr = id_pool[id_count++];
    }
    return ptr;
}


/* register a new id */
void register_id(const char* new_id){
    const char* id_ptr = NULL;
    int count = -1;
    // search in table
    int i=0;
    for(i=0;i<table_count;i++){
        if(strncmp(new_id,id_table[i].id,MAX_ID_LENGTH)==0){
            // find in table
            id_ptr = id_table[i].id;
            // increase counter
            count = id_table[i].count += 1;
            break;
        }
    }
    if(!id_ptr){ // Not found in table
        // Add new id to pool
        id_ptr = add_new_id(new_id);
        if(id_ptr){
            // when success
            id_table[table_count].id = id_ptr;
            count = id_table[table_count].count = 1;
            ++table_count;
        }
    }
    // debug output
    //printf("new id %s , count %d , at %p\n",new_id,count,id_ptr);
    

}

/* allocate id pool */
void init_id_pool(){
    id_count = 0;
    id_pool = (char**)malloc(sizeof(char*)*MAX_ID_COUNT);
    int i;
    for(i=0;i<MAX_ID_COUNT;i++){
        id_pool[i] = (char*)malloc(sizeof(char)*MAX_ID_LENGTH);
    }
}

/* release id pool */
void dispose_id_pool(){
    int i;
    for(i=0;i<MAX_ID_COUNT;i++){
        free(id_pool[i]);
    }
    free(id_pool);
}

/* init id table , include id pool */
void init_id_table(){
    // Init the pool first
    init_id_pool();    
    // create the table
    id_table = (ENTRY*)malloc(sizeof(ENTRY)*MAX_ID_COUNT);

    
}
/* release id table , include id pool */
void dispose_id_table(){
    
    // free the table
    free(id_table);
    // finally, dispose the pool
    dispose_id_pool();  
}

// compare function
int table_cmp(const void* va,const void* vb){
    const ENTRY* a = va;
    const ENTRY* b = vb;
    return a->count - b->count;
}

// Sort the id table with qsort
void sort_id_table(){
    qsort(id_table,table_count,sizeof(ENTRY),table_cmp);    
}

void show_freq_table(){
    // sort the table
    sort_id_table();
    // print the table
    printf("frequencies of identifiers:\n");
    int i;
    for(i=0;i<table_count;i++){
        printf("%s\t%d\n",id_table[i].id,id_table[i].count);
    }
}


/*
int main( int argc, char **argv )
{
    if (argc >= 2)
    {
        yyin = fopen( argv[1], "r" );
        if ( NULL == yyin ) {
            fprintf( ERR_FD, "Cannot open: %s\n", argv[1] );
            exit(-1);
        }
    }
    else
    {
        yyin = stdin;
    }

    init_id_table();

    buf[0] = '\0';
    yylex();

    if (Opt_Statistic)
    {
        show_freq_table();
        // Print frequency table
    }
    dispose_id_table();

    exit(0);
}
*/

