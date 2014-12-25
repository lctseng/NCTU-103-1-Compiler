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
#include <string.h>
#include "symtbl.h"


const char* DUMP_FORMAT = "%-33s%-11s%-12s%-19s%-24s\n";
const char* KIND_NAME[10];
const char* TYPE_NAME[] = {"int","float","double","bool","string","void"};
const char* OP_NAME[] = {"**ERROR**","arithmetic 'Add'","arithmetic 'Sub'","arithmetic 'Mul'","arithmetic 'Div'","arithmetic 'Mod'","relational"};
const char* OP_NEED_TYPE[] = {"**ERROR**","numeric operand","numeric operand","numeric operand","numeric operand","integer operand","numeric operand"};


extern int Opt_Source;
extern int Opt_Token;
extern int Opt_Statistic;
extern int Opt_Symbol;
extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */

char tempBuf[MAX_STRING_SIZE];
int funcTypeTmp;
bool supressError = false;
bool coercion_check_only = false; // when doing coercion, DO NOT modify source typeStruct_t
int scopeTypeTmp = SCOPE_ERROR;
int lastStmtType = STMT_ERROR; // last stmt type, for 'return check'

struct {
    int end;
    const char* pool[MAX_STRING_POOL];
} errorPool;

typedef struct {
    bool isDeclare;
    typeList_t* pTypeList; 
    int retType;
    bool paramAddOK;
} funcOption_t;

funcOption_t funcOption;

void init_funcOption();

void init_errorPool();
void add_errorPool(const char* msg);

// symbol type
typedef struct{
    bool is_const;
    int type;
    int dim;
    int dims[MAX_DIM+1]; // n-th dim. store in dims[n], scalar has dim = 0
} param_type;


typeStruct_t typeTmp;




void tmp_type_clear_array_indice(){
    typeTmp.dim = 0;
}

void tmp_type_init(){
    init_typeStruct(&typeTmp); 
    typeTmp.val = 0;
}

void tmp_type_set_scalar(int type){
    typeTmp.v_type = type;
}

void tmp_type_init_scalar(int type){
    tmp_type_init();
    tmp_type_set_scalar(type);
}


void tmp_type_add_dim(int dim){
    typeTmp.dims[typeTmp.dim] = dim;
    ++typeTmp.dim;
}

// type string generate
void generate_type_string(char* buf,typeStruct_t* pType){
    char indice[MAX_STRING_SIZE];
    bzero(buf,MAX_STRING_SIZE);
    strcat(buf,TYPE_NAME[pType->v_type]);
    for(int i=0;i<pType->dim;i++){
        snprintf(indice,MAX_STRING_SIZE,"[%d]",pType->dims[i]);
        strcat(buf,indice);
    }

}


// symbol attribute
typedef union{
    int int_val;
    float float_val;
    double double_val;
    int bool_val;
    char* string_val;
    typeList_t param_list;
} sym_attr;




// symbol table entry
typedef struct{
    char name[33];
    int kind;
    int level;
    typeStruct_t type;
    sym_attr attr;
    int option; 
    bool isDeclare;
    bool isMatch;
} s_table_entry;

// symbol table
typedef struct{
    s_table_entry entries[MAX_TABLE_ENTRY];
    int end;
    int level;
    int scopeType;
  
} symbol_table;

// table stack
typedef struct{
    symbol_table tables[MAX_TABLES];
    int end;
    int level;
    int pretend;
} table_stack;


// init a symbol table
void init_table(symbol_table* p_table,int level){
    p_table->end = 0;
    p_table->level = level;
    if(scopeTypeTmp != SCOPE_ERROR){
        p_table->scopeType = scopeTypeTmp;
        scopeTypeTmp = SCOPE_ERROR;
    }
    else{
        p_table->scopeType = SCOPE_NORMAL;
    }
}

// push new table and return it
// mode = PRETEND : push new table and reserve (will cancel next push and pretend that new table has been add)
symbol_table* push_table(int mode);
// pop new table and return it
symbol_table* pop_table();

// init a table stack
void init_table_stack();

// string pointer pool
struct string_pool {
    void* data[MAX_STRING_POOL];
    int end;
};

// declar pool
struct string_pool str_pool;


void add_ptr(void* str){
    int i = str_pool.end;
    str_pool.data[i] = str;
    //printf("Pool:Add[%d]='%s'\n",i,str_pool.data[i]);
    str_pool.end +=1 ;
}
// Add string
void add_string(char* str){
    add_ptr(str);
}

// output error msg
void print_error(const char* msg){
    if(!supressError){
        snprintf(tempBuf,MAX_STRING_SIZE,"##########Error at Line #%d: %s##########\n",linenum,msg);
        fprintf(ERR_FD,tempBuf);
        char* str = (char*)malloc(strlen(tempBuf)+1);
        strcpy(str,tempBuf);
        add_string(str);
        add_errorPool(str);
    }
}
// declare table
table_stack stk;
// get current table
symbol_table* current_table();
// get global table
symbol_table* global_table();
// create entry in current table
s_table_entry* create_entry(){
    symbol_table* table = current_table(); 
    s_table_entry* ent =&table->entries[table->end];
    ent->level = table->level;
    //DEBUG//printf("New Table Entry created in level %d, index %d\n",table->level,table->end);
    table->end++;
    return ent;
}

// check variable entry exist 
// check : same scope conflict 
bool check_variable_entry_exist(const char* name,int kind){ 
    // buf
    char errbuf[MAX_STRING_SIZE];
    // get current table
    symbol_table* table = current_table(); 
    // search current table
    for(int i=0;i<table->end;i++){
        s_table_entry* ent = &table->entries[i]; 
        if(strcmp(name,ent->name)==0){
            // name conflict, no matter which type  (function,variable,constant,parameter...)
            //DEBUG//printf("Name Conflict for %s\n",name);
            snprintf(errbuf,MAX_STRING_SIZE,"%s identifier '%s' has been declared in the same scope.",KIND_NAME[kind],name);
            print_error(errbuf);
            return false;
        }
    }
    // no conflict
    return true;
}

// check function addable, according to define or declarbool check_function_entry_addable(name,kind){
bool check_function_entry_addable(const char* name,int kind){
    // check 
    if(funcOption.isDeclare){
        // declare, no one can appear before it, rule same as variable
        return check_variable_entry_exist(name,kind);
    }
    else{
        // definition, allow previous is a declare
        // buf
        char errbuf[MAX_STRING_SIZE];
        // get current table
        symbol_table* table = current_table(); 
        // search current table
        for(int i=0;i<table->end;i++){
            s_table_entry* ent = &table->entries[i]; 
            if(strcmp(name,ent->name)==0){
                // check if previous one is declare-function
                if(ent->kind==K_FUNCTION && ent->isDeclare){
                    //DEBUG//printf("Declare has founed,checking... : %s\n",name);
                    // check return type
                    if(funcOption.retType != T_ERROR && ent->type.v_type != funcOption.retType){
                        print_error("Return type of function definition is not match with declaration");
                        return false;
                    }
                    // check param type
                    if(!check_typeList_consist(&ent->attr.param_list,funcOption.pTypeList,false)) {
                        print_error("Parameter types of function definition is not match with declaration");
                        return false;
                    }
                    funcOption.paramAddOK = true;
                    ent->isMatch = true;
                    return false;
                }
                else{
                    // name conflict for non-declare
                    //DEBUG//printf("Name Conflict for %s\n",name);
                    snprintf(errbuf,MAX_STRING_SIZE,"Function identifier '%s' has been used before.",name);
                    print_error(errbuf);
                    return false;
                }
            }
        }
        // no conflict
        return true;
    }
}

// check declare dimension need > 0
// return true if not an array
bool check_dimension(typeStruct_t* pType){
    for(int i=0;i<pType->dim;i++){
        if(pType->dims[i] <= 0){
            print_error("Array index must greater than 0");
            return false;
        }
    }
    return true;
}

// check addable
bool check_entry_addable(const char* name,int kind){
    bool result = true;
    // type error check
    //DEBUG//printf("Type code:%d\n",typeTmp.v_type);
    result = result&&(typeTmp.v_type >= 0);
    // kind check
    switch(kind){
        case K_VARIABLE: case K_CONSTANT: case K_PARAMETER:
            // type cannot be void
            if(typeTmp.v_type == T_VOID){
                print_error("'void' cannot be used to declare variable/constant/parameter");
                result = false;
            }
            // other check
            result = result&&check_variable_entry_exist(name,kind);     
            result = result&&check_dimension(&typeTmp); 
            break;
        case K_FUNCTION:
            result = result&&check_function_entry_addable(name,kind);
            break;
        default:
            // keep
            break;
    }
    return result;
}

void init_typeStruct(typeStruct_t* pType){
    pType->v_type = T_ERROR;
    pType->dim = 0;
    pType->is_const = false;
    pType->is_raw = false;
}

// add table entry with name
// option : extra augment -- CANCEL
s_table_entry* create_basic_entry(const char* name,int kind){
    //DEBUG//printf("start entry check\n");
    if(!check_entry_addable(name,kind)){
        return NULL;
    }  
    //DEBUG//printf("pass entry check\n");
    // create  new entry
    s_table_entry* ent = create_entry();
    // set name
    bzero(ent->name,sizeof(ent->name));
    strncpy(ent->name,name,32);
    // set kind
    ent->kind = kind;
    // set type
    // if it is function, type will extract from funcOption
    if(kind == K_FUNCTION){
        init_typeStruct(&ent->type); 
        //DEBUG//printf("Function Ret Type : %d\n", funcOption.retType);
        ent->type.v_type = funcOption.retType;
    }
    else{
        memcpy(&ent->type,&typeTmp,sizeof(typeTmp));    
    }
    // clear array
    tmp_type_clear_array_indice();
    // set option
    ent->option = 0;
    // set declare
    if(kind==K_FUNCTION){
        ent->isDeclare = funcOption.isDeclare;
    }
    else{
        ent->isDeclare = false;
    }
    // set other
    ent->isMatch = false;
    // return
    return ent;
}


s_table_entry* process_constant_declare(const char* name,typeStruct_t type) {
    // basic entry
    s_table_entry* ent =  create_basic_entry(name,K_CONSTANT);
    if(ent){ // if that entry has successfully added
        // fill attribute
        switch(type.v_type){
            case T_INT:
                ent->attr.int_val = type.val;
                break;
            case T_FLOAT:
                ent->attr.float_val = type.fval;
                break;
            case T_DOUBLE:
                ent->attr.double_val = type.dval;
                break;
            case T_BOOL:
                ent->attr.bool_val = type.val;
                break;
            case T_STRING:
                ent->attr.string_val = type.sval; 
                break;
        }
    }
    return ent;
}

bool check_dim_consist(typeStruct_t* a,typeStruct_t* b){
    if(a->dim != b->dim){
        return false;
    }    
    for(int i=0;i<a->dim;i++){
        if(a->dims[i]!=b->dims[i]) {
            //DEBUG//printf("Dim[%d] differ : %d and %d\n",i,a->dims[i],b->dims[i]);
            return false;
        }   
    }
    return true;
}

// check and try to do coercion
// it will change later to fit earlier
bool do_type_coercion(typeStruct_t* pTarget,typeStruct_t* pType){
    //DEBUG//printf("Doing Coercion\n");
    bool result = false;
    int src_type = pType->v_type;
    int targetType = pTarget->v_type;
    bool no_show_err = false;
    if(pTarget->v_type == T_ERROR || pType->v_type == T_ERROR){
        // no coercion when someone is T_ERROR
        //DEBUG//printf("Coercion error on previous error!");
        no_show_err = true;
    }
    else if(!check_dim_consist(pTarget,pType)){
        // dim must be the same to do coercion
        print_error("Caonnot perform coercion on arrays with different size or dimension");
        //DEBUG//printf("Dim %d(%s) to %d(%s)\n",pType->dim,TYPE_NAME[pType->v_type],pTarget->dim,TYPE_NAME[pTarget->v_type]);
        no_show_err = true;
    }
    else if(targetType == pType->v_type){
        //DEBUG//printf("%s to %s\n",TYPE_NAME[src_type],TYPE_NAME[targetType]);
        // same type do not need coercion
        result =  true;
    }
    else{
        //DEBUG//printf("Coercion : %s to %s\n",TYPE_NAME[src_type],TYPE_NAME[targetType]);
        switch(targetType){
            case T_FLOAT: // allow INT to FLOAT
                if(src_type==T_INT){
                    if(!coercion_check_only){
                        if(pType->is_const){ // only constant can convert value
                            pType->fval = (float)pType->val;
                            //DEBUG//printf("Convert INT(%d) to FLOAT(%f)...\n",pType->val,(float)pType->val);
                        }
                        pType->v_type = T_FLOAT;
                    }
                    result = true;
                }
                else if(pType->v_type==T_DOUBLE && pType->is_const && pType->is_raw){
                    // raw float
                    if(!coercion_check_only){
                        pType->fval = (float)pType->dval;
                        pType->v_type = T_FLOAT;
                    }
                    result = true;
                }
                else{
                    result = false;
                }
                break;
            case T_DOUBLE:
                if(!coercion_check_only){
                    pType->v_type = T_DOUBLE;
                }
                switch(src_type){ // source type
                    case T_INT: // INT to DOUBLE
                        if(pType->is_const){
                            pType->dval = (double)pType->val;
                        }
                        result = true;
                    case T_FLOAT: // FLOAT to DOUBLE
                        if(pType->is_const){
                            pType->dval = (double)pType->fval;
                        }
                        result =  true;
                    default :
                        result = false;
                }
                result = true;
                break;
            default:
                result = false;
        }
    }
    //DEBUG//printf("Coercion end\n");
    if(!result && !no_show_err){
        no_show_err = true;
        char msg[MAX_STRING_SIZE];
        snprintf(msg,MAX_STRING_SIZE,"'%s' cannot be coerced into '%s'",TYPE_NAME[src_type],TYPE_NAME[targetType]); 
        print_error(msg);
    }
    else{
        if(src_type != targetType){
            //DEBUG//printf("Coerced '%s' to '%s' success\n",TYPE_NAME[src_type],TYPE_NAME[targetType]);
        }
    }
    return result;
}

// check if that assignment is ok
// assign b to a, thus a cannot be const
bool check_variable_assign_able(typeStruct_t* a,typeStruct_t* b){
    // no process for error type
    if(a->v_type == T_ERROR || b->v_type == T_ERROR){
        return false;
    }
    // no array assign
    if(a->dim > 0 || b->dim > 0){
        print_error("Array cannot be involved in assignment");
        return false;
    }
    // check constant
    //DEBUG//printf("Const : %d\n",a->is_const);
    if(a->is_const){
        print_error("Cannot do assignment to constant variable");
        return false;
    }
    // do coercion
    //DEBUG//printf("Variable assign check  type: %s to %s\n",TYPE_NAME[b->v_type],TYPE_NAME[a->v_type]);
    return do_type_coercion(a,b);
}

// check if two type struct is all the same, but errors are distinct
bool check_typeStruct_same(typeStruct_t* a,typeStruct_t* b){
    if(a->v_type == T_ERROR || b->v_type == T_ERROR){
        return false; // error return
    }
    //DEBUG//printf("type check : %s , %s \n",TYPE_NAME[a->v_type],TYPE_NAME[b->v_type]);
    
    // check basic type
    if(a->v_type != b->v_type){
        return false;
    }
    // check dim
    if(a->dim != b->dim){
        return false;
    }
    // check each
    for(int i=0;i<a->dim;i++){
        if(a->dims[i]!=b->dims[i]){
            return false;
        }
    }
    return true;
}

bool check_typeList_consist(typeList_t* a,typeList_t* b,bool allow_coercion){
    bool result = true;
    // check dim
    if(a->end != b->end){
        //DEBUG//printf("Dim mismatch for %d and %d\n",a->end,b->end);
        result = false;
    }
    // check each dims
    for(int i=0;i<a->end&&result;i++){
        if(allow_coercion){    
            if(!do_type_coercion(&a->data[i],&b->data[i])){
                //DEBUG//printf("Coercion failed\n");
                result = false;
                break;
            }
        }
        else{ // no coercion, each type list must be the same
            //DEBUG//printf("***NO COERCION***\n");
            //DEBUG//printf("check in %d\n",i);
            if(!check_typeStruct_same(&a->data[i],&b->data[i])){
                result = false;
                break;
            }
            
        }
    }

    return result;
}

// check consistant
int check_function_call_consist(const char* name,typeList_t* pTypeList){
    int ret_type = T_ERROR; // -1 means not found
    int errType = ENTRY_NOT_FOUND; // extra error msg
    // search global table for function 
    bool old_supress = supressError;
    supressError = true;
    symbol_table* pTable = global_table();
    for(int i=0;i<pTable->end;i++){
        s_table_entry* ent = &pTable->entries[i];
        // function kind only
        if(ent->kind == K_FUNCTION){
            // same name
            if(strcmp(name,ent->name)==0){
                // check type list consist
                //DEBUG//printf("Check function call...\n");
                if(check_typeList_consist(&ent->attr.param_list,pTypeList,true)){
                    // find return function type
                    ret_type = ent->type.v_type;
                    //DEBUG//printf("Table search : type '%s' for function '%s'\n",TYPE_NAME[ret_type],name);
                }
                else{
                    // parameter mismatch
                    ret_type = T_ERROR;
                    errType = ENTRY_MISMATCH;
                }
                break;
            }
        }
    }
    supressError = old_supress;
    char errmsg[MAX_STRING_SIZE];
    if(ret_type < 0){
        switch(errType){
            case ENTRY_NOT_FOUND:
                snprintf(errmsg,MAX_STRING_SIZE,"Function '%s' isn't declared.",name);
                break;
            case ENTRY_MISMATCH:
                snprintf(errmsg,MAX_STRING_SIZE,"Function call '%s' error : parameter types are not consist with its declaration",name);
                break;
            default:
                bzero(errmsg,MAX_STRING_SIZE);
                break;
        }
        print_error(errmsg);
    }
    return ret_type;
}

// generate constant attribute
void generate_constant_attr_string(char* buf,s_table_entry* ent){
    size_t buf_sz = MAX_STRING_SIZE;
    // write attribute
    switch(ent->type.v_type){
        case T_INT:
            snprintf(buf,buf_sz,"%d",ent->attr.int_val);
            break;
        case T_FLOAT:
            snprintf(buf,buf_sz,"%f",ent->attr.float_val);
            break;
        case T_DOUBLE:
            snprintf(buf,buf_sz,"%lf",ent->attr.double_val);
            break;
        case T_BOOL:
            snprintf(buf,buf_sz,"%s",ent->attr.int_val == 1 ? "true" : "false");
            break;
        case T_STRING:
            snprintf(buf,buf_sz,"%s",ent->attr.string_val);
            break;
    } 
}
// generate param list
void generate_function_attr_atring(char* buf,s_table_entry* ent){
    bzero(buf,MAX_STRING_SIZE); 
    // for each type
    for(int i=0;i<ent->attr.param_list.end;i++){
        // comma
        if(i!=0){
            strcat(buf,",");
        }
        typeStruct_t* pType = &ent->attr.param_list.data[i];
        // scalar type
        strcat(buf,TYPE_NAME[pType->v_type]);
        // array indice
        for(int j=0;j<pType->dim;j++){
            snprintf(tempBuf,MAX_STRING_SIZE,"[%d]",pType->dims[j]);
            strcat(buf,tempBuf);
        }
    }
}

// attr string generate
void generate_attr_string(char* buf,s_table_entry* ent){
    bzero(buf,MAX_STRING_SIZE); 
    switch(ent->kind){
        case K_CONSTANT:
            generate_constant_attr_string(buf,ent);
            break;
        case K_FUNCTION:
            generate_function_attr_atring(buf,ent);
            break;
    }
}

// table dump
void dump_table(symbol_table* table){
    if(!Opt_Symbol){
        return;
    }
    char levelStr[MAX_STRING_SIZE];
    char typeStr[MAX_STRING_SIZE];
    char attrStr[MAX_STRING_SIZE];
    
    printf("=======================================================================================\n");
    printf(DUMP_FORMAT,"Name","Kind","Level","Type","Attribute");
    printf("---------------------------------------------------------------------------------------\n");
    for(int i=0;i<table->end;i++){
        s_table_entry* ent = &table->entries[i];
        snprintf(levelStr,MAX_STRING_SIZE,"%d(%s)",ent->level,ent->level > 0 ? "local" : "global" );
        generate_type_string(typeStr,&ent->type);
        generate_attr_string(attrStr,ent);
        printf(DUMP_FORMAT,ent->name,KIND_NAME[ent->kind],levelStr,typeStr,attrStr);

    } 
    printf("=======================================================================================\n");
}
// dump current table
void dump_current_table(){
    dump_table(current_table());
}



void process_check_eof();

void program_reduce_process(){
    dump_current_table();
    process_check_eof();
}

bool isIntegerType(int type){
    return type ==T_INT;
}
bool isFloatType(int type){
    return type ==T_FLOAT;
}
bool isDoubleType(int type){
    return type ==T_DOUBLE;
}

bool isNumericType(int type){
    return isIntegerType(type) || isFloatType(type) || isDoubleType(type);
}


typedef bool (*funcTypeCheck)(int);

funcTypeCheck get_binary_numeric_expr_check_func(int op){
    switch(op){
        case OP_ADD:case OP_SUB: case OP_MUL: case OP_DIV: case OP_RELATION:
            return &isNumericType;
        case OP_MOD:
            return &isIntegerType;
    }
    return NULL;
}

bool operatorTypeChecker(typeStruct_t* pType,int op,funcTypeCheck checker,const char* errFmt){
    bool result = true;
    if(!checker || !checker(pType->v_type))
    {
        result = false;
        if(pType->v_type != T_ERROR){
            // if previous has no error, means this time has a convert error
            char errmsg[MAX_STRING_SIZE];
            snprintf(errmsg,MAX_STRING_SIZE,errFmt,OP_NAME[op],OP_NEED_TYPE[op],TYPE_NAME[pType->v_type]);
            print_error(errmsg);
        }
        else{
            // error cause by previous error
            //DEBUG//printf("Previous error occur on %s operator, require %s \n",OP_NAME[op],OP_NEED_TYPE[op]);
        }

    }
    return result;
    
}

int extractGreaterType(int a,int b){
    if(a == T_DOUBLE || b == T_DOUBLE){
        return T_DOUBLE;
    }
    else if(a == T_FLOAT || b == T_FLOAT){
        return T_FLOAT;
    }
    else  
    {
        return T_INT;
    }
}

bool noArrayCheck(typeStruct_t* pType){
    bool result = true;
    if(pType->v_type != T_ERROR && pType->dim > 0){
        char errmsg[MAX_STRING_SIZE];
        snprintf(errmsg,MAX_STRING_SIZE,"%d dimension of %s array type is not allow here.",pType->dim,TYPE_NAME[pType->v_type]);
        print_error(errmsg);
        result = false;
    }
    return result;
}

// check whether given expr has correct type for binary numeric operator, return the final computation type
int binary_numeric_expr_check(typeStruct_t* lhs,typeStruct_t* rhs,int op){
    int ret_type = T_ERROR;
    funcTypeCheck checker = get_binary_numeric_expr_check_func(op);
    if(!noArrayCheck(lhs)||!noArrayCheck(rhs)){ // check if has array type
        ret_type = T_ERROR;       
    }
    else if(!operatorTypeChecker(lhs,op,checker,"%s operator expect a(n) '%s' on left-hand-side, but '%s' provided.")){ // LHS
        ret_type = T_ERROR;      
    }
    else if(!operatorTypeChecker(rhs,op,checker,"%s operator expect a(n) '%s' on right-hand-side, but '%s' provided.")){ // RHS
        ret_type = T_ERROR;      
    }
    else{ // all correct!
        // set return type
        switch(op){
            case OP_RELATION:
                ret_type = T_BOOL;
                break;
            case OP_MOD:
                ret_type = T_INT;
                break;
            case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV:
                ret_type = extractGreaterType(lhs->v_type,rhs->v_type); 
                break;
        }

    }
    return ret_type;
    
}

bool checkFlag(int checked,int flag){
    return (flag & checked) > 0;
}

// find variable/parameter/constant type for that name
void fill_type_for_name(typeStruct_t* pType,const char* name,int flag){
    bool found = false;
    // search each table
    //DEBUG//printf("***Look Flag:%X\n",flag);
    for(int i = stk.end-1;i>=0&&!found;i-- ){
        symbol_table* pTable = &stk.tables[i];
        //DEBUG//printf("==>Table look up %s at level %d\n",name,pTable->level);
        for(int j = pTable->end-1;j>=0&&!found;j--){
            s_table_entry* ent = &pTable->entries[j];
            //DEBUG//printf("Flag Check : %X & %X => %d\n",ent->kind,flag,checkFlag(ent->kind,flag));
            if((checkFlag(ent->kind,flag))&&strcmp(ent->name,name)==0){
                // same name and kind are consist
                // fill every fields
                pType->v_type = ent->type.v_type;
                pType->dim = ent->type.dim;
                memcpy(&pType->dims,&ent->type.dims,sizeof(pType->dims));
                // set constant
                if(ent->kind == K_CONSTANT){
                    pType->is_const = true;
                }
                else{
                    pType->is_const = false;
                }
                //DEBUG//printf("===> Found '%s' as type : %s, const : %s , dimensions : %d\n",name,TYPE_NAME[pType->v_type],pType->is_const ? "true":"false",pType->dim);
                found = true;
            }
        }
    }
    if(!found){
        char errmsg[MAX_STRING_SIZE];
        snprintf(errmsg,MAX_STRING_SIZE,"variable/parameter/constant identifier '%s' not declared.",name);
        print_error(errmsg);
        pType->v_type = T_ERROR;
    }
}


void perform_negation(typeStruct_t* pType){
    //DEBUG//printf("Performing Negation...type id :%d\n",pType->v_type);
    switch(pType->v_type){
        case T_FLOAT:
            pType->fval *= -1;
            break;
        case T_DOUBLE:
            pType->dval *= -1;
            break;
        case T_INT:
            pType->val *= -1;
            //DEBUG//printf("Integer negated val : %d\n", pType->val);
            break;
    }
}

// check if array initializer type and size are OK
bool check_array_initializer(typeStruct_t* pIndice,typeList_t* pList){
    int total = 1;
    // run each dim and check the dim
    for(int i=0;i<pIndice->dim;i++){
        int dim = pIndice->dims[i];
        if(dim <= 0){
            print_error("Array initializer:Array index must greater than or equal to 0");
            return false;
        }
        total *= dim;
    }
    // check count exceeds?
    if(pList->end > total){
        print_error("Array initializer:The number of initializer exceeds the array size");
        return false;
    }
    // check expression match
    // valid struct
    typeStruct_t valid;
    init_typeStruct(&valid);
    valid.v_type = pIndice->v_type;
    // coercion only need check
    bool old_check = coercion_check_only;
    coercion_check_only = true;
    for(int i=0;i<pList->end;i++){
        typeStruct_t* pType = &pList->data[i];
        if(!do_type_coercion(&valid,pType)){
            // convert fail
            return false;
        }       
    }
    coercion_check_only = old_check;
    // pass
    return true;

}

// check current table or outter table has that scope rule
bool check_scope_match(int scopeRule){
    for(int i = stk.end-1;i>=0;i-- ){
        symbol_table* pTable = &stk.tables[i];
        //DEBUG//printf("==>Table check at level %d , scope : %d\n",pTable->level,pTable->scopeType);
        if(checkFlag(pTable->scopeType,scopeRule)){
            return true;
        }
    }
    return false;

    
}

// check is in loop
bool check_in_loop(){
    return check_scope_match(SCOPE_LOOP);
}

// process loop check
bool process_loop_check(){
    if(!check_in_loop()){
        print_error("'break/continue' can only used inside loops");
    } 
}

// check is in retrun-able function
bool check_in_return_function(){
    return check_scope_match(SCOPE_FUNCTION);
}


// process return-able check
bool process_return_function_check(typeStruct_t* pType){
    // check-in function
    if(!check_in_return_function()){
        print_error("'return' can only used inside non-void function");
        return false;
    } 
    // scalar check
    if(pType->dim > 0){
        print_error("return value should not be an array type");
        return false;
    }
    // check function type
    bool old_check = coercion_check_only;
    coercion_check_only = true;
    typeStruct_t target;
    init_typeStruct(&target);
    target.v_type = funcOption.retType;
    if(!do_type_coercion(&target,pType)){
        return false;       
    }
    coercion_check_only = old_check;
    return true;
}

// check if expr is printable
bool check_printable(typeStruct_t* pType){
    // ignore when previous error
    if(pType->v_type==T_ERROR){
        return false;
    }
    // array can't print
    if(pType->dim > 0){
        print_error("Cannot use 'print' on an array variable.");
        return false;
    }
    return true;
     
}

// check if var_ref is readable for KW_READ
bool check_readable(typeStruct_t* pType){
    // ignore when error
    if(pType->v_type==T_ERROR){
        return false;
    }
    // constant can't read
    if(pType->is_const){
        print_error("Cannot use 'read' on a constant variable.");
        return false;
    }
    // cannot read array
    if(pType->dim > 0){
        print_error("Cannot use 'read' on an array variable.");
        return false;
    }
    return true;
}


void go_to_loop_scope(){
    //DEBUG//printf("Go into a loop scope\n");
    scopeTypeTmp = SCOPE_LOOP;
}

// END_DECLARE

#line 1084 "y.tab.c" /* yacc.c:339  */

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
    ARITHMETIC_PARENTHESES = 268,
    LOWER_THEN_ARITHMETIC_PARENTHESES = 269,
    OP_OR = 270,
    OP_AND = 271,
    OP_NOT = 272,
    OP_LT = 273,
    OP_LE = 274,
    OP_EQ = 275,
    OP_GE = 276,
    OP_GT = 277,
    OP_NE = 278,
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
#define OP_OR 270
#define OP_AND 271
#define OP_NOT 272
#define OP_LT 273
#define OP_LE 274
#define OP_EQ 275
#define OP_GE 276
#define OP_GT 277
#define OP_NE 278
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
#line 1020 "parser.y" /* yacc.c:355  */

    int value;
    char* text;
    int type;
    typeStruct_t typeStruct;
    typeList_t typeList;

#line 1212 "y.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1227 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   287

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  125
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  217

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    30,     2,     2,
      15,    16,    28,    26,     2,    27,     2,    29,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,     2,    49,     2,     2,     2,     2,
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
      17,    18,    19,    20,    21,    22,    23,    24,    25,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1093,  1093,  1097,  1102,  1103,  1104,  1107,  1108,  1109,
    1110,  1111,  1112,  1115,  1118,  1119,  1120,  1121,  1122,  1123,
    1127,  1128,  1132,  1133,  1136,  1140,  1156,  1168,  1174,  1175,
    1177,  1185,  1188,  1189,  1192,  1207,  1242,  1243,  1244,  1245,
    1251,  1284,  1285,  1291,  1291,  1371,  1372,  1377,  1387,  1399,
    1413,  1428,  1428,  1442,  1443,  1446,  1447,  1450,  1451,  1454,
    1455,  1458,  1459,  1464,  1465,  1466,  1467,  1468,  1469,  1470,
    1474,  1475,  1479,  1482,  1487,  1487,  1488,  1488,  1493,  1493,
    1496,  1497,  1500,  1501,  1502,  1505,  1511,  1512,  1516,  1517,
    1518,  1522,  1523,  1524,  1527,  1533,  1569,  1577,  1583,  1618,
    1634,  1635,  1636,  1637,  1638,  1639,  1642,  1643,  1646,  1651,
    1656,  1661,  1666,  1671,  1691,  1692,  1696,  1729,  1746,  1747,
    1748,  1753,  1770,  1772,  1779,  1785
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "BOOL", "FLOAT", "DOUBLE",
  "STRING", "CONST", "VOID", "END", "LESS_THAN_ELSE", "KW_ELSE",
  "ARITHMETIC_PARENTHESES", "LOWER_THEN_ARITHMETIC_PARENTHESES", "'('",
  "')'", "OP_OR", "OP_AND", "OP_NOT", "OP_LT", "OP_LE", "OP_EQ", "OP_GE",
  "OP_GT", "OP_NE", "'+'", "'-'", "'*'", "'/'", "'%'", "KW_RETURN",
  "KW_BREAK", "KW_CONTINUE", "KW_FOR", "KW_WHILE", "KW_DO", "KW_IF",
  "SEMICOLON", "ID", "ASSIGN", "COMMA", "KW_PRINT", "KW_READ",
  "INTEGER_CONSTANT", "FLOATING_CONSTANT", "BOOLEAN_CONSTANT",
  "STRING_CONSTANT", "'{'", "'}'", "'['", "']'", "$accept", "program",
  "decl_and_def_list", "declaration_list", "var_decl", "type",
  "identifier", "var_list", "var_entry", "scalar_id", "array_indice",
  "single_array_indice", "const_decl", "const_list", "single_const",
  "literal_constant", "funct_decl", "definition_list", "funct_def", "$@1",
  "args", "single_arg", "arg_list", "compound_stat", "$@2",
  "local_decl_list", "var_decl_list", "var_decl_entry",
  "zero_or_more_stat_list", "stat_list", "stat", "if_stat", "if_only_stat",
  "if_else_stat", "while_stat", "$@3", "$@4", "for_stat", "$@5",
  "for_bool_expr", "for_expr", "single_assginment",
  "one_or_more_for_assignment", "jump_stat", "simple_stat", "var_ref",
  "intexpr_array_indice", "intexpr_single_array_indice", "bool_expr",
  "relation_op", "binary_logic_op", "expr", "funct_call",
  "zero_or_more_expr_list", "expr_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,    40,    41,   270,   271,   272,
     273,   274,   275,   276,   277,   278,    43,    45,    42,    47,
      37,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   123,   125,
      91,    93
};
# endif

#define YYPACT_NINF -126

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-126)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     139,  -126,  -126,  -126,  -126,  -126,   274,  -126,     7,   139,
    -126,   -28,  -126,  -126,  -126,   -28,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,   -24,   -21,    -2,   139,    14,     8,     0,
     139,  -126,   -28,   201,    63,    13,    22,    20,  -126,  -126,
    -126,    49,  -126,   -28,  -126,     9,   274,   -28,    67,  -126,
      43,    63,    63,    63,  -126,  -126,  -126,  -126,    -3,  -126,
    -126,   224,  -126,    35,    41,  -126,   -28,   -28,  -126,   138,
    -126,  -126,   -28,  -126,    20,    54,   201,   210,   235,  -126,
      63,    63,  -126,    47,  -126,  -126,  -126,  -126,  -126,  -126,
    -126,  -126,    63,    63,    63,    63,    63,    63,    63,  -126,
      63,     3,    83,  -126,  -126,  -126,  -126,    53,  -126,  -126,
     224,    88,    64,   135,  -126,    51,    51,  -126,  -126,  -126,
     246,   235,    57,   201,   201,  -126,  -126,  -126,    63,  -126,
    -126,    96,    97,   139,   224,    54,  -126,  -126,   -28,  -126,
     181,  -126,   139,    63,    77,    78,  -126,  -126,  -126,   103,
      63,   -28,  -126,    73,   181,  -126,  -126,  -126,  -126,  -126,
    -126,    85,  -126,  -126,    84,    90,  -126,   149,  -126,  -126,
     111,   115,    53,    63,   173,    47,    93,  -126,  -126,  -126,
      63,  -126,  -126,    63,    63,    98,   119,   224,  -126,  -126,
     224,    99,  -126,    95,    84,   224,   123,   134,    53,    63,
     -28,    53,    63,   156,   112,  -126,  -126,  -126,   164,    53,
      63,   150,  -126,   176,  -126,    53,  -126
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    14,    15,    17,    16,    18,     0,    19,     0,     0,
      11,     0,    10,    12,     6,     0,     1,     8,     7,     9,
       6,    27,    24,     0,    22,    20,     3,     0,     0,    32,
       2,    13,     0,    46,     0,     0,    21,    29,     4,     5,
      42,     0,    31,     0,    23,    20,     0,     0,     0,    50,
      45,     0,     0,     0,    36,    37,    38,    39,    94,   118,
     120,    25,   119,     0,     0,    28,     0,     0,    41,     0,
      34,    33,     0,    47,    20,    43,     0,     0,   117,   113,
     123,     0,    95,    97,   107,   106,   100,   101,   105,   103,
     102,   104,     0,     0,     0,     0,     0,     0,     0,    30,
     123,    20,     0,    35,    48,    21,    40,     0,    49,   114,
     124,     0,   122,     0,    96,   108,   109,   110,   111,   112,
     115,   116,     0,    46,    46,    51,    44,   121,     0,    98,
      26,     0,     0,    54,   125,     0,    43,    57,     0,    58,
      60,    53,    56,     0,     0,     0,    78,    74,    76,     0,
       0,     0,    63,     0,    59,    62,    65,    70,    71,    66,
      67,     0,    68,    64,     0,     0,    55,     0,    89,    90,
       0,     0,     0,     0,     0,    94,     0,    52,    61,    91,
       0,    69,    88,    84,     0,     0,     0,    99,    92,    93,
      85,     0,    86,    83,   120,    82,     0,     0,     0,    81,
       0,     0,     0,    72,     0,    80,    87,    75,     0,     0,
      84,     0,    73,     0,    77,     0,    79
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -126,  -126,   199,   221,    -8,    -4,   -22,   190,  -126,   -11,
     -34,  -126,    -1,   182,  -126,   -25,     1,  -126,     6,  -126,
     -97,   167,  -126,   -98,  -126,  -126,   140,  -126,  -126,  -126,
     130,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
      75,  -125,  -126,  -126,  -126,  -123,   203,  -126,  -111,  -126,
    -126,   -29,  -121,   187,  -126
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,    26,    38,    10,    11,    22,    23,    24,    58,
      36,    37,    12,    28,    29,    59,    13,    39,    40,   107,
      48,    49,    50,   152,   133,   140,   141,   142,   153,   154,
     155,   156,   157,   158,   159,   171,   172,   160,   170,   204,
     191,   161,   193,   162,   163,    60,    82,    83,   186,    97,
      98,   187,    62,   111,   112
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,    17,    15,    65,    27,    61,    14,    16,    18,   126,
      19,    21,    80,    33,    31,    20,    70,   164,   123,   165,
      32,    45,    77,    78,    79,    73,   131,   132,   176,    47,
      17,   164,    27,   165,    66,    67,    74,    18,    34,    19,
     105,    43,    72,    34,   103,    68,    42,    81,    35,    34,
     104,   110,   113,    35,    41,   101,   102,    63,   192,    35,
     194,    74,    64,   115,   116,   117,   118,   119,   120,   121,
      35,   110,    47,   196,   185,   206,    69,   164,    51,    94,
      95,    96,    52,    75,    76,   192,    99,   194,   205,   100,
      53,   208,   106,    54,    55,    56,    57,    81,   124,   134,
     203,   125,    21,   207,   127,   128,   130,    54,    55,    56,
      57,   212,   135,   136,   167,   168,   169,   216,   173,    47,
      47,   174,   177,   179,   180,   137,   183,    45,   181,   138,
     184,   189,   139,   197,   137,   198,   200,   199,   138,   201,
     175,   139,     1,     2,     3,     4,     5,     6,     7,   202,
     210,   190,    84,    85,   195,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    84,    85,   209,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
     211,   195,    54,    55,    56,    57,   129,   182,   214,   175,
      84,    85,   215,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,     1,     2,     3,     4,     5,    46,
       7,   188,   143,   144,   145,   146,   147,   148,   149,    30,
      21,     9,    44,   150,   151,    71,   109,    84,    85,   125,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    84,    85,   108,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,     1,     2,     3,
       4,     5,   166,     7,   178,   213,   114,   122
};

static const yytype_uint8 yycheck[] =
{
      11,     9,     6,    37,    15,    34,     0,     0,     9,   107,
       9,    39,    15,    15,    38,     9,    41,   140,    15,   140,
      41,    32,    51,    52,    53,    47,   123,   124,   151,    33,
      38,   154,    43,   154,    38,    39,    47,    38,    40,    38,
      74,    41,    46,    40,    69,    39,    38,    50,    50,    40,
      72,    80,    81,    50,    40,    66,    67,    44,   183,    50,
     183,    72,    40,    92,    93,    94,    95,    96,    97,    98,
      50,   100,    76,   184,   172,   200,    27,   200,    15,    28,
      29,    30,    19,    16,    41,   210,    51,   210,   199,    48,
      27,   202,    38,    44,    45,    46,    47,    50,    15,   128,
     198,    48,    39,   201,    16,    41,    49,    44,    45,    46,
      47,   209,    16,    16,   143,    38,    38,   215,    15,   123,
     124,   150,    49,    38,    40,   133,    15,   138,    38,   133,
      15,    38,   133,    35,   142,    16,    41,    38,   142,    16,
     151,   142,     3,     4,     5,     6,     7,     8,     9,    15,
      38,   180,    17,    18,   183,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    17,    18,    12,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      16,   210,    44,    45,    46,    47,    51,    38,    38,   200,
      17,    18,    16,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,     3,     4,     5,     6,     7,     8,
       9,    38,    31,    32,    33,    34,    35,    36,    37,    20,
      39,     0,    32,    42,    43,    43,    16,    17,    18,    48,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    17,    18,    76,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     3,     4,     5,
       6,     7,   142,     9,   154,   210,    83,   100
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    53,    55,
      56,    57,    64,    68,    70,    57,     0,    56,    64,    68,
      70,    39,    58,    59,    60,    61,    54,    61,    65,    66,
      54,    38,    41,    15,    40,    50,    62,    63,    55,    69,
      70,    40,    38,    41,    59,    61,     8,    57,    72,    73,
      74,    15,    19,    27,    44,    45,    46,    47,    61,    67,
      97,   103,   104,    44,    40,    62,    57,    57,    70,    27,
      67,    65,    57,    58,    61,    16,    41,   103,   103,   103,
      15,    50,    98,    99,    17,    18,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,   101,   102,    51,
      48,    61,    61,    67,    58,    62,    38,    71,    73,    16,
     103,   105,   106,   103,    98,   103,   103,   103,   103,   103,
     103,   103,   105,    15,    15,    48,    75,    16,    41,    51,
      49,    72,    72,    76,   103,    16,    16,    56,    57,    64,
      77,    78,    79,    31,    32,    33,    34,    35,    36,    37,
      42,    43,    75,    80,    81,    82,    83,    84,    85,    86,
      89,    93,    95,    96,    97,   104,    78,   103,    38,    38,
      90,    87,    88,    15,   103,    61,    97,    49,    82,    38,
      40,    38,    38,    15,    15,    75,   100,   103,    38,    38,
     103,    92,    93,    94,    97,   103,   100,    35,    16,    38,
      41,    16,    15,    75,    91,   100,    93,    75,   100,    12,
      38,    16,    75,    92,    38,    16,    75
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    53,    54,    54,    54,    55,    55,    55,
      55,    55,    55,    56,    57,    57,    57,    57,    57,    57,
      58,    58,    59,    59,    60,    60,    60,    61,    62,    62,
      63,    64,    65,    65,    66,    66,    67,    67,    67,    67,
      68,    69,    69,    71,    70,    72,    72,    73,    73,    74,
      74,    76,    75,    77,    77,    78,    78,    79,    79,    80,
      80,    81,    81,    82,    82,    82,    82,    82,    82,    82,
      83,    83,    84,    85,    87,    86,    88,    86,    90,    89,
      91,    91,    92,    92,    92,    93,    94,    94,    95,    95,
      95,    96,    96,    96,    97,    97,    98,    98,    99,   100,
     101,   101,   101,   101,   101,   101,   102,   102,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   104,   105,   105,   106,   106
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     2,     2,     2,     0,     2,     2,     2,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     3,     1,     3,     6,     1,     2,     1,
       3,     4,     1,     3,     3,     4,     1,     1,     1,     1,
       6,     2,     1,     0,     7,     1,     0,     2,     3,     3,
       1,     0,     5,     1,     0,     2,     1,     1,     1,     1,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     5,     7,     0,     6,     0,     8,     0,    10,
       1,     0,     1,     1,     0,     3,     1,     3,     3,     2,
       2,     2,     3,     3,     1,     2,     2,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     2,     3,     3,     3,     2,     1,     1,
       1,     4,     1,     0,     1,     3
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
#line 1093 "parser.y" /* yacc.c:1646  */
    {
            //DEBUG//printf("Reduce to program\n");
            program_reduce_process();
            }
#line 2489 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 1097 "parser.y" /* yacc.c:1646  */
    {
            program_reduce_process();
	    }
#line 2497 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 1118 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2503 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 1119 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2509 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 1120 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2515 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 1121 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2521 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 1122 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2527 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 1123 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2533 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 1136 "parser.y" /* yacc.c:1646  */
    {
                // create basic
                create_basic_entry((yyvsp[0].text),K_VARIABLE);
          }
#line 2542 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 1140 "parser.y" /* yacc.c:1646  */
    {
                // create basic
                if((yyvsp[0].typeStruct).v_type >= 0){
                    //DEBUG//printf("Declared '%s' %s with expr type : %s\n",$1,TYPE_NAME[typeTmp.v_type],TYPE_NAME[$3.v_type]);
                    if(do_type_coercion(&typeTmp,&(yyvsp[0].typeStruct))){
                        //DEBUG//printf("==>Coercion pass!\n");
                        create_basic_entry((yyvsp[-2].text),K_VARIABLE);
                    }
                    else{
                        //DEBUG//printf("==>Coercion failure!\n");
                    }
                }
                else{
                    //DEBUG//printf("Previous type error!\n");   
                }
          }
#line 2563 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 1156 "parser.y" /* yacc.c:1646  */
    { 
                // check type list match
                if(check_array_initializer(&typeTmp,&(yyvsp[-1].typeList))){    
                    // create basic
                    create_basic_entry((yyvsp[-5].text),K_VARIABLE);
                }
          }
#line 2575 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 1168 "parser.y" /* yacc.c:1646  */
    { (yyval.text) = (yyvsp[0].text); }
#line 2581 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 1177 "parser.y" /* yacc.c:1646  */
    {
                        //DEBUG//printf("Reduce to single indice, dim = %d\n",$2.val);
                        tmp_type_add_dim((yyvsp[-1].typeStruct).val);
                        }
#line 2590 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 1192 "parser.y" /* yacc.c:1646  */
    {
                // normal positive ALSO NEED COERCION!!!!
                typeStruct_t type;
                memcpy(&type,&(yyvsp[0].typeStruct),sizeof(type));
                type.is_raw = false;
                // float generate
                if(typeTmp.v_type==T_FLOAT && type.v_type == T_DOUBLE){ 
                    type.v_type = T_FLOAT;
                    type.fval = (float)type.dval;
                }
                // try to coercion, convert type struct type recorded in typeTmp
                if(do_type_coercion(&typeTmp,&type)){
                    process_constant_declare((yyvsp[-2].text),type);
                }
             }
#line 2610 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 1207 "parser.y" /* yacc.c:1646  */
    {
                typeStruct_t type;
                memcpy(&type,&(yyvsp[0].typeStruct),sizeof(type));
                type.is_raw = false;
                // float generate
                if(typeTmp.v_type==T_FLOAT && type.v_type == T_DOUBLE){ 
                    type.v_type = T_FLOAT;
                    type.fval = (float)type.dval;
                }
                if(do_type_coercion(&typeTmp,&type)){ // try to coercion, convert type struct type recorded in typeTmp
                    bool success = true;
                    switch(typeTmp.v_type){
                        case T_INT:
                            type.val *= -1;
                            break;
                        case T_FLOAT:
                            type.fval *= -1;
                            break;
                        case T_DOUBLE:
                            type.dval *= -1;
                            break;
                        default:
                            // only numeric constant can use negation '-'
                            print_error("Only numeric constant can use negation '-'");
                            success = false;
                            break;
                    }
                    if(success){
                        process_constant_declare((yyvsp[-3].text),type);
                    }
                }
             }
#line 2647 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 1251 "parser.y" /* yacc.c:1646  */
    {
                    //push_table(TABLE_PRETEND);
                    //DEBUG//printf("Reduce  to funct_decl \n");
                    // add function declar entry
                    // add basic entry, check mode is function declare
                    init_funcOption();
                    funcOption.isDeclare = true;
                    funcOption.retType = (yyvsp[-5].type);
                    s_table_entry* ent = create_basic_entry((yyvsp[-4].text),K_FUNCTION);
                    if(ent){
                        // set type
                        ent->type.v_type = (yyvsp[-5].type);
                        // add attribute
                        ent->attr.param_list.end = 0;
                        for(int i=0;i<(yyvsp[-2].typeList).end;i++){
                            typeStruct_t* pTarget = &ent->attr.param_list.data[ent->attr.param_list.end];
                            ++ent->attr.param_list.end;
                            // type
                            pTarget->v_type = (yyvsp[-2].typeList).data[i].v_type;
                            // dim
                            pTarget->dim = (yyvsp[-2].typeList).data[i].dim;
                            // dims
                            memcpy(pTarget->dims,(yyvsp[-2].typeList).data[i].dims,sizeof(pTarget->dims));
                        }
                    }


                }
#line 2680 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 1291 "parser.y" /* yacc.c:1646  */
    {
                    // add function declar entry
                    // add basic entry, check mode is function declare
                    init_funcOption();
                    funcOption.isDeclare = false;
                    funcOption.pTypeList = &(yyvsp[-1].typeList);
                    for(int i=0;i<(yyvsp[-1].typeList).end;i++){
                        //DEBUG//printf("list[%d] %s , dim:%d\n",i,TYPE_NAME[$4.data[i].v_type],$4.data[i].dim);
                    }
                    funcOption.retType = (yyvsp[-4].type);
                    //DEBUG//printf("Reduce to funct_def , has type %s\n",TYPE_NAME[$1]); 
                    s_table_entry* ent = create_basic_entry((yyvsp[-3].text),K_FUNCTION);
                    // add pretend table after function entry added
                    // scope : see it is void or non-void
                    // if entry has an error, we will take current local type as real type
                    int effective_type;
                    if(!ent){ 
                        effective_type = (yyvsp[-4].type);    
                    }
                    else{
                        effective_type = ent->type.v_type;        
                    }
                    scopeTypeTmp = effective_type == T_VOID ? SCOPE_NORMAL : SCOPE_FUNCTION;
                    // if error occur, return type will adjust 
                    funcOption.retType = effective_type;
                    push_table(TABLE_PRETEND);
                    //DEBUG//printf("creating function entry for %s\n",$2);
                    if(ent || funcOption.paramAddOK){
                        // add attribute
                        if(ent){
                            ent->attr.param_list.end = 0;
                        }
                        for(int i=0;i<(yyvsp[-1].typeList).end;i++){
                            if((yyvsp[-1].typeList).data[i].v_type == T_ERROR){ // Error arg
                                continue;
                            }
                            if(!check_dimension(&(yyvsp[-1].typeList).data[i])){ // dimension check
                                continue;
                            }
                            if(ent){
                                typeStruct_t* pTarget = &ent->attr.param_list.data[ent->attr.param_list.end];
                                ++ent->attr.param_list.end;
                                // type
                                pTarget->v_type = (yyvsp[-1].typeList).data[i].v_type;
                                // dim
                                pTarget->dim = (yyvsp[-1].typeList).data[i].dim;
                                // dims
                                memcpy(pTarget->dims,(yyvsp[-1].typeList).data[i].dims,sizeof(pTarget->dims));
                            }
                            // create param type
                            s_table_entry* param = create_basic_entry((yyvsp[-1].typeList).data[i].sval,K_PARAMETER);
                            if (param){
                                //DEBUG//printf("==>Param (%s) added\n",KIND_NAME[param->kind]);
                                // new param added
                                // set type
                                param->type.v_type =  (yyvsp[-1].typeList).data[i].v_type;
                                // set dim
                                param->type.dim = (yyvsp[-1].typeList).data[i].dim;
                                // copy dims
                                memcpy(param->type.dims,(yyvsp[-1].typeList).data[i].dims,sizeof(param->type.dims));
                            }
                            else{
                            
                                //DEBUG//printf("==>Param add FAILURE\n");
                            }
                        } 

                    }
                    
                }
#line 2755 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 1361 "parser.y" /* yacc.c:1646  */
    {
                    // check last stmt
                    if(lastStmtType != STMT_COMPOUND_RETURN && (yyvsp[-6].type)!=T_VOID){
                        print_error("Last statment of non-void function must be a 'return' statment");
                    }
                }
#line 2766 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 1372 "parser.y" /* yacc.c:1646  */
    {     
            (yyval.typeList).end = 0;
         }
#line 2774 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 1377 "parser.y" /* yacc.c:1646  */
    {
                    //DEBUG//printf("Reduce to single_arg\n");
                    (yyval.typeStruct).v_type = typeTmp.v_type;
                    // copy dim info
                    (yyval.typeStruct).dim = typeTmp.dim;
                    memcpy(&(yyval.typeStruct).dims,&typeTmp.dims,sizeof((yyval.typeStruct).dims));
                    tmp_type_clear_array_indice();
                    (yyval.typeStruct).is_const = FALSE;
                    (yyval.typeStruct).sval = (yyvsp[0].text);
                }
#line 2789 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 1387 "parser.y" /* yacc.c:1646  */
    {
                    //DEBUG//printf("Reduce to const single_arg\n");
                    (yyval.typeStruct).v_type = typeTmp.v_type;
                    // copy dim info
                    (yyval.typeStruct).dim = typeTmp.dim;
                    memcpy(&(yyval.typeStruct).dims,&typeTmp.dims,sizeof((yyval.typeStruct).dims));
                    tmp_type_clear_array_indice();
                    (yyval.typeStruct).is_const = TRUE; 
                    (yyval.typeStruct).sval = (yyvsp[0].text);
                    }
#line 2804 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 1399 "parser.y" /* yacc.c:1646  */
    {
                //DEBUG//printf("Reduce single_arg,arg_list to arg_list\n");
                // copy old one
                memcpy(&(yyval.typeList),&(yyvsp[-2].typeList),sizeof((yyval.typeList)));
                // merge new one
                (yyval.typeList).data[(yyval.typeList).end].is_const = (yyvsp[0].typeStruct).is_const;
                (yyval.typeList).data[(yyval.typeList).end].v_type = (yyvsp[0].typeStruct).v_type;
                (yyval.typeList).data[(yyval.typeList).end].dim = (yyvsp[0].typeStruct).dim;
                memcpy(&(yyval.typeList).data[(yyval.typeList).end].dims,&(yyvsp[0].typeStruct).dims,sizeof((yyval.typeList).data[(yyval.typeList).end].dims));
                (yyval.typeList).data[(yyval.typeList).end].sval = (yyvsp[0].typeStruct).sval;
                (yyval.typeList).end += 1;
                // create parameter entry
                //create_basic_entry($1.sval,K_PARAMETER); 
                }
#line 2823 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1413 "parser.y" /* yacc.c:1646  */
    {
                //DEBUG//printf("Reduce single_arg to arg_list\n");
                (yyval.typeList).end = 1;
                (yyval.typeList).data[0].is_const = (yyvsp[0].typeStruct).is_const;
                (yyval.typeList).data[0].v_type = (yyvsp[0].typeStruct).v_type;
                (yyval.typeList).data[0].dim = (yyvsp[0].typeStruct).dim;
                memcpy(&(yyval.typeList).data[0].dims,&(yyvsp[0].typeStruct).dims,sizeof((yyval.typeList).data[0].dims));
                // name info
                (yyval.typeList).data[0].sval = (yyvsp[0].typeStruct).sval;
                // create parameter entry
                //create_basic_entry($1.sval,K_PARAMETER);
            }
#line 2840 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1428 "parser.y" /* yacc.c:1646  */
    {push_table(TABLE_NORMAL);}
#line 2846 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1428 "parser.y" /* yacc.c:1646  */
    {
                    //DEBUG//printf("Reduce to compund stat\n");
                    pop_table();
                    if(lastStmtType == STMT_RETURN){
                        //DEBUG//printf("===>Last stmt is RETURN\n");
                        lastStmtType = STMT_COMPOUND_RETURN;
                    }
                    else{
                        //DEBUG//printf("===>Last stmt is NOT RETURN\n");
                        lastStmtType = STMT_NORMAL;
                        
                    }     
                }
#line 2864 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1465 "parser.y" /* yacc.c:1646  */
    {lastStmtType = STMT_NORMAL;}
#line 2870 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1466 "parser.y" /* yacc.c:1646  */
    {lastStmtType = STMT_NORMAL;}
#line 2876 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1467 "parser.y" /* yacc.c:1646  */
    {lastStmtType = STMT_NORMAL;}
#line 2882 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1468 "parser.y" /* yacc.c:1646  */
    {lastStmtType = STMT_NORMAL;}
#line 2888 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1487 "parser.y" /* yacc.c:1646  */
    {go_to_loop_scope();}
#line 2894 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1488 "parser.y" /* yacc.c:1646  */
    {go_to_loop_scope();}
#line 2900 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1493 "parser.y" /* yacc.c:1646  */
    {go_to_loop_scope();}
#line 2906 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1506 "parser.y" /* yacc.c:1646  */
    {
                            check_variable_assign_able(&(yyvsp[-2].typeStruct),&(yyvsp[0].typeStruct));  
                        }
#line 2914 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1516 "parser.y" /* yacc.c:1646  */
    {process_return_function_check(&(yyvsp[-1].typeStruct));lastStmtType = STMT_RETURN;}
#line 2920 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1517 "parser.y" /* yacc.c:1646  */
    {process_loop_check(); lastStmtType = STMT_NORMAL;}
#line 2926 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1518 "parser.y" /* yacc.c:1646  */
    {process_loop_check();lastStmtType = STMT_NORMAL;}
#line 2932 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1523 "parser.y" /* yacc.c:1646  */
    {check_printable(&(yyvsp[-1].typeStruct));}
#line 2938 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1524 "parser.y" /* yacc.c:1646  */
    { check_readable(&(yyvsp[-1].typeStruct)); }
#line 2944 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1528 "parser.y" /* yacc.c:1646  */
    {
                (yyval.typeStruct).sval = (yyvsp[0].text); // name
                fill_type_for_name((typeStruct_t*)&(yyval.typeStruct),(yyvsp[0].text),DEFAULT_TYPE_FLAG);
                (yyval.typeStruct).is_raw = false;
            }
#line 2954 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1534 "parser.y" /* yacc.c:1646  */
    {
                (yyval.typeStruct).sval = (yyvsp[-1].text); // name
                fill_type_for_name((typeStruct_t*)&(yyval.typeStruct),(yyvsp[-1].text),DEFAULT_TYPE_FLAG);
                if((yyval.typeStruct).v_type != T_ERROR){
                    int dim = (yyval.typeStruct).dim;
                    int expand = 0;
                    //DEBUG//printf("-->prepare to expand array with dim : %d\n",$2.dim);
                    for(int i=0;i<(yyvsp[0].typeStruct).dim;i++){
                        if(dim > 0){ // can expand
                            //DEBUG//printf("==>expand info level %d of %d\n",i,$2.dim);
                            dim--;
                            expand++;
                        }
                        else{
                            //DEBUG//printf("==>error on expand level %d\n",i);
                            // cannot expand furthur
                            char errmsg[MAX_STRING_SIZE];
                            snprintf(errmsg,MAX_STRING_SIZE,"Cannot use array notation for non-array type '%s'",TYPE_NAME[(yyval.typeStruct).v_type]);
                            print_error(errmsg);
                            (yyval.typeStruct).v_type = T_ERROR; 
                            break;
                        }
                    }
                    // shift array if no error occur
                    if((yyval.typeStruct).v_type != T_ERROR){
                        for(int i=0;i<dim;i++){
                            (yyval.typeStruct).dims[i] = (yyval.typeStruct).dims[i+expand];
                        }
                        (yyval.typeStruct).dim = dim;
                    }
                }
                (yyval.typeStruct).is_const = false;
            }
#line 2992 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1570 "parser.y" /* yacc.c:1646  */
    {
                        //DEBUG//printf("int:Reduce singe_array to array\n");
                        // merge index
                        (yyval.typeStruct) = (yyvsp[0].typeStruct);
                        (yyval.typeStruct).dims[(yyval.typeStruct).dim] = (yyvsp[-1].typeStruct).dims[0];
                        (yyval.typeStruct).dim++;
                    }
#line 3004 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1578 "parser.y" /* yacc.c:1646  */
    {
                    //DEBUG//printf("int:Reduce singe to array\n");
                     
                }
#line 3013 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1584 "parser.y" /* yacc.c:1646  */
    {
                                (yyval.typeStruct).dim = 1;
                                //DEBUG//printf("int:Reduce to single indice\n");    
                                // check if expr is integer and not an array
                                if((yyvsp[-1].typeStruct).v_type != T_INT){
                                    (yyval.typeStruct).v_type = T_ERROR;
                                    if((yyvsp[-1].typeStruct).v_type != T_ERROR){
                                        // new error
                                        char errmsg[MAX_STRING_SIZE];
                                        snprintf(errmsg,MAX_STRING_SIZE,"Array index must be scalar integer type");
                                        print_error(errmsg);

                                    }
                                }
                                else{
                                    // furthur check if constant and the number < 0
                                    //DEBUG//printf("index is const : %s , val is %d\n",$2.is_const ? "true" : "false",$2.val);
                                    if((yyvsp[-1].typeStruct).is_const && (yyvsp[-1].typeStruct).val < 0){
                                        (yyval.typeStruct).v_type = T_ERROR;
                                        print_error("Array index must greater than or equal to 0");
                                            
                                    }
                                    else{
                                        (yyval.typeStruct).v_type = T_INT;
                                    }


                                }

                            }
#line 3048 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1619 "parser.y" /* yacc.c:1646  */
    {
                (yyval.typeStruct) = (yyvsp[0].typeStruct);
                // check type consist
                if((yyval.typeStruct).v_type != T_BOOL){
                    (yyval.typeStruct).v_type = T_ERROR;
                    if((yyvsp[0].typeStruct).v_type != T_ERROR){
                        // old one is not error
                        char errmsg[MAX_STRING_SIZE];
                        snprintf(errmsg,MAX_STRING_SIZE,"Expect a boolean expression here, but '%s' provided",TYPE_NAME[(yyvsp[0].typeStruct).v_type]);
                        print_error(errmsg);
                    }
                }
            }
#line 3066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1647 "parser.y" /* yacc.c:1646  */
    {
            (yyval.typeStruct).v_type = binary_numeric_expr_check((typeStruct_t*)&(yyvsp[-2].typeStruct),(typeStruct_t*)&(yyvsp[0].typeStruct),OP_ADD);
            (yyval.typeStruct).is_const = false;
        }
#line 3075 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1652 "parser.y" /* yacc.c:1646  */
    {
            (yyval.typeStruct).v_type = binary_numeric_expr_check((typeStruct_t*)&(yyvsp[-2].typeStruct),(typeStruct_t*)&(yyvsp[0].typeStruct),OP_SUB);
            (yyval.typeStruct).is_const = false;
        }
#line 3084 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1657 "parser.y" /* yacc.c:1646  */
    {
            (yyval.typeStruct).v_type = binary_numeric_expr_check((typeStruct_t*)&(yyvsp[-2].typeStruct),(typeStruct_t*)&(yyvsp[0].typeStruct),OP_MUL);
            (yyval.typeStruct).is_const = false;
        }
#line 3093 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1662 "parser.y" /* yacc.c:1646  */
    {
            (yyval.typeStruct).v_type = binary_numeric_expr_check((typeStruct_t*)&(yyvsp[-2].typeStruct),(typeStruct_t*)&(yyvsp[0].typeStruct),OP_DIV);
            (yyval.typeStruct).is_const = false;
        }
#line 3102 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1667 "parser.y" /* yacc.c:1646  */
    {
            (yyval.typeStruct).v_type = binary_numeric_expr_check((typeStruct_t*)&(yyvsp[-2].typeStruct),(typeStruct_t*)&(yyvsp[0].typeStruct),OP_MOD);
            (yyval.typeStruct).is_const = false;
        }
#line 3111 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1671 "parser.y" /* yacc.c:1646  */
    {
            //DEBUG//printf("Reduce with negation operator, original const is %s\n",$2.is_const ?  "true" : "false");
            (yyval.typeStruct) = (yyvsp[0].typeStruct);
            // check type is numeric or not
            if(!isNumericType((yyvsp[0].typeStruct).v_type)){
                (yyval.typeStruct).v_type = T_ERROR;
                if((yyvsp[0].typeStruct).v_type != T_ERROR){    
                    char errmsg[MAX_STRING_SIZE];
                    snprintf(errmsg,MAX_STRING_SIZE,"Only numeric expression can use negation '-', but '%s' provided.",TYPE_NAME[(yyvsp[0].typeStruct).v_type]);
                    print_error(errmsg);
                }
                else{
                    //DEBUG//printf("Previous error occur on operand for Negation Operator\n");
                }
            }
            if((yyval.typeStruct).is_const){
                perform_negation((typeStruct_t*)&(yyval.typeStruct));    
            }

        }
#line 3136 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1691 "parser.y" /* yacc.c:1646  */
    { (yyval.typeStruct) = (yyvsp[-1].typeStruct); }
#line 3142 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1692 "parser.y" /* yacc.c:1646  */
    {
            (yyval.typeStruct).v_type = binary_numeric_expr_check((typeStruct_t*)&(yyvsp[-2].typeStruct),(typeStruct_t*)&(yyvsp[0].typeStruct),OP_RELATION);
            (yyval.typeStruct).is_const = false;
        }
#line 3151 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1696 "parser.y" /* yacc.c:1646  */
    {
        // both side need to be boolean type
        if((yyvsp[-2].typeStruct).v_type != T_BOOL){ // p1 error
            (yyval.typeStruct).v_type = T_ERROR;     
            if((yyvsp[-2].typeStruct).v_type != T_ERROR){
                // if previous has no error, means this time has a convert error
                char errmsg[MAX_STRING_SIZE];
                snprintf(errmsg,MAX_STRING_SIZE,"Binary logical operator expect a boolean operand on left-hand-side, but '%s' provided.",TYPE_NAME[(yyvsp[-2].typeStruct).v_type]);
                print_error(errmsg);
            }
            else{
                //DEBUG//printf("Previous error occur on operand for Logical Operator\n");
            }
        }
        else if((yyvsp[0].typeStruct).v_type != T_BOOL){
            (yyval.typeStruct).v_type = T_ERROR;    
            if((yyvsp[0].typeStruct).v_type != T_ERROR){
                // if previous has no error, means this time has a convert error
                char errmsg[MAX_STRING_SIZE];
                snprintf(errmsg,MAX_STRING_SIZE,"Binary logical operator expect a boolean operand on right-hand-side, but '%s' provided.",TYPE_NAME[(yyvsp[0].typeStruct).v_type]);
                print_error(errmsg);
            }
            else{
                printf("Previous error occur on operand for Logical Operator\n");
            }
            
        }
        else{
             // success type, assign it 
            (yyval.typeStruct) = (yyvsp[-2].typeStruct);
        }
        (yyval.typeStruct).is_const = false;
     }
#line 3189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1729 "parser.y" /* yacc.c:1646  */
    {
        (yyval.typeStruct) = (yyvsp[0].typeStruct); // important, assign result 
        // check if type is not bool
        if((yyval.typeStruct).v_type != T_BOOL){
            if((yyval.typeStruct).v_type != T_ERROR){
                // if previous has no error, means this time has a convert error
                char errmsg[MAX_STRING_SIZE];
                snprintf(errmsg,MAX_STRING_SIZE,"Logical operator NOT:'!' expect a boolean operand, but '%s' provided.",TYPE_NAME[(yyval.typeStruct).v_type]);
                print_error(errmsg);
            }
            else{
                //DEBUG//printf("Previous error occur on operand for Logical NOT\n");
            }
            (yyval.typeStruct).v_type = T_ERROR;
        }
        (yyval.typeStruct).is_const = false;
     }
#line 3211 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1753 "parser.y" /* yacc.c:1646  */
    {
                // check function consistency
                int ret_type = check_function_call_consist((yyvsp[-3].text),&(yyvsp[-1].typeList));
                if(ret_type>=0){
                    //DEBUG//printf("-->function with return type '%s'called\n",TYPE_NAME[ret_type]);  
                }
                else{
                    //DEBUG//printf("Not found\n");    
                }
                // set typeStruct
                (yyval.typeStruct).sval = (yyvsp[-3].text);
                (yyval.typeStruct).v_type = ret_type;
                (yyval.typeStruct).is_const = false;

           }
#line 3231 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1772 "parser.y" /* yacc.c:1646  */
    {
                            // empty, build empty list
                            (yyval.typeList).end = 0;
                        }
#line 3240 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1780 "parser.y" /* yacc.c:1646  */
    {
                // build from single expr
                (yyval.typeList).end = 1;
                (yyval.typeList).data[0] = (yyvsp[0].typeStruct);
            }
#line 3250 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1786 "parser.y" /* yacc.c:1646  */
    {
                // copy list
                (yyval.typeList) = (yyvsp[-2].typeList);
                // concat list
                (yyval.typeList).data[(yyval.typeList).end] = (yyvsp[0].typeStruct);
                (yyval.typeList).end++;
            }
#line 3262 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3266 "y.tab.c" /* yacc.c:1646  */
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
#line 1796 "parser.y" /* yacc.c:1906  */


int clear(){

// clear pool
    for(int i=0;i<str_pool.end;i++){
        //printf("Pool:Clear[%d]='%s'\n",i,str_pool.data[i]);
        free(str_pool.data[i]);
    }
}

void init_funcOption(){
    funcOption.isDeclare = false;
    funcOption.pTypeList = NULL;
    funcOption.retType = T_ERROR;
    funcOption.paramAddOK = false;
}
void init_errorPool(){
    errorPool.end = 0;
}
void add_errorPool(const char* msg){
    if(errorPool.end<MAX_ERROR_MSG){
        errorPool.pool[errorPool.end] = msg;
        ++errorPool.end;
    }
    else{
        fprintf( stderr, "*** WARNING!!! Cannot add more error messages into pool !! ***\n" );
    }
}

// show all error, return if there is at least an error
bool show_errors(){
    if(errorPool.end==0){
        return false;
    }
#ifdef SHOW_ALL_ERROR
    printf("-------Dump all errors-------\n");
    for(int i=0;i<errorPool.end;i++){
        printf(errorPool.pool[i]);
    }
#endif
    return true;
}

// get current_table
symbol_table* current_table(){
    return &stk.tables[stk.end-1];
}
// get global_table
symbol_table* global_table(){
    return &stk.tables[0];
}

// push new table and return it
symbol_table* push_table(int mode){
    table_stack* p_table_stk = &stk;
    if(mode==TABLE_PRETEND){
        ++p_table_stk->pretend;
        //DEBUG//printf("New PRETEND Table Pushed, Level : %d\n",p_table_stk->level);    
    }
    else if(p_table_stk->pretend > 0){
        // normal mode, but cheated
        --p_table_stk->pretend;
        //DEBUG//printf("New FAKE Table Pushed, Level : %d\n",p_table_stk->tables[p_table_stk->end-1].level);    
        return &p_table_stk->tables[p_table_stk->end-1];
    }
    else{
        //DEBUG//printf("New NORMAL Table Pushed, Level : %d\n",p_table_stk->level);    
    }
    symbol_table* p_new = &p_table_stk->tables[p_table_stk->end];
    init_table(p_new,p_table_stk->level);
    //DEBUG//printf("New Table Scope : %d\n",p_new->scopeType);
    p_table_stk->end += 1;
    p_table_stk->level += 1;
    return p_new;
}

// pop table and return it
symbol_table* pop_table(){
    dump_current_table();
    table_stack* p_table_stk = &stk;
    symbol_table* old_table = &p_table_stk->tables[p_table_stk->end-1];
    //DEBUG//printf("Old Table Poped, Level : %d , scope %d -> %d\n",old_table->level,old_table->scopeType,p_table_stk->end > 1 ? p_table_stk->tables[p_table_stk->end-2].scopeType : -1);    
    p_table_stk->end -= 1;
    p_table_stk->level -= 1;
    return old_table;
}



// init a table stack
void init_table_stack(){
    stk.end = 0;
    stk.level = 0;
    stk.pretend = 0;
    scopeTypeTmp = SCOPE_NORMAL;
    push_table(TABLE_NORMAL);
}

int yyerror( char *msg )
{
  fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
  fprintf( stderr, "|--------------------------------------------------------------------------\n" );
  clear();
  exit(-1);
}

// function declare-definition test
// record struct
typedef struct{
    bool declare;
    bool define;
    const char* name;
} func_check_t;
void run_function_declare_definition_test(){
    func_check_t check[MAX_TABLE_ENTRY];
    int end = 0;
    // scan current table
    symbol_table* table = current_table(); 
    for(int i=0;i<table->end;i++){
        s_table_entry* ent = &table->entries[i]; 
        if(ent->kind==K_FUNCTION){
           if(ent->isDeclare){
                // single declare found, record it
                check[end].name = ent->name;
                check[end].declare = true;
                check[end].define = ent->isMatch;
                end++;
                //DEBUG//printf("decl : %s\n",ent->name);
           }
        }
    }
    char errmsg[MAX_STRING_SIZE];
    // check if there exist some function 
    for(int i=0;i<end;i++){
        if(!check[i].define){ // someone haven't defined yet
            snprintf(errmsg,MAX_STRING_SIZE,"Function %s has been declared but not be defined",check[i].name); 
            print_error(errmsg);
        }
    }
    
}

// final process for end of file
void process_check_eof(){
    // function declare-definition test
    run_function_declare_definition_test();
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
        // init
        KIND_NAME[1] = "function";
        KIND_NAME[2] = "parameter";
        KIND_NAME[4] = "variable";
        KIND_NAME[8] = "constant";
        init_id_table();
        str_pool.end = 0;
        init_table_stack(&stk);
        init_errorPool();

	yyin = fp;
	yyparse();
        
        if (Opt_Statistic)
        {
            show_freq_table();
            // Print frequency table
        }
        dispose_id_table();
        
        if(!show_errors()){           
            fprintf( stdout, "\n" );
            fprintf( stdout, "|---------------------------------------------|\n" );
            fprintf( stdout, "|  There is no syntactic and semantic error!  |\n" );
            fprintf( stdout, "|---------------------------------------------|\n" );
        }
        clear();

	exit(0);
}

