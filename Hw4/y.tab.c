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
const char* TYPE_ASM[] = {"I","F","D","Z","Ljava/lang/String;","V"};
const char* TYPE_ASM_PREFIX[] = {"i","f","d","i","ERROR","ERROR"};
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
int greaterType = T_ERROR; // store greater type if needed
int loopTypeTmp = LOOP_NONE;
int paramVarNum = 0;


// label mgmt
labelMgmt_t labelM;
// loop stack
stack_t loopStk;
// function call name stack
funcCallStk_t funcCallStk;

// targe file
FILE* asmOut;

// next Variable Number , set to 1 when entering an funcrion definition
int nextVarNum;




struct {
    int end;
    const char* pool[MAX_STRING_POOL];
} errorPool;

typedef struct {
    bool isDeclare;
    const char* name;
    bool isMain;
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
    p_table->loopType = loopTypeTmp;
    loopTypeTmp = LOOP_NONE;
    if(p_table->loopType != LOOP_NONE){
        // record loop label number
        p_table->loopLabelNumber = topLabel();
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
        // assembly generate for variables
        if(kind == K_VARIABLE ){
            if(current_table()->level == 0){
                // global variables
                //DEBUG//printf("New Global Variable %s\n",name);
                // gen assembly
                if(ent->type.v_type >= 0){
                    fprintf(asmOut,".field public static %s %s\n",name,TYPE_ASM[ent->type.v_type]);
                }
            }
            else{
                // local variables
                //DEBUG//printf("New Local Variable %s with var num %d\n",name,nextVarNum);
                ent->varNum = nextVarNum;
                // next starting var num , one more if current is double
                ++nextVarNum;
                if(ent->type.v_type == T_DOUBLE){
                    ++nextVarNum;
                }
            }
        }
        else if(kind == K_PARAMETER){
            // only need to fill var num
            ent->varNum = paramVarNum++;
        }
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
                /*
                else if(pType->v_type==T_DOUBLE && pType->is_const && pType->is_raw){
                    // raw float
                    if(!coercion_check_only){
                        pType->fval = (float)pType->dval;
                        pType->v_type = T_FLOAT;
                    }
                    result = true;
                }
                */
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
    bool old_supress = supressError;
    supressError = true;
    // search global table for function 
    s_table_entry* ent = findFuncEntry(name);
    if(ent){
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
                greaterType = extractGreaterType(lhs->v_type,rhs->v_type);
                ret_type = T_BOOL;
                break;
            case OP_MOD:
                ret_type = greaterType = T_INT;
                break;
            case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV:
                ret_type = greaterType = extractGreaterType(lhs->v_type,rhs->v_type);
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


void go_to_loop_scope(int loopType){
    //DEBUG//printf("Go into a loop scope\n");
    scopeTypeTmp = SCOPE_LOOP;
    loopTypeTmp = loopType;
}

// END_DECLARE

#line 1100 "y.tab.c" /* yacc.c:339  */

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
#line 1036 "parser.y" /* yacc.c:355  */

    int value;
    char* text;
    int type;
    typeStruct_t typeStruct;
    typeList_t typeList;

#line 1228 "y.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1243 "y.tab.c" /* yacc.c:358  */

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
#define YYNNTS  66
/* YYNRULES -- Number of rules.  */
#define YYNRULES  136
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  228

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
       0,  1111,  1111,  1115,  1120,  1121,  1122,  1125,  1126,  1127,
    1128,  1129,  1130,  1133,  1136,  1137,  1138,  1139,  1140,  1141,
    1145,  1146,  1150,  1151,  1154,  1158,  1181,  1193,  1199,  1200,
    1202,  1210,  1213,  1214,  1217,  1234,  1271,  1272,  1273,  1274,
    1280,  1313,  1314,  1320,  1320,  1438,  1439,  1444,  1454,  1466,
    1480,  1495,  1495,  1509,  1510,  1513,  1514,  1517,  1518,  1521,
    1522,  1525,  1526,  1531,  1532,  1533,  1534,  1535,  1536,  1537,
    1545,  1545,  1547,  1547,  1548,  1566,  1566,  1566,  1567,  1567,
    1567,  1567,  1572,  1572,  1572,  1572,  1572,  1572,  1575,  1576,
    1579,  1580,  1581,  1584,  1592,  1593,  1597,  1604,  1609,  1617,
    1618,  1618,  1623,  1623,  1630,  1636,  1672,  1680,  1686,  1721,
    1737,  1738,  1739,  1740,  1741,  1742,  1745,  1746,  1749,  1754,
    1758,  1762,  1766,  1770,  1792,  1793,  1796,  1831,  1852,  1856,
    1859,  1893,  1893,  1911,  1913,  1920,  1928
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
  "')'", "L_OP_OR", "L_OP_AND", "L_OP_NOT", "L_OP_LT", "L_OP_LE",
  "L_OP_EQ", "L_OP_GE", "L_OP_GT", "L_OP_NE", "'+'", "'-'", "'*'", "'/'",
  "'%'", "KW_RETURN", "KW_BREAK", "KW_CONTINUE", "KW_FOR", "KW_WHILE",
  "KW_DO", "KW_IF", "SEMICOLON", "ID", "ASSIGN", "COMMA", "KW_PRINT",
  "KW_READ", "INTEGER_CONSTANT", "FLOATING_CONSTANT", "BOOLEAN_CONSTANT",
  "STRING_CONSTANT", "'{'", "'}'", "'['", "']'", "$accept", "program",
  "decl_and_def_list", "declaration_list", "var_decl", "type",
  "identifier", "var_list", "var_entry", "scalar_id", "array_indice",
  "single_array_indice", "const_decl", "const_list", "single_const",
  "literal_constant", "funct_decl", "definition_list", "funct_def", "$@1",
  "args", "single_arg", "arg_list", "compound_stat", "$@2",
  "local_decl_list", "var_decl_list", "var_decl_entry",
  "zero_or_more_stat_list", "stat_list", "stat", "if_stat", "$@3",
  "else_part", "$@4", "while_stat", "$@5", "$@6", "$@7", "$@8", "$@9",
  "for_stat", "$@10", "$@11", "$@12", "$@13", "$@14", "for_bool_expr",
  "for_expr", "single_assginment", "one_or_more_for_assignment",
  "jump_stat", "simple_stat", "$@15", "$@16", "var_ref",
  "intexpr_array_indice", "intexpr_single_array_indice", "bool_expr",
  "relation_op", "binary_logic_op", "expr", "funct_call", "$@17",
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

#define YYPACT_NINF -155

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-155)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     180,  -155,  -155,  -155,  -155,  -155,   276,  -155,    14,   180,
    -155,   -21,  -155,  -155,  -155,   -21,  -155,  -155,  -155,  -155,
    -155,  -155,  -155,   -18,   -19,    31,   180,   -14,    -9,     2,
     180,  -155,   -21,   203,   112,     5,    -5,     7,  -155,  -155,
    -155,   133,  -155,   -21,  -155,   -35,   276,   -21,    44,  -155,
      32,   112,   112,   112,  -155,  -155,  -155,  -155,    -6,  -155,
    -155,   226,  -155,    27,    58,  -155,   -21,   -21,  -155,    54,
    -155,  -155,   -21,  -155,     7,    42,   203,   212,   237,  -155,
    -155,   112,  -155,    59,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,   112,   112,   112,   112,   112,   112,   112,  -155,
     112,    35,    93,  -155,  -155,  -155,  -155,    63,  -155,  -155,
     112,    66,  -155,    75,    75,  -155,  -155,  -155,   248,   237,
     226,    64,    71,   203,   203,  -155,  -155,    98,  -155,  -155,
     112,    99,   102,   180,  -155,   226,    42,  -155,  -155,   -21,
    -155,   183,  -155,   180,   112,    81,    82,  -155,  -155,  -155,
     107,  -155,  -155,  -155,    74,   183,  -155,  -155,  -155,  -155,
      92,  -155,  -155,    95,   100,  -155,   144,  -155,  -155,   118,
     122,    63,   112,   112,   -21,  -155,  -155,  -155,   112,  -155,
    -155,   112,   112,   105,   125,   226,   175,    59,   108,   226,
     109,  -155,   111,    95,   226,   127,   134,  -155,  -155,  -155,
    -155,   -21,  -155,  -155,    63,   112,  -155,    63,   112,   136,
     116,  -155,  -155,   139,  -155,  -155,  -155,  -155,    63,   112,
     137,  -155,  -155,  -155,   160,  -155,    63,  -155
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
      45,     0,     0,     0,    36,    37,    38,    39,   104,   128,
     130,    25,   129,     0,     0,    28,     0,     0,    41,     0,
      34,    33,     0,    47,    20,    43,     0,     0,   127,   123,
     131,     0,   105,   107,   117,   116,   110,   111,   115,   113,
     112,   114,     0,     0,     0,     0,     0,     0,     0,    30,
     134,    20,     0,    35,    48,    21,    40,     0,    49,   124,
     134,     0,   106,   118,   119,   120,   121,   122,   125,   126,
     135,     0,   133,    46,    46,    51,    44,     0,   108,    26,
       0,     0,     0,    54,   132,   136,     0,    43,    57,     0,
      58,    60,    53,    56,     0,     0,     0,    82,    75,    78,
       0,   100,   102,    63,     0,    59,    62,    65,    66,    67,
       0,    68,    64,     0,     0,    55,     0,    97,    98,     0,
       0,     0,     0,     0,     0,    52,    61,    99,     0,    69,
      96,    92,     0,     0,     0,   109,     0,   104,     0,    93,
       0,    94,    91,   130,    90,     0,     0,    70,   101,   103,
      83,     0,    76,    79,     0,    89,    95,     0,     0,    74,
       0,    88,    77,     0,    72,    71,    84,    80,     0,    92,
       0,    73,    85,    81,     0,    86,     0,    87
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -155,  -155,   161,   194,    -7,     1,   -31,   189,  -155,   -11,
     -26,  -155,    -1,   181,  -155,   -24,    -8,  -155,     3,  -155,
     -61,   147,  -155,   -97,  -155,  -155,    84,  -155,  -155,  -155,
      90,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,    65,  -143,
    -155,  -155,  -155,  -155,  -155,  -122,   204,  -155,  -154,  -155,
    -155,   -28,  -128,  -155,   176,  -155
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,    26,    38,    10,    11,    22,    23,    24,    58,
      36,    37,    12,    28,    29,    59,    13,    39,    40,   107,
      48,    49,    50,   153,   133,   141,   142,   143,   154,   155,
     156,   157,   204,   215,   218,   158,   170,   207,   171,   208,
     220,   159,   169,   205,   219,   224,   226,   210,   190,   160,
     192,   161,   162,   173,   174,    60,    82,    83,   184,    97,
      98,   185,    62,   110,   121,   122
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,    19,    17,    14,    27,    34,    61,    15,    18,    80,
     126,    65,    20,   164,    16,    35,    73,    70,    21,   163,
      31,    45,    32,    77,    78,    79,    41,   164,   195,    42,
      19,    17,    27,   163,    47,    64,    74,    18,   191,    66,
      67,   104,    68,    43,    81,   103,    33,    72,   105,    63,
     123,   211,   188,   111,   213,   101,   102,    35,   206,   193,
      75,    74,   131,   132,   113,   114,   115,   116,   117,   118,
     119,    34,   120,    76,   183,    34,   191,    47,    99,   163,
     106,    35,   120,    84,    85,    35,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,   193,    54,    55,
      56,    57,   135,    94,    95,    96,   100,   209,   124,    81,
     212,   125,   130,   129,   134,   136,   166,   128,   137,   167,
     168,   221,   172,   175,    47,    47,   138,    51,    45,   227,
     177,    52,   140,   181,   139,   178,   138,   182,   179,    53,
     196,   197,   140,   202,   139,   186,   199,   200,   214,   203,
     189,    21,   201,   194,   216,   217,    54,    55,    56,    57,
      69,    84,    85,   187,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,   223,   225,    54,    55,    56,
      57,    30,   180,     1,     2,     3,     4,     5,     6,     7,
     187,   194,    84,    85,     9,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,     1,     2,     3,     4,
       5,    46,     7,   198,   144,   145,   146,   147,   148,   149,
     150,    44,    21,   108,    71,   151,   152,   165,   109,    84,
      85,   125,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    84,    85,   176,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,     1,
       2,     3,     4,     5,   222,     7,   127,   112
};

static const yytype_uint8 yycheck[] =
{
      11,     9,     9,     0,    15,    40,    34,     6,     9,    15,
     107,    37,     9,   141,     0,    50,    47,    41,    39,   141,
      38,    32,    41,    51,    52,    53,    40,   155,   182,    38,
      38,    38,    43,   155,    33,    40,    47,    38,   181,    38,
      39,    72,    39,    41,    50,    69,    15,    46,    74,    44,
      15,   205,   174,    81,   208,    66,    67,    50,   201,   181,
      16,    72,   123,   124,    92,    93,    94,    95,    96,    97,
      98,    40,   100,    41,   171,    40,   219,    76,    51,   201,
      38,    50,   110,    17,    18,    50,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,   219,    44,    45,
      46,    47,   130,    28,    29,    30,    48,   204,    15,    50,
     207,    48,    41,    49,    16,    16,   144,    51,    16,    38,
      38,   218,    15,    49,   123,   124,   133,    15,   139,   226,
      38,    19,   133,    15,   133,    40,   143,    15,    38,    27,
      35,    16,   143,    16,   143,   173,    38,    38,    12,    15,
     178,    39,    41,   181,    38,    16,    44,    45,    46,    47,
      27,    17,    18,   174,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    38,    16,    44,    45,    46,
      47,    20,    38,     3,     4,     5,     6,     7,     8,     9,
     201,   219,    17,    18,     0,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,     3,     4,     5,     6,
       7,     8,     9,    38,    31,    32,    33,    34,    35,    36,
      37,    32,    39,    76,    43,    42,    43,   143,    16,    17,
      18,    48,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    17,    18,   155,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,     3,
       4,     5,     6,     7,   219,     9,   110,    83
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
     107,   113,   114,    44,    40,    62,    57,    57,    70,    27,
      67,    65,    57,    58,    61,    16,    41,   113,   113,   113,
      15,    50,   108,   109,    17,    18,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,   111,   112,    51,
      48,    61,    61,    67,    58,    62,    38,    71,    73,    16,
     115,   113,   108,   113,   113,   113,   113,   113,   113,   113,
     113,   116,   117,    15,    15,    48,    75,   116,    51,    49,
      41,    72,    72,    76,    16,   113,    16,    16,    56,    57,
      64,    77,    78,    79,    31,    32,    33,    34,    35,    36,
      37,    42,    43,    75,    80,    81,    82,    83,    87,    93,
     101,   103,   104,   107,   114,    78,   113,    38,    38,    94,
      88,    90,    15,   105,   106,    49,    82,    38,    40,    38,
      38,    15,    15,    75,   110,   113,   113,    61,   107,   113,
     100,   101,   102,   107,   113,   110,    35,    16,    38,    38,
      38,    41,    16,    15,    84,    95,   101,    89,    91,    75,
      99,   110,    75,   110,    12,    85,    38,    16,    86,    96,
      92,    75,   100,    38,    97,    16,    98,    75
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
      84,    83,    86,    85,    85,    88,    89,    87,    90,    91,
      92,    87,    94,    95,    96,    97,    98,    93,    99,    99,
     100,   100,   100,   101,   102,   102,   103,   103,   103,   104,
     105,   104,   106,   104,   107,   107,   108,   108,   109,   110,
     111,   111,   111,   111,   111,   111,   112,   112,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   115,   114,   116,   116,   117,   117
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
       0,     7,     0,     3,     0,     0,     0,     7,     0,     0,
       0,    10,     0,     0,     0,     0,     0,    14,     1,     0,
       1,     1,     0,     3,     1,     3,     3,     2,     2,     2,
       0,     4,     0,     4,     1,     2,     2,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     2,     3,     3,     3,     2,     1,     1,
       1,     0,     5,     1,     0,     1,     3
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
#line 1111 "parser.y" /* yacc.c:1646  */
    {
            //DEBUG//printf("Reduce to program\n");
            program_reduce_process();
            }
#line 2514 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 1115 "parser.y" /* yacc.c:1646  */
    {
            program_reduce_process();
	    }
#line 2522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 1136 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2528 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 1137 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2534 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 1138 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2540 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 1139 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2546 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 1140 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2552 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 1141 "parser.y" /* yacc.c:1646  */
    {tmp_type_init_scalar((yyvsp[0].type));(yyval.type)=(yyvsp[0].type);}
#line 2558 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 1154 "parser.y" /* yacc.c:1646  */
    {
                // create basic
                create_basic_entry((yyvsp[0].text),K_VARIABLE);
          }
#line 2567 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 1158 "parser.y" /* yacc.c:1646  */
    {
                // create basic
                bool old_check = coercion_check_only;
                coercion_check_only = true;
                if((yyvsp[0].typeStruct).v_type >= 0){
                    //DEBUG//printf("Declared '%s' %s with expr type : %s\n",$1,TYPE_NAME[typeTmp.v_type],TYPE_NAME[$3.v_type]);
                    if(do_type_coercion(&typeTmp,&(yyvsp[0].typeStruct))){
                        //DEBUG//printf("==>Coercion pass!\n");
                        create_basic_entry((yyvsp[-2].text),K_VARIABLE);
                        // assembly assignment , create fake and fill name
                        typeStruct_t fake = typeTmp;
                        fake.sval = (yyvsp[-2].text);
                        asmGenAssignment(&fake,&(yyvsp[0].typeStruct));
                    }
                    else{
                        //DEBUG//printf("==>Coercion failure!\n");
                    }
                }
                else{
                    //DEBUG//printf("Previous type error!\n");   
                }
                coercion_check_only = old_check;
          }
#line 2595 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 1181 "parser.y" /* yacc.c:1646  */
    { 
                // check type list match
                if(check_array_initializer(&typeTmp,&(yyvsp[-1].typeList))){    
                    // create basic
                    create_basic_entry((yyvsp[-5].text),K_VARIABLE);
                }
          }
#line 2607 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 1193 "parser.y" /* yacc.c:1646  */
    { (yyval.text) = (yyvsp[0].text); }
#line 2613 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 1202 "parser.y" /* yacc.c:1646  */
    {
                        //DEBUG//printf("Reduce to single indice, dim = %d\n",$2.val);
                        tmp_type_add_dim((yyvsp[-1].typeStruct).val);
                        }
#line 2622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 1217 "parser.y" /* yacc.c:1646  */
    {
                // normal positive ALSO NEED COERCION!!!!
                typeStruct_t type;
                memcpy(&type,&(yyvsp[0].typeStruct),sizeof(type));
                type.is_raw = false;
                /*
                // float generate
                if(typeTmp.v_type==T_FLOAT && type.v_type == T_DOUBLE){ 
                    type.v_type = T_FLOAT;
                    type.fval = (float)type.dval;
                }
                */
                // try to coercion, convert type struct type recorded in typeTmp
                if(do_type_coercion(&typeTmp,&type)){
                    process_constant_declare((yyvsp[-2].text),type);
                }
             }
#line 2644 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 1234 "parser.y" /* yacc.c:1646  */
    {
                typeStruct_t type;
                memcpy(&type,&(yyvsp[0].typeStruct),sizeof(type));
                type.is_raw = false;
                /*
                // float generate
                if(typeTmp.v_type==T_FLOAT && type.v_type == T_DOUBLE){ 
                    type.v_type = T_FLOAT;
                    type.fval = (float)type.dval;
                }
                */
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
#line 2683 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 1280 "parser.y" /* yacc.c:1646  */
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
#line 2716 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 1320 "parser.y" /* yacc.c:1646  */
    {
                    // clear variable number , for main, need to plus 1
                    nextVarNum = strcmp((yyvsp[-3].text),"main")==0 ? INIT_VAR_NUM + 1 : INIT_VAR_NUM ;
                    paramVarNum = nextVarNum;
                    
                    // add function declar entry
                    // add basic entry, check mode is function declare
                    init_funcOption();
                    funcOption.isDeclare = false;
                    funcOption.name = (yyvsp[-3].text);
                    funcOption.isMain = strcmp((yyvsp[-3].text),"main")==0;
                    typeList_t* pTypeList = &(yyvsp[-1].typeList);
                    funcOption.pTypeList = &(yyvsp[-1].typeList);
                    for(int i=0;i<pTypeList->end;i++){
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
                        // fill function entry attr
                        for(int i=0;i<pTypeList->end;i++){
                            if(pTypeList->data[i].v_type == T_ERROR){ // Error arg
                                continue;
                            }
                            if(!check_dimension(&pTypeList->data[i])){ // dimension check
                                continue;
                            }
                            if(ent){
                                typeStruct_t* pTarget = &ent->attr.param_list.data[ent->attr.param_list.end];
                                ++ent->attr.param_list.end;
                                // type
                                pTarget->v_type = pTypeList->data[i].v_type;
                                // dim
                                pTarget->dim = pTypeList->data[i].dim;
                                // dims
                                memcpy(pTarget->dims,pTypeList->data[i].dims,sizeof(pTarget->dims));
                            }
                            /*
                            nextVarNum++; // var num is for parameter
                            if(ent->type.v_type == T_DOUBLE){
                                nextVarNum++; // one more for double
                            }
                            */

                        } 
                        // genrate function head Assembly
                        asmGenFunctionHead((yyvsp[-3].text));
                        //DEBUG//printf("Function Head generated for %s\n",$2);
                        //create param entries
                        for(int i=0;i<pTypeList->end;i++){
                            if(pTypeList->data[i].v_type == T_ERROR){ // Error arg
                                continue;
                            }
                            if(!check_dimension(&pTypeList->data[i])){ // dimension check
                                continue;
                            }
                            // create param type
                            s_table_entry* param = create_basic_entry(pTypeList->data[i].sval,K_PARAMETER);
                            if (param){
                                //DEBUG//printf("==>Param (%s) added\n",KIND_NAME[param->kind]);
                                // new param added
                                // set type
                                param->type.v_type =  pTypeList->data[i].v_type;
                                // set dim
                                param->type.dim = pTypeList->data[i].dim;
                                // copy dims
                                memcpy(param->type.dims,pTypeList->data[i].dims,sizeof(param->type.dims));
                                // paramNum fix
                                if(param->type.v_type == T_DOUBLE){
                                    ++paramVarNum;
                                }

                            }
                            else{
                            
                                //DEBUG//printf("==>Param add FAILURE\n");
                            }
                        }
                    }
                    nextVarNum = paramVarNum;
                    
                }
#line 2827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 1426 "parser.y" /* yacc.c:1646  */
    {
                    // check last stmt
                    if(lastStmtType != STMT_COMPOUND_RETURN && (yyvsp[-6].type)!=T_VOID){
                        print_error("Last statment of non-void function must be a 'return' statment");
                    }
                    asmGenFunctionTail();
                    funcOption.retType = T_ERROR;
                }
#line 2840 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 1439 "parser.y" /* yacc.c:1646  */
    {     
            (yyval.typeList).end = 0;
         }
#line 2848 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 1444 "parser.y" /* yacc.c:1646  */
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
#line 2863 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 1454 "parser.y" /* yacc.c:1646  */
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
#line 2878 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 1466 "parser.y" /* yacc.c:1646  */
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
#line 2897 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1480 "parser.y" /* yacc.c:1646  */
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
#line 2914 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1495 "parser.y" /* yacc.c:1646  */
    {push_table(TABLE_NORMAL);}
#line 2920 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1495 "parser.y" /* yacc.c:1646  */
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
#line 2938 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1532 "parser.y" /* yacc.c:1646  */
    {lastStmtType = STMT_NORMAL;}
#line 2944 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1533 "parser.y" /* yacc.c:1646  */
    {lastStmtType = STMT_NORMAL;}
#line 2950 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1534 "parser.y" /* yacc.c:1646  */
    {lastStmtType = STMT_NORMAL;}
#line 2956 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1535 "parser.y" /* yacc.c:1646  */
    {lastStmtType = STMT_NORMAL;}
#line 2962 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1537 "parser.y" /* yacc.c:1646  */
    { 
        asmGenFunctionCall((yyvsp[-1].typeStruct).sval,true); // discard return value if any
     }
#line 2970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1545 "parser.y" /* yacc.c:1646  */
    {pushLabel();asmGenIfHead();}
#line 2976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1547 "parser.y" /* yacc.c:1646  */
    {asmGenIfGoExit();asmGenIfFalse();}
#line 2982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1547 "parser.y" /* yacc.c:1646  */
    {asmGenIfExit();popLabel();}
#line 2988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1548 "parser.y" /* yacc.c:1646  */
    {asmGenIfFalse();popLabel();}
#line 2994 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1566 "parser.y" /* yacc.c:1646  */
    {go_to_loop_scope(LOOP_WHILE);pushLabel();asmGenWhileHead();}
#line 3000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1566 "parser.y" /* yacc.c:1646  */
    {asmGenWhileGoExit();}
#line 3006 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1566 "parser.y" /* yacc.c:1646  */
    {asmGenWhileTail();popLabel();}
#line 3012 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1567 "parser.y" /* yacc.c:1646  */
    {go_to_loop_scope(LOOP_DO_WHILE);pushLabel();asmGenDoWhileHead();}
#line 3018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1567 "parser.y" /* yacc.c:1646  */
    {asmGenDoWhileTest();}
#line 3024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1567 "parser.y" /* yacc.c:1646  */
    {asmGenDoWhileTail();popLabel();}
#line 3030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1572 "parser.y" /* yacc.c:1646  */
    {go_to_loop_scope(LOOP_FOR);}
#line 3036 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1572 "parser.y" /* yacc.c:1646  */
    {pushLabel();asmGenForBegin();}
#line 3042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1572 "parser.y" /* yacc.c:1646  */
    {asmGenForGotoTrue();asmGenForIncrease();}
#line 3048 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1572 "parser.y" /* yacc.c:1646  */
    {asmGenForGotoBegin();}
#line 3054 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1572 "parser.y" /* yacc.c:1646  */
    {asmGenForTrue();}
#line 3060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1572 "parser.y" /* yacc.c:1646  */
    {asmGenForTail();popLabel();}
#line 3066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1585 "parser.y" /* yacc.c:1646  */
    {
                            asmGenAssignment(&(yyvsp[-2].typeStruct),&(yyvsp[0].typeStruct));


                        }
#line 3076 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1597 "parser.y" /* yacc.c:1646  */
    {
                    process_return_function_check(&(yyvsp[-1].typeStruct));
                    lastStmtType = STMT_RETURN;
                    if(!funcOption.isMain){
                        asmGenFunctionReturn(funcOption.retType,(yyvsp[-1].typeStruct).v_type); 
                    }
                }
#line 3088 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1604 "parser.y" /* yacc.c:1646  */
    {
                    process_loop_check(); 
                    lastStmtType = STMT_NORMAL;
                    asmGenLoopControl(LOOP_CTRL_BREAK);
                }
#line 3098 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1609 "parser.y" /* yacc.c:1646  */
    {
                    process_loop_check();
                    lastStmtType = STMT_NORMAL;
                    asmGenLoopControl(LOOP_CTRL_CONTINUE);
                }
#line 3108 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1618 "parser.y" /* yacc.c:1646  */
    {asmGenPrintHead();}
#line 3114 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1618 "parser.y" /* yacc.c:1646  */
    {
                    check_printable(&(yyvsp[-1].typeStruct));
                    asmGenPrintTail(&(yyvsp[-1].typeStruct));
                    
                }
#line 3124 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1623 "parser.y" /* yacc.c:1646  */
    {asmGenReadHead();}
#line 3130 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1623 "parser.y" /* yacc.c:1646  */
    { 
                    check_readable(&(yyvsp[-1].typeStruct)); 
                    asmGenReadTail(&(yyvsp[-1].typeStruct));
                    
                }
#line 3140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1631 "parser.y" /* yacc.c:1646  */
    {
                (yyval.typeStruct).sval = (yyvsp[0].text); // name
                fill_type_for_name((typeStruct_t*)&(yyval.typeStruct),(yyvsp[0].text),DEFAULT_TYPE_FLAG);
                (yyval.typeStruct).is_raw = false;
            }
#line 3150 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1637 "parser.y" /* yacc.c:1646  */
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
#line 3188 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1673 "parser.y" /* yacc.c:1646  */
    {
                        //DEBUG//printf("int:Reduce singe_array to array\n");
                        // merge index
                        (yyval.typeStruct) = (yyvsp[0].typeStruct);
                        (yyval.typeStruct).dims[(yyval.typeStruct).dim] = (yyvsp[-1].typeStruct).dims[0];
                        (yyval.typeStruct).dim++;
                    }
#line 3200 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1681 "parser.y" /* yacc.c:1646  */
    {
                    //DEBUG//printf("int:Reduce singe to array\n");
                     
                }
#line 3209 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1687 "parser.y" /* yacc.c:1646  */
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
#line 3244 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1722 "parser.y" /* yacc.c:1646  */
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
#line 3262 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1750 "parser.y" /* yacc.c:1646  */
    {
            asmGenBinaryExpression(&(yyval.typeStruct),&(yyvsp[-2].typeStruct),&(yyvsp[0].typeStruct),OP_ADD);
        }
#line 3270 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1755 "parser.y" /* yacc.c:1646  */
    {
            asmGenBinaryExpression(&(yyval.typeStruct),&(yyvsp[-2].typeStruct),&(yyvsp[0].typeStruct),OP_SUB);
        }
#line 3278 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1759 "parser.y" /* yacc.c:1646  */
    {
            asmGenBinaryExpression(&(yyval.typeStruct),&(yyvsp[-2].typeStruct),&(yyvsp[0].typeStruct),OP_MUL);
        }
#line 3286 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1763 "parser.y" /* yacc.c:1646  */
    {
            asmGenBinaryExpression(&(yyval.typeStruct),&(yyvsp[-2].typeStruct),&(yyvsp[0].typeStruct),OP_DIV);
        }
#line 3294 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1767 "parser.y" /* yacc.c:1646  */
    {
            asmGenBinaryExpression(&(yyval.typeStruct),&(yyvsp[-2].typeStruct),&(yyvsp[0].typeStruct),OP_MOD);
        }
#line 3302 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1770 "parser.y" /* yacc.c:1646  */
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
            // make negation assembly
            asmGenOperator(&(yyval.typeStruct),OP_NEG);
    
        }
#line 3329 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1792 "parser.y" /* yacc.c:1646  */
    { (yyval.typeStruct) = (yyvsp[-1].typeStruct); }
#line 3335 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1793 "parser.y" /* yacc.c:1646  */
    {
            asmGenBinaryExpression(&(yyval.typeStruct),&(yyvsp[-2].typeStruct),&(yyvsp[0].typeStruct),(yyvsp[-1].value));
        }
#line 3343 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1796 "parser.y" /* yacc.c:1646  */
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
            // this time, no type coercion needed
            asmGenOperator(&(yyval.typeStruct),(yyvsp[-1].value));
        }
        (yyval.typeStruct).is_const = false;
     }
#line 3383 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1831 "parser.y" /* yacc.c:1646  */
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
        // assembly generate, NOT is equal to "XOR with true"
        // push a true
        fprintf(asmOut,"    iconst_1\n");
        asmGenOperator(&(yyval.typeStruct),(yyvsp[-1].value));
     }
#line 3409 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1852 "parser.y" /* yacc.c:1646  */
    {
        // load the value to the top of stack
        asmGenLiteral(&(yyvsp[0].typeStruct));
     }
#line 3418 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1856 "parser.y" /* yacc.c:1646  */
    {
        asmGenFunctionCall((yyvsp[0].typeStruct).sval,false);
     }
#line 3426 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1859 "parser.y" /* yacc.c:1646  */
    {
        // the operand stack will load this variable
        //DEBUG//printf("Reference to Variable %s\n",$1.sval);
        s_table_entry* ent = findVarEntry((yyvsp[0].typeStruct).sval);
        if(ent && ent->type.v_type != T_ERROR){
            if(ent->kind == K_CONSTANT){
                // constant , load direct value    
                typeStruct_t fake = ent->type;
                fillTypeStructWithSymAttr(&fake,&ent->attr);
                asmGenLiteral(&fake);
            }
            else{
                // load it value
                if(ent->level > 0){
                    // local  
                    // retrive var num
                    //DEBUG//printf("-->Local, Var num:%d\n",ent->varNum);
                    // type prefix
                    const char* prefix = TYPE_ASM_PREFIX[ent->type.v_type];
                    fprintf(asmOut,"    %sload %d\n",prefix,ent->varNum);
                }
                else{
                    // global   
                    if(ent->type.v_type >= 0){
                        fprintf(asmOut,"    getstatic %s/%s %s\n",OUT_NAME,ent->name,TYPE_ASM[ent->type.v_type]);
                    }
                }
            }
        }
     }
#line 3461 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1893 "parser.y" /* yacc.c:1646  */
    {pushFuncCall((yyvsp[-1].text));}
#line 3467 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1893 "parser.y" /* yacc.c:1646  */
    {
                popFuncCall();
                // check function consistency
                int ret_type = check_function_call_consist((yyvsp[-4].text),&(yyvsp[-1].typeList));
                if(ret_type>=0){
                    //DEBUG//printf("-->function with return type '%s'called\n",TYPE_NAME[ret_type]);  
                }
                else{
                    //DEBUG//printf("Not found\n");    
                }
                // set typeStruct
                (yyval.typeStruct).sval = (yyvsp[-4].text);
                (yyval.typeStruct).v_type = ret_type;
                (yyval.typeStruct).is_const = false;

           }
#line 3488 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1913 "parser.y" /* yacc.c:1646  */
    {
                            // empty, build empty list
                            (yyval.typeList).end = 0;
                        }
#line 3497 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1921 "parser.y" /* yacc.c:1646  */
    {
                // build from single expr
                (yyval.typeList).end = 1;
                (yyval.typeList).data[0] = (yyvsp[0].typeStruct);
                // match expr coercion
                asmGenFunctionCallCoercion(&(yyvsp[0].typeStruct));
            }
#line 3509 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 1929 "parser.y" /* yacc.c:1646  */
    {
                // copy list
                (yyval.typeList) = (yyvsp[-2].typeList);
                // concat list
                (yyval.typeList).data[(yyval.typeList).end] = (yyvsp[0].typeStruct);
                (yyval.typeList).end++;
                // match expr coercion
                asmGenFunctionCallCoercion(&(yyvsp[0].typeStruct));
            }
#line 3523 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3527 "y.tab.c" /* yacc.c:1646  */
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
#line 1941 "parser.y" /* yacc.c:1906  */


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
    funcOption.isMain = false;
    funcOption.name = NULL;
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


void initFuncCallStk(){
    funcCallStk.end = 0;
}
void pushFuncCall(const char* name){
    s_table_entry* ent = findFuncEntry(name);
    if(ent){
        funcCall_t* ptr = &funcCallStk.data[funcCallStk.end++];
        ptr->index = 0;
        memcpy(&ptr->list,&ent->attr.param_list,sizeof(ptr->list));
        //DEBUG//printf("new function call %s with end : %d\n",name,ptr->list.end);
    }
}
void popFuncCall(){
    --funcCallStk.end;
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

s_table_entry* findFuncEntry(const char* name){ 
    // search global table for function 
    symbol_table* pTable = global_table();
    for(int i=0;i<pTable->end;i++){
        s_table_entry* ent = &pTable->entries[i];
        // function kind only
        if(ent->kind == K_FUNCTION){
            // same name
            if(strcmp(name,ent->name)==0){
                return ent;
            }
        }
    }
    return NULL;
}

void asmGenFunctionCallCoercion(typeStruct_t* pType){
    // only when function call stack is not empty
    if(funcCallStk.end <= 0){
        return;
    }
    // try to match current top
    funcCall_t* pCall = &funcCallStk.data[funcCallStk.end-1];
    // check out bound
    //DEBUG//printf("Index : %d , End : %d\n",pCall->index,pCall->list.end);
    if(pCall->index >= pCall->list.end){
        return;
    }
    //DEBUG//printf("Generate function call coercion2\n");
    typeStruct_t* pTarget = &pCall->list.data[pCall->index++];
    int dstType = pTarget->v_type;
    int srcType = pType->v_type;
    asmGenCoercion(dstType,srcType);
}

void asmGenScanner(){ 
    fprintf(asmOut,"    new java/util/Scanner\n");
    fprintf(asmOut,"    dup\n");
    fprintf(asmOut,"    getstatic java/lang/System/in Ljava/io/InputStream;\n");
    fprintf(asmOut,"    invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V\n");
    fprintf(asmOut,"    putstatic %s/_sc Ljava/util/Scanner;\n",OUT_NAME);

}

void asmGenProgramHead(){
    // Basic Header
    fprintf(asmOut,".class public %s\n",OUT_NAME);    
    fprintf(asmOut,".super java/lang/Object\n");
    // hidden field for Scanner
    fprintf(asmOut,".field public static _sc Ljava/util/Scanner;\n");

}

s_table_entry* findEntryByName(const char* name,EntryChecker checker){

    // search each table
    for(int i = stk.end-1;i>=0;i-- ){
        symbol_table* pTable = &stk.tables[i];
        for(int j = pTable->end-1;j>=0;j--){
            s_table_entry* ent = &pTable->entries[j];
            if(strcmp(name,ent->name)==0&&(!checker || checker(ent))){
                return ent; 
            }
        }
    }

    return NULL;
}

bool entryIsVar(s_table_entry* ent){
    return (ent && (ent->kind == K_VARIABLE || ent->kind == K_PARAMETER || ent->kind == K_CONSTANT ));
}
bool entryIsFunction(s_table_entry* ent){
    return (ent && (ent->kind == K_FUNCTION ));
}

s_table_entry* findVarEntry(const char* name){ 
    return findEntryByName(name,entryIsVar);
}
bool checkTypeStructSanity(typeStruct_t* pType){
    return pType && pType->v_type != T_ERROR;
}

void fillTypeStructWithSymAttr(typeStruct_t* pType,sym_attr* pAttr){
    switch(pType->v_type){
    case T_INT:
        pType->val = pAttr->int_val;
        break;
    case T_BOOL:
        pType->val = pAttr->bool_val;
        break;
    case T_FLOAT:
        pType->fval = pAttr->float_val;
        break;
    case T_DOUBLE:
        pType->dval = pAttr->double_val;
        break;
    case T_STRING:
        pType->sval = pAttr->string_val;
        break;
    }
}
void initLabelMgmt(labelMgmt_t* pMgmt){
    pMgmt->count = 0;
    pMgmt->stk.end = 0;
}
// push label
void pushLabel(){
    labelM.stk.data[labelM.stk.end++] = labelM.count++; // stack top is usable! 
}
// pop label
int popLabel(){
    return labelM.stk.data[--labelM.stk.end];
}
// get top label
int topLabel(){
    return labelM.stk.data[labelM.stk.end-1];
}
void asmGenLoadStackToLocal(typeStruct_t* pType,int varNum){
    if(checkTypeStructSanity(pType)){
        fprintf(asmOut,"    %sstore %d\n",TYPE_ASM_PREFIX[pType->v_type],varNum); 
    }
}
void asmGenStoreLocalToStack(typeStruct_t* pType,int varNum){
    if(checkTypeStructSanity(pType)){
        fprintf(asmOut,"    %sload %d\n",TYPE_ASM_PREFIX[pType->v_type],varNum); 
    }
}
void asmGenOperator(typeStruct_t* pType,int op){
    if(pType && pType->v_type != T_ERROR){
        const char* opStr = "ERROR";
        switch(op){
        case OP_ADD:
            opStr = "add";
            break;
        case OP_SUB:
            opStr = "sub";
            break;
        case OP_MUL:
            opStr = "mul";
            break;
        case OP_DIV:
            opStr = "div";
            break;
        case OP_MOD:
            opStr = "rem";
            break;
        case OP_NEG:
            opStr = "neg";
            break;
        case OP_AND:
            opStr = "and";
            break;
        case OP_OR:
            opStr = "or";
            break;
        case OP_NOT:
            opStr = "xor";
            break;   
        }
        fprintf(asmOut,"    %s%s\n",TYPE_ASM_PREFIX[pType->v_type],opStr);
    }
}
void asmGenRelationalOperator(int cmpType,int op){
    //create labels
    char labelT[MAX_STRING_SIZE];
    char labelF[MAX_STRING_SIZE];
    snprintf(labelT,MAX_STRING_SIZE,"LABEL_BOOLEXPR_TRUE_%d",labelM.count);
    snprintf(labelF,MAX_STRING_SIZE,"LABEL_BOOLEXPR_FALSE_%d",labelM.count);
    ++labelM.count;
    // according to type generate compare command
    switch(cmpType){
    case T_INT:
        fprintf(asmOut,"    isub\n");
        break;
    case T_FLOAT: case T_DOUBLE:
        fprintf(asmOut,"    %scmpl\n",TYPE_ASM_PREFIX[cmpType]);
        break;
    }
    // according to op, generate comparison half-command
    switch(op){
    case OP_GT:
        fprintf(asmOut,"    ifgt ");
        break;
    case OP_GE:
        fprintf(asmOut,"    ifge ");
        break;
    case OP_EQ:
        fprintf(asmOut,"    ifeq ");
        break;
    case OP_NE:
        fprintf(asmOut,"    ifne ");
        break;
    case OP_LT:
        fprintf(asmOut,"    iflt ");
        break;
    case OP_LE:
        fprintf(asmOut,"    ifle ");
        break;
    }
    // attach true label
    fprintf(asmOut,"%s\n",labelT);
    // false assignment
    fprintf(asmOut,"    iconst_0 ; false = 0\n");
    // goto exit label
    fprintf(asmOut,"    goto %s\n",labelF);
    // label : true
    fprintf(asmOut,"%s:\n",labelT);
    // true assignment
    fprintf(asmOut,"    iconst_1 ; true = 1\n");
    // false label
    fprintf(asmOut,"%s:\n",labelF);
    



}


void asmGenIfHead(){
    int topL = topLabel();
    // create false label
    char labelF[MAX_STRING_SIZE];
    snprintf(labelF,MAX_STRING_SIZE,"LABEL_IF_FALSE_%d",topL);
    // generate if equal than jump code
    fprintf(asmOut,"    ifeq %s\n",labelF);
}

// code for goto exit IF
void asmGenIfGoExit(){
    // create exit label
    char label[MAX_STRING_SIZE];
    snprintf(label,MAX_STRING_SIZE,"LABEL_IF_EXIT_%d",topLabel());
    // generate if equal than jump code
    fprintf(asmOut,"    goto %s\n",label);
    
}
// code for generate label false location
void asmGenIfFalse(){
    // create exit label
    char label[MAX_STRING_SIZE];
    snprintf(label,MAX_STRING_SIZE,"LABEL_IF_FALSE_%d",topLabel());
    // generate if equal than jump code
    fprintf(asmOut,"%s:\n",label);
}
// code for generate label exit location
void asmGenIfExit(){
    // create exit label
    char label[MAX_STRING_SIZE];
    snprintf(label,MAX_STRING_SIZE,"LABEL_IF_EXIT_%d",topLabel());
    // generate if equal than jump code
    fprintf(asmOut,"%s:\n",label);
}

void asmGenDoWhileHead(){
    asmGenWhileHead();    
}

void asmGenDoWhileTail(){
    // labels : begin
    int topL = topLabel();
    char labelB[MAX_STRING_SIZE];
    snprintf(labelB,MAX_STRING_SIZE,"LABEL_WHILE_BEGIN_%d",topL);
    // check , if not equal to zero, jump
    fprintf(asmOut,"    ifne %s\n",labelB);
    // exit label
    asmGenWhileExit();
}
void asmGenDoWhileTest(){
    int topL = topLabel();
    char labelT[MAX_STRING_SIZE];
    snprintf(labelT,MAX_STRING_SIZE,"LABEL_WHILE_TEST_%d",topL);
    // emit goto exit
    fprintf(asmOut,"%s:\n",labelT);
        
} 
void asmGenWhileHead(){
    // labels : begin
    int topL = topLabel();
    char labelB[MAX_STRING_SIZE];
    snprintf(labelB,MAX_STRING_SIZE,"LABEL_WHILE_BEGIN_%d",topL);
    // emit begin label
    fprintf(asmOut,"%s:\n",labelB);
}
// code for goto exit WHILE
void asmGenWhileGoExit(){
    // labels : exit
    int topL = topLabel();
    char labelE[MAX_STRING_SIZE];
    snprintf(labelE,MAX_STRING_SIZE,"LABEL_WHILE_EXIT_%d",topL);
    // if top equal to zero , goto exit
    fprintf(asmOut,"    ifeq %s\n",labelE);
    
}

void asmGenWhileGotoBegin(){
    // append goto begin
    // labels : begin
    int topL = topLabel();
    char labelB[MAX_STRING_SIZE];
    snprintf(labelB,MAX_STRING_SIZE,"LABEL_WHILE_BEGIN_%d",topL);
    // emit goto begin
    fprintf(asmOut,"    goto %s\n",labelB);
    
}

void asmGenWhileTail(){
    // goto begin
    asmGenWhileGotoBegin();
    // exit label
    asmGenWhileExit();
}

// code for generate label exit location
void asmGenWhileExit(){
    // labels : exit
    int topL = topLabel();
    char labelE[MAX_STRING_SIZE];
    snprintf(labelE,MAX_STRING_SIZE,"LABEL_WHILE_EXIT_%d",topL);
    // emit exit label
    fprintf(asmOut,"%s:\n",labelE);   
}

// begin of bool expr, begin label
void asmGenForBegin(){ 
    // labels : begin
    int topL = topLabel();
    char labelB[MAX_STRING_SIZE];
    snprintf(labelB,MAX_STRING_SIZE,"LABEL_FOR_BEGIN_%d",topL);
    // emit begin
    fprintf(asmOut,"%s:\n",labelB);
}
// end of increase , goto begin
void asmGenForGotoBegin(){
    // labels : begin
    int topL = topLabel();
    char labelB[MAX_STRING_SIZE];
    snprintf(labelB,MAX_STRING_SIZE,"LABEL_FOR_BEGIN_%d",topL);
    // emit goto begin
    fprintf(asmOut,"    goto %s\n",labelB);
}
// end of bool expr , goto true or goto exit
void asmGenForGotoTrue(){
    // labels : true , exit
    int topL = topLabel();
    char labelT[MAX_STRING_SIZE];
    char labelE[MAX_STRING_SIZE];
    snprintf(labelT,MAX_STRING_SIZE,"LABEL_FOR_TRUE_%d",topL);
    snprintf(labelE,MAX_STRING_SIZE,"LABEL_FOR_EXIT_%d",topL);
    // if not equal zero, goto true
    fprintf(asmOut,"    ifne %s\n",labelT);
    // otherwise, goto exit
    fprintf(asmOut,"    goto %s\n",labelE);
}
// true label
void asmGenForTrue(){
    // labels : true 
    int topL = topLabel();
    char labelT[MAX_STRING_SIZE];
    snprintf(labelT,MAX_STRING_SIZE,"LABEL_FOR_TRUE_%d",topL);
    // true label
    fprintf(asmOut,"%s:\n",labelT);
    
}
// begin of increase label
void asmGenForIncrease(){
    // labels : increase
    int topL = topLabel();
    char labelI[MAX_STRING_SIZE];
    snprintf(labelI,MAX_STRING_SIZE,"LABEL_FOR_INCREASE_%d",topL);
    // emit increase
    fprintf(asmOut,"%s:\n",labelI);
    
}

// end of statments, goto increase ; build end label 
void asmGenForTail(){
    // labels : increase , exit
    int topL = topLabel();
    char labelI[MAX_STRING_SIZE];
    char labelE[MAX_STRING_SIZE];
    snprintf(labelI,MAX_STRING_SIZE,"LABEL_FOR_INCREASE_%d",topL);
    snprintf(labelE,MAX_STRING_SIZE,"LABEL_FOR_EXIT_%d",topL);
    // emit goti increase
    fprintf(asmOut,"    goto %s\n",labelI);
    // build exit label
    fprintf(asmOut,"%s:\n",labelE);
}

void asmGenLoopControl(int ctrl){
    // search nearest loop table
    symbol_table* pTable = NULL;
    for(int i = stk.end-1;i>=0;i-- ){
        symbol_table* pFind = &stk.tables[i];
        if(pFind->scopeType == SCOPE_LOOP){
            pTable = pFind;
            break;
        }
    }
    if(pTable){
        switch(pTable->loopType){
        case LOOP_WHILE:
            if(ctrl == LOOP_CTRL_BREAK){
                asmGenWhileBreak(pTable);
            }
            else{
                asmGenWhileContinue(pTable);
            }
            break;
        case LOOP_DO_WHILE:
            if(ctrl == LOOP_CTRL_BREAK){
                asmGenDoWhileBreak(pTable);
            }
            else{
                asmGenDoWhileContinue(pTable);
            }
            break;
        case LOOP_FOR:
            if(ctrl == LOOP_CTRL_BREAK){
                asmGenForBreak(pTable);
            }
            else{
                asmGenForContinue(pTable); 
            }
            break;
        } 
    }
}
// code for break in loop table
void asmGenWhileBreak(symbol_table* pTable){
    // jump to exit label
    int topL = pTable->loopLabelNumber;
    char labelE[MAX_STRING_SIZE];
    snprintf(labelE,MAX_STRING_SIZE,"LABEL_WHILE_EXIT_%d",topL);
    // emit goto exit
    fprintf(asmOut,"    goto %s\n",labelE);
    
}
void asmGenDoWhileBreak(symbol_table* pTable){
    asmGenWhileBreak(pTable);
}

void asmGenForBreak(symbol_table* pTable){
    // jump to exit label
    int topL = pTable->loopLabelNumber;
    char labelE[MAX_STRING_SIZE];
    snprintf(labelE,MAX_STRING_SIZE,"LABEL_FOR_EXIT_%d",topL);
    // emit goto exit
    fprintf(asmOut,"    goto %s\n",labelE); 
}

void asmGenWhileContinue(symbol_table* pTable){
    // append goto begin
    // labels : begin
    int topL = pTable->loopLabelNumber;
    char labelB[MAX_STRING_SIZE];
    snprintf(labelB,MAX_STRING_SIZE,"LABEL_WHILE_BEGIN_%d",topL);
    // emit goto begin
    fprintf(asmOut,"    goto %s\n",labelB);
}
void asmGenDoWhileContinue(symbol_table* pTable){
    // label test 
    int topL = pTable->loopLabelNumber;
    char labelT[MAX_STRING_SIZE];
    snprintf(labelT,MAX_STRING_SIZE,"LABEL_WHILE_TEST_%d",topL);
    // goto test
    fprintf(asmOut,"    goto %s\n",labelT); 
}
void asmGenForContinue(symbol_table* pTable){
    // goto increase
    // labels : increase
    int topL = pTable->loopLabelNumber;
    char labelI[MAX_STRING_SIZE];
    snprintf(labelI,MAX_STRING_SIZE,"LABEL_FOR_INCREASE_%d",topL);
    // emit goti increase
    fprintf(asmOut,"    goto %s\n",labelI);
}

void asmGenAssignment(typeStruct_t* pDst,typeStruct_t* pSrc){
    
    int srcType = pSrc->v_type;
    int dstType = pDst->v_type;
    //DEBUG//printf("Set Value to Variable %s with source type %s, target type %s\n",pDst->sval,TYPE_NAME[srcType],TYPE_NAME[dstType]);
    bool old_check = coercion_check_only;
    coercion_check_only = true;
    check_variable_assign_able(pDst,pSrc);  
    coercion_check_only = old_check;
    // do corecion 
    asmGenCoercion(dstType,srcType);
    // generate assembly
    // this time , expr is appear at top of stack
    // the operand stack will load this variable
    s_table_entry* ent = findVarEntry(pDst->sval);
    asmGenStoreEntry(ent);
}

void asmGenBinaryExpression(typeStruct_t* pDst,typeStruct_t* pSrc1,typeStruct_t* pSrc2,int op_type){
    // sanity check
    if(!checkTypeStructSanity(pDst) || !checkTypeStructSanity(pSrc1) || !checkTypeStructSanity(pSrc2)){
        return;
    }
    // relation op type convert
    int general_op_type = op_type;
    if(op_type >= OP_NOT && op_type <= OP_LE ){
        general_op_type = OP_RELATION;
    }
    // check semantics and compute final type
    int srcType1 = pSrc1->v_type;
    int srcType2 = pSrc2->v_type;
    bool old_check = coercion_check_only;
    coercion_check_only = true;
    pDst->v_type = binary_numeric_expr_check(pSrc1,pSrc2,general_op_type);
    coercion_check_only = old_check;
    pDst->is_const = false;
    int dstType = greaterType;
    // assembly gen.
    // now, two operand was on the stack
    // but type still in consist
    // first store RHS operand into local variable tmp
    asmGenLoadStackToLocal(pSrc2,VN_TMP1); 
    // try coercion for LHS
    asmGenCoercion(dstType,srcType1);
    asmGenStoreLocalToStack(pSrc2,VN_TMP1);// push RHS back
    // try coercion for RHS
    asmGenCoercion(dstType,srcType2);
    // gen operator
    if(general_op_type == OP_RELATION){
        asmGenRelationalOperator(dstType,op_type);
    }
    else{
        asmGenOperator(pDst,op_type);
    }

    
}
void asmGenFunctionHead(const char* name){
    // search in global table
    symbol_table* pTable = global_table();
    for(int i=0;i<pTable->end;i++){
        s_table_entry* ent = &pTable->entries[i];
        // function kind only
        if(ent->kind == K_FUNCTION){
            // same name
            if(strcmp(name,ent->name)==0){
                // generate name
                fprintf(asmOut,".method public static %s(",name);
                // output parameter type
                bool mainDef = false;
                if(strcmp(name,"main")==0){
                    // main    
                    mainDef = true;
                    fprintf(asmOut,"[Ljava/lang/String;");
                }
                else{
                    // otherwise
                    asmGenFunctionTypebyList(&ent->attr.param_list);

                }
                // return type 
                if(mainDef){
                        fprintf(asmOut,")V\n");
                }
                else{
                    if(ent->type.v_type >= 0){
                        fprintf(asmOut,")%s\n",TYPE_ASM[ent->type.v_type]);
                    }
                    
                }

                // stack info (FIXED)
                fprintf(asmOut,".limit stack %d\n",MAX_JAVA_STACK);
                fprintf(asmOut,".limit locals %d\n",MAX_JAVA_LOCAL);
                
                // main function process
                if(mainDef){
                    asmGenScanner();
                }
                break;

            }
        }
    }
    
}
void asmGenFunctionTail(){
    // force return, prevent falling through
    fprintf(asmOut,"    return\n");
    fprintf(asmOut,".end method\n");
}
void asmGenFunctionReturn(int dstType,int srcType){
    // coercion
    asmGenCoercion(dstType,srcType);
    // generate return instruction
    fprintf(asmOut,"    %sreturn\n",TYPE_ASM_PREFIX[dstType]);
    
}
void asmGenPrintHead(){
    fprintf(asmOut,"    getstatic java/lang/System/out Ljava/io/PrintStream;\n");
}
void asmGenPrintTail(typeStruct_t* pType){
    if(pType->v_type >= 0){
        fprintf(asmOut,"    invokevirtual java/io/PrintStream/print(%s)V\n",TYPE_ASM[pType->v_type]); 
    }
}
void asmGenReadHead(){
    fprintf(asmOut,"    getstatic %s/_sc Ljava/util/Scanner;\n",OUT_NAME);
}
void asmGenReadTail(typeStruct_t* pType){
    const char* asmFix = NULL;
    switch(pType->v_type){
    case T_INT:
        asmFix = "Int()I";
        break;
    case T_BOOL:
        asmFix = "Boolean()Z";
        break;
    case T_FLOAT:
        asmFix = "Float()F";
        break;
    case T_DOUBLE:
        asmFix = "Double()D";
        break;
    }
    s_table_entry* ent = findVarEntry(pType->sval);
    if(asmFix && ent){
        fprintf(asmOut,"    invokevirtual java/util/Scanner/next%s\n",asmFix);
        asmGenStoreEntry(ent);
        
    }

}
void asmGenCoercion(int dstType,int srcType){
    if(dstType != srcType){
        //DEBUG//printf("[ASM] Coercion from %s to %s\n",TYPE_NAME[srcType],TYPE_NAME[dstType]);
    }
    // if no one error
    switch(dstType){
    case T_FLOAT:
        if(srcType == T_INT){
            // INT to FLOAT
            fprintf(asmOut,"    i2f\n");
        }
        break;
    case T_DOUBLE:
        switch(srcType){
        case T_INT:
            // INT to DOUBLE
            fprintf(asmOut,"    i2d\n");
            break;
        case T_FLOAT:
            // FLOAT to DOUBLE
            fprintf(asmOut,"    f2d\n");
            break;
        }
    }
}

void asmGenLiteral(typeStruct_t* pConst){
    switch(pConst->v_type){
    case T_INT:
        fprintf(asmOut,"    ldc %d\n",pConst->val);
        break;
    case T_BOOL:
        fprintf(asmOut,"    iconst_%d\n",pConst->val);
        break;
    case T_STRING:
        fprintf(asmOut,"    ldc \"%s\"\n",pConst->sval);
        break;
    case T_FLOAT:
        fprintf(asmOut,"    ldc %f\n",pConst->fval);
        break;
    case T_DOUBLE:
        fprintf(asmOut,"    ldc2_w %lf\n",pConst->dval);
        break;
    }  
    
}

void asmGenStoreEntry(s_table_entry* ent){
    
    if(ent){
        // load it value
        if(ent->level > 0){
            // local  
            // retrive var num
            //DEBUG//printf("-->Local, Var num:%d\n",ent->varNum);
            // type prefix
            const char* prefix = "ERROR";
            switch(ent->type.v_type){
            case T_BOOL:case T_INT:
                prefix = "i";
                break;
            case T_FLOAT:
                prefix = "f";
                break;
            case T_DOUBLE:
                prefix = "d";
                break;
            }
            fprintf(asmOut,"    %sstore %d\n",prefix,ent->varNum);
        }
        else{
            // global   
            if(ent->type.v_type >= 0){
                fprintf(asmOut,"    putstatic %s/%s %s\n",OUT_NAME,ent->name,TYPE_ASM[ent->type.v_type]);
            }
        }
    }
}
// function call, option is "discard return value"
void asmGenFunctionCall(const char* name,bool discard){
    // find function entry
    s_table_entry* ent = findEntryByName(name,entryIsFunction);
    if(ent){
        // entry exist , so does args on stack
        fprintf(asmOut,"    invokestatic %s/%s(",OUT_NAME,name); 
        // param types
        asmGenFunctionTypebyList(&ent->attr.param_list);
        // return type and close
        fprintf(asmOut,")%s\n",TYPE_ASM[ent->type.v_type]);
        if(discard){
            // discard return value for bool,int,float,double(2-word)
            switch(ent->type.v_type){
            case T_INT: case T_BOOL: case T_FLOAT: // single-word
                fprintf(asmOut,"    pop\n");
                break;
            case T_DOUBLE: // two-word
                fprintf(asmOut,"    pop2\n");
                break;
            }
        }
    }
}

void asmGenFunctionTypebyList(typeList_t* pList){ 
    // otherwise
    for(int j=0;j<pList->end;j++){
        typeStruct_t* pType = &pList->data[j];
        if(pType->v_type >= 0){
            fprintf(asmOut,TYPE_ASM[pType->v_type]);
        }
    }
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
        char outName[MAX_STRING_SIZE];
        snprintf(outName,MAX_STRING_SIZE,"%s.j",OUT_NAME);
	asmOut = fopen( outName, "w" );
        KIND_NAME[1] = "function";
        KIND_NAME[2] = "parameter";
        KIND_NAME[4] = "variable";
        KIND_NAME[8] = "constant";
        init_id_table();
        str_pool.end = 0;
        init_table_stack(&stk);
        init_errorPool();
        initLabelMgmt(&labelM);
        initFuncCallStk();

        asmGenProgramHead();
	yyin = fp;
	yyparse();
        
        if (Opt_Statistic)
        {
            show_freq_table();
            // Print frequency table
        }
        dispose_id_table();
        int ret_code = 0;
        if(!show_errors()){           
            fprintf( stdout, "\n" );
            fprintf( stdout, "|---------------------------------------------|\n" );
            fprintf( stdout, "|  There is no syntactic and semantic error!  |\n" );
            fprintf( stdout, "|---------------------------------------------|\n" );
        }
        else{
            ret_code = 1;
        }
        clear();

	exit(ret_code);
}

