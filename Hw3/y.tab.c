#include <stdlib.h>
#include <string.h>
#ifndef lint
#ifdef __unused
__unused
#endif
static char const 
yyrcsid[] = "$FreeBSD: release/9.1.0/usr.bin/yacc/skeleton.c 216370 2010-12-11 08:32:16Z joel $";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
#if defined(__cplusplus) || __STDC__
static int yygrowstack(void);
#else
static int yygrowstack();
#endif
#define YYPREFIX "yy"
#line 2 "parser.y"
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
bool coercion_check_only = false; /* when doing coercion, DO NOT modify source typeStruct_t*/
int scopeTypeTmp = SCOPE_ERROR;
int lastStmtType = STMT_ERROR; /* last stmt type, for 'return check'*/

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

/* symbol type*/
typedef struct{
    bool is_const;
    int type;
    int dim;
    int dims[MAX_DIM+1]; /* n-th dim. store in dims[n], scalar has dim = 0*/
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

/* type string generate*/
void generate_type_string(char* buf,typeStruct_t* pType){
    char indice[MAX_STRING_SIZE];
    bzero(buf,MAX_STRING_SIZE);
    strcat(buf,TYPE_NAME[pType->v_type]);
    for(int i=0;i<pType->dim;i++){
        snprintf(indice,MAX_STRING_SIZE,"[%d]",pType->dims[i]);
        strcat(buf,indice);
    }

}


/* symbol attribute*/
typedef union{
    int int_val;
    float float_val;
    double double_val;
    int bool_val;
    char* string_val;
    typeList_t param_list;
} sym_attr;




/* symbol table entry*/
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

/* symbol table*/
typedef struct{
    s_table_entry entries[MAX_TABLE_ENTRY];
    int end;
    int level;
    int scopeType;
  
} symbol_table;

/* table stack*/
typedef struct{
    symbol_table tables[MAX_TABLES];
    int end;
    int level;
    int pretend;
} table_stack;


/* init a symbol table*/
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

/* push new table and return it*/
/* mode = PRETEND : push new table and reserve (will cancel next push and pretend that new table has been add)*/
symbol_table* push_table(int mode);
/* pop new table and return it*/
symbol_table* pop_table();

/* init a table stack*/
void init_table_stack();

/* string pointer pool*/
struct string_pool {
    void* data[MAX_STRING_POOL];
    int end;
};

/* declar pool*/
struct string_pool str_pool;


void add_ptr(void* str){
    int i = str_pool.end;
    str_pool.data[i] = str;
    /*printf("Pool:Add[%d]='%s'\n",i,str_pool.data[i]);*/
    str_pool.end +=1 ;
}
/* Add string*/
void add_string(char* str){
    add_ptr(str);
}

/* output error msg*/
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
/* declare table*/
table_stack stk;
/* get current table*/
symbol_table* current_table();
/* get global table*/
symbol_table* global_table();
/* create entry in current table*/
s_table_entry* create_entry(){
    symbol_table* table = current_table(); 
    s_table_entry* ent =&table->entries[table->end];
    ent->level = table->level;
    /*DEBUG//printf("New Table Entry created in level %d, index %d\n",table->level,table->end);*/
    table->end++;
    return ent;
}

/* check variable entry exist */
/* check : same scope conflict */
bool check_variable_entry_exist(const char* name,int kind){ 
    /* buf*/
    char errbuf[MAX_STRING_SIZE];
    /* get current table*/
    symbol_table* table = current_table(); 
    /* search current table*/
    for(int i=0;i<table->end;i++){
        s_table_entry* ent = &table->entries[i]; 
        if(strcmp(name,ent->name)==0){
            /* name conflict, no matter which type  (function,variable,constant,parameter...)*/
            /*DEBUG//printf("Name Conflict for %s\n",name);*/
            snprintf(errbuf,MAX_STRING_SIZE,"%s identifier '%s' has been declared in the same scope.",KIND_NAME[kind],name);
            print_error(errbuf);
            return false;
        }
    }
    /* no conflict*/
    return true;
}

/* check function addable, according to define or declarbool check_function_entry_addable(name,kind){*/
bool check_function_entry_addable(const char* name,int kind){
    /* check */
    if(funcOption.isDeclare){
        /* declare, no one can appear before it, rule same as variable*/
        return check_variable_entry_exist(name,kind);
    }
    else{
        /* definition, allow previous is a declare*/
        /* buf*/
        char errbuf[MAX_STRING_SIZE];
        /* get current table*/
        symbol_table* table = current_table(); 
        /* search current table*/
        for(int i=0;i<table->end;i++){
            s_table_entry* ent = &table->entries[i]; 
            if(strcmp(name,ent->name)==0){
                /* check if previous one is declare-function*/
                if(ent->kind==K_FUNCTION && ent->isDeclare){
                    /*DEBUG//printf("Declare has founed,checking... : %s\n",name);*/
                    /* check return type*/
                    if(funcOption.retType != T_ERROR && ent->type.v_type != funcOption.retType){
                        print_error("Return type of function definition is not match with declaration");
                        return false;
                    }
                    /* check param type*/
                    if(!check_typeList_consist(&ent->attr.param_list,funcOption.pTypeList,false)) {
                        print_error("Parameter types of function definition is not match with declaration");
                        return false;
                    }
                    funcOption.paramAddOK = true;
                    ent->isMatch = true;
                    return false;
                }
                else{
                    /* name conflict for non-declare*/
                    /*DEBUG//printf("Name Conflict for %s\n",name);*/
                    snprintf(errbuf,MAX_STRING_SIZE,"Function identifier '%s' has been used before.",name);
                    print_error(errbuf);
                    return false;
                }
            }
        }
        /* no conflict*/
        return true;
    }
}

/* check declare dimension need > 0*/
/* return true if not an array*/
bool check_dimension(typeStruct_t* pType){
    for(int i=0;i<pType->dim;i++){
        if(pType->dims[i] <= 0){
            print_error("Array index must greater than 0");
            return false;
        }
    }
    return true;
}

/* check addable*/
bool check_entry_addable(const char* name,int kind){
    bool result = true;
    /* type error check*/
    /*DEBUG//printf("Type code:%d\n",typeTmp.v_type);*/
    result = result&&(typeTmp.v_type >= 0);
    /* kind check*/
    switch(kind){
        case K_VARIABLE: case K_CONSTANT: case K_PARAMETER:
            /* type cannot be void*/
            if(typeTmp.v_type == T_VOID){
                print_error("'void' cannot be used to declare variable/constant/parameter");
                result = false;
            }
            /* other check*/
            result = result&&check_variable_entry_exist(name,kind);     
            result = result&&check_dimension(&typeTmp); 
            break;
        case K_FUNCTION:
            result = result&&check_function_entry_addable(name,kind);
            break;
        default:
            /* keep*/
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

/* add table entry with name*/
/* option : extra augment -- CANCEL*/
s_table_entry* create_basic_entry(const char* name,int kind){
    /*DEBUG//printf("start entry check\n");*/
    if(!check_entry_addable(name,kind)){
        return NULL;
    }  
    /*DEBUG//printf("pass entry check\n");*/
    /* create  new entry*/
    s_table_entry* ent = create_entry();
    /* set name*/
    bzero(ent->name,sizeof(ent->name));
    strncpy(ent->name,name,32);
    /* set kind*/
    ent->kind = kind;
    /* set type*/
    /* if it is function, type will extract from funcOption*/
    if(kind == K_FUNCTION){
        init_typeStruct(&ent->type); 
        /*DEBUG//printf("Function Ret Type : %d\n", funcOption.retType);*/
        ent->type.v_type = funcOption.retType;
    }
    else{
        memcpy(&ent->type,&typeTmp,sizeof(typeTmp));    
    }
    /* clear array*/
    tmp_type_clear_array_indice();
    /* set option*/
    ent->option = 0;
    /* set declare*/
    if(kind==K_FUNCTION){
        ent->isDeclare = funcOption.isDeclare;
    }
    else{
        ent->isDeclare = false;
    }
    /* set other*/
    ent->isMatch = false;
    /* return*/
    return ent;
}


s_table_entry* process_constant_declare(const char* name,typeStruct_t type) {
    /* basic entry*/
    s_table_entry* ent =  create_basic_entry(name,K_CONSTANT);
    if(ent){ /* if that entry has successfully added*/
        /* fill attribute*/
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
            /*DEBUG//printf("Dim[%d] differ : %d and %d\n",i,a->dims[i],b->dims[i]);*/
            return false;
        }   
    }
    return true;
}

/* check and try to do coercion*/
/* it will change later to fit earlier*/
bool do_type_coercion(typeStruct_t* pTarget,typeStruct_t* pType){
    /*DEBUG//printf("Doing Coercion\n");*/
    bool result = false;
    int src_type = pType->v_type;
    int targetType = pTarget->v_type;
    bool no_show_err = false;
    if(pTarget->v_type == T_ERROR || pType->v_type == T_ERROR){
        /* no coercion when someone is T_ERROR*/
        /*DEBUG//printf("Coercion error on previous error!");*/
        no_show_err = true;
    }
    else if(!check_dim_consist(pTarget,pType)){
        /* dim must be the same to do coercion*/
        print_error("Caonnot perform coercion on arrays with different size or dimension");
        /*DEBUG//printf("Dim %d(%s) to %d(%s)\n",pType->dim,TYPE_NAME[pType->v_type],pTarget->dim,TYPE_NAME[pTarget->v_type]);*/
        no_show_err = true;
    }
    else if(targetType == pType->v_type){
        /*DEBUG//printf("%s to %s\n",TYPE_NAME[src_type],TYPE_NAME[targetType]);*/
        /* same type do not need coercion*/
        result =  true;
    }
    else{
        /*DEBUG//printf("Coercion : %s to %s\n",TYPE_NAME[src_type],TYPE_NAME[targetType]);*/
        switch(targetType){
            case T_FLOAT: /* allow INT to FLOAT*/
                if(src_type==T_INT){
                    if(!coercion_check_only){
                        if(pType->is_const){ /* only constant can convert value*/
                            pType->fval = (float)pType->val;
                            /*DEBUG//printf("Convert INT(%d) to FLOAT(%f)...\n",pType->val,(float)pType->val);*/
                        }
                        pType->v_type = T_FLOAT;
                    }
                    result = true;
                }
                else if(pType->v_type==T_DOUBLE && pType->is_const && pType->is_raw){
                    /* raw float*/
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
                switch(src_type){ /* source type*/
                    case T_INT: /* INT to DOUBLE*/
                        if(pType->is_const){
                            pType->dval = (double)pType->val;
                        }
                        result = true;
                    case T_FLOAT: /* FLOAT to DOUBLE*/
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
    /*DEBUG//printf("Coercion end\n");*/
    if(!result && !no_show_err){
        no_show_err = true;
        char msg[MAX_STRING_SIZE];
        snprintf(msg,MAX_STRING_SIZE,"'%s' cannot be coerced into '%s'",TYPE_NAME[src_type],TYPE_NAME[targetType]); 
        print_error(msg);
    }
    else{
        if(src_type != targetType){
            /*DEBUG//printf("Coerced '%s' to '%s' success\n",TYPE_NAME[src_type],TYPE_NAME[targetType]);*/
        }
    }
    return result;
}

/* check if that assignment is ok*/
/* assign b to a, thus a cannot be const*/
bool check_variable_assign_able(typeStruct_t* a,typeStruct_t* b){
    /* no process for error type*/
    if(a->v_type == T_ERROR || b->v_type == T_ERROR){
        return false;
    }
    /* no array assign*/
    if(a->dim > 0 || b->dim > 0){
        print_error("Array cannot be involved in assignment");
        return false;
    }
    /* check constant*/
    /*DEBUG//printf("Const : %d\n",a->is_const);*/
    if(a->is_const){
        print_error("Cannot do assignment to constant variable");
        return false;
    }
    /* do coercion*/
    /*DEBUG//printf("Variable assign check  type: %s to %s\n",TYPE_NAME[b->v_type],TYPE_NAME[a->v_type]);*/
    return do_type_coercion(a,b);
}

/* check if two type struct is all the same, but errors are distinct*/
bool check_typeStruct_same(typeStruct_t* a,typeStruct_t* b){
    if(a->v_type == T_ERROR || b->v_type == T_ERROR){
        return false; /* error return*/
    }
    /*DEBUG//printf("type check : %s , %s \n",TYPE_NAME[a->v_type],TYPE_NAME[b->v_type]);*/
    
    /* check basic type*/
    if(a->v_type != b->v_type){
        return false;
    }
    /* check dim*/
    if(a->dim != b->dim){
        return false;
    }
    /* check each*/
    for(int i=0;i<a->dim;i++){
        if(a->dims[i]!=b->dims[i]){
            return false;
        }
    }
    return true;
}

bool check_typeList_consist(typeList_t* a,typeList_t* b,bool allow_coercion){
    bool result = true;
    /* check dim*/
    if(a->end != b->end){
        /*DEBUG//printf("Dim mismatch for %d and %d\n",a->end,b->end);*/
        result = false;
    }
    /* check each dims*/
    for(int i=0;i<a->end&&result;i++){
        if(allow_coercion){    
            if(!do_type_coercion(&a->data[i],&b->data[i])){
                /*DEBUG//printf("Coercion failed\n");*/
                result = false;
                break;
            }
        }
        else{ /* no coercion, each type list must be the same*/
            /*DEBUG//printf("***NO COERCION***\n");*/
            /*DEBUG//printf("check in %d\n",i);*/
            if(!check_typeStruct_same(&a->data[i],&b->data[i])){
                result = false;
                break;
            }
            
        }
    }

    return result;
}

/* check consistant*/
int check_function_call_consist(const char* name,typeList_t* pTypeList){
    int ret_type = T_ERROR; /* -1 means not found*/
    int errType = ENTRY_NOT_FOUND; /* extra error msg*/
    /* search global table for function */
    bool old_supress = supressError;
    supressError = true;
    symbol_table* pTable = global_table();
    for(int i=0;i<pTable->end;i++){
        s_table_entry* ent = &pTable->entries[i];
        /* function kind only*/
        if(ent->kind == K_FUNCTION){
            /* same name*/
            if(strcmp(name,ent->name)==0){
                /* check type list consist*/
                /*DEBUG//printf("Check function call...\n");*/
                if(check_typeList_consist(&ent->attr.param_list,pTypeList,true)){
                    /* find return function type*/
                    ret_type = ent->type.v_type;
                    /*DEBUG//printf("Table search : type '%s' for function '%s'\n",TYPE_NAME[ret_type],name);*/
                }
                else{
                    /* parameter mismatch*/
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

/* generate constant attribute*/
void generate_constant_attr_string(char* buf,s_table_entry* ent){
    size_t buf_sz = MAX_STRING_SIZE;
    /* write attribute*/
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
/* generate param list*/
void generate_function_attr_atring(char* buf,s_table_entry* ent){
    bzero(buf,MAX_STRING_SIZE); 
    /* for each type*/
    for(int i=0;i<ent->attr.param_list.end;i++){
        /* comma*/
        if(i!=0){
            strcat(buf,",");
        }
        typeStruct_t* pType = &ent->attr.param_list.data[i];
        /* scalar type*/
        strcat(buf,TYPE_NAME[pType->v_type]);
        /* array indice*/
        for(int j=0;j<pType->dim;j++){
            snprintf(tempBuf,MAX_STRING_SIZE,"[%d]",pType->dims[j]);
            strcat(buf,tempBuf);
        }
    }
}

/* attr string generate*/
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

/* table dump*/
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
/* dump current table*/
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
            /* if previous has no error, means this time has a convert error*/
            char errmsg[MAX_STRING_SIZE];
            snprintf(errmsg,MAX_STRING_SIZE,errFmt,OP_NAME[op],OP_NEED_TYPE[op],TYPE_NAME[pType->v_type]);
            print_error(errmsg);
        }
        else{
            /* error cause by previous error*/
            /*DEBUG//printf("Previous error occur on %s operator, require %s \n",OP_NAME[op],OP_NEED_TYPE[op]);*/
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

/* check whether given expr has correct type for binary numeric operator, return the final computation type*/
int binary_numeric_expr_check(typeStruct_t* lhs,typeStruct_t* rhs,int op){
    int ret_type = T_ERROR;
    funcTypeCheck checker = get_binary_numeric_expr_check_func(op);
    if(!noArrayCheck(lhs)||!noArrayCheck(rhs)){ /* check if has array type*/
        ret_type = T_ERROR;       
    }
    else if(!operatorTypeChecker(lhs,op,checker,"%s operator expect a(n) '%s' on left-hand-side, but '%s' provided.")){ /* LHS*/
        ret_type = T_ERROR;      
    }
    else if(!operatorTypeChecker(rhs,op,checker,"%s operator expect a(n) '%s' on right-hand-side, but '%s' provided.")){ /* RHS*/
        ret_type = T_ERROR;      
    }
    else{ /* all correct!*/
        /* set return type*/
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

/* find variable/parameter/constant type for that name*/
void fill_type_for_name(typeStruct_t* pType,const char* name,int flag){
    bool found = false;
    /* search each table*/
    /*DEBUG//printf("***Look Flag:%X\n",flag);*/
    for(int i = stk.end-1;i>=0&&!found;i-- ){
        symbol_table* pTable = &stk.tables[i];
        /*DEBUG//printf("==>Table look up %s at level %d\n",name,pTable->level);*/
        for(int j = pTable->end-1;j>=0&&!found;j--){
            s_table_entry* ent = &pTable->entries[j];
            /*DEBUG//printf("Flag Check : %X & %X => %d\n",ent->kind,flag,checkFlag(ent->kind,flag));*/
            if((checkFlag(ent->kind,flag))&&strcmp(ent->name,name)==0){
                /* same name and kind are consist*/
                /* fill every fields*/
                pType->v_type = ent->type.v_type;
                pType->dim = ent->type.dim;
                memcpy(&pType->dims,&ent->type.dims,sizeof(pType->dims));
                /* set constant*/
                if(ent->kind == K_CONSTANT){
                    pType->is_const = true;
                }
                else{
                    pType->is_const = false;
                }
                /*DEBUG//printf("===> Found '%s' as type : %s, const : %s , dimensions : %d\n",name,TYPE_NAME[pType->v_type],pType->is_const ? "true":"false",pType->dim);*/
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
    /*DEBUG//printf("Performing Negation...type id :%d\n",pType->v_type);*/
    switch(pType->v_type){
        case T_FLOAT:
            pType->fval *= -1;
            break;
        case T_DOUBLE:
            pType->dval *= -1;
            break;
        case T_INT:
            pType->val *= -1;
            /*DEBUG//printf("Integer negated val : %d\n", pType->val);*/
            break;
    }
}

/* check if array initializer type and size are OK*/
bool check_array_initializer(typeStruct_t* pIndice,typeList_t* pList){
    int total = 1;
    /* run each dim and check the dim*/
    for(int i=0;i<pIndice->dim;i++){
        int dim = pIndice->dims[i];
        if(dim <= 0){
            print_error("Array initializer:Array index must greater than or equal to 0");
            return false;
        }
        total *= dim;
    }
    /* check count exceeds?*/
    if(pList->end > total){
        print_error("Array initializer:The number of initializer exceeds the array size");
        return false;
    }
    /* check expression match*/
    /* valid struct*/
    typeStruct_t valid;
    init_typeStruct(&valid);
    valid.v_type = pIndice->v_type;
    /* coercion only need check*/
    bool old_check = coercion_check_only;
    coercion_check_only = true;
    for(int i=0;i<pList->end;i++){
        typeStruct_t* pType = &pList->data[i];
        if(!do_type_coercion(&valid,pType)){
            /* convert fail*/
            return false;
        }       
    }
    coercion_check_only = old_check;
    /* pass*/
    return true;

}

/* check current table or outter table has that scope rule*/
bool check_scope_match(int scopeRule){
    for(int i = stk.end-1;i>=0;i-- ){
        symbol_table* pTable = &stk.tables[i];
        /*DEBUG//printf("==>Table check at level %d , scope : %d\n",pTable->level,pTable->scopeType);*/
        if(checkFlag(pTable->scopeType,scopeRule)){
            return true;
        }
    }
    return false;

    
}

/* check is in loop*/
bool check_in_loop(){
    return check_scope_match(SCOPE_LOOP);
}

/* process loop check*/
bool process_loop_check(){
    if(!check_in_loop()){
        print_error("'break/continue' can only used inside loops");
    } 
}

/* check is in retrun-able function*/
bool check_in_return_function(){
    return check_scope_match(SCOPE_FUNCTION);
}


/* process return-able check*/
bool process_return_function_check(typeStruct_t* pType){
    /* check-in function*/
    if(!check_in_return_function()){
        print_error("'return' can only used inside non-void function");
        return false;
    } 
    /* scalar check*/
    if(pType->dim > 0){
        print_error("return value should not be an array type");
        return false;
    }
    /* check function type*/
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

/* check if expr is printable*/
bool check_printable(typeStruct_t* pType){
    /* ignore when previous error*/
    if(pType->v_type==T_ERROR){
        return false;
    }
    /* array can't print*/
    if(pType->dim > 0){
        print_error("Cannot use 'print' on an array variable.");
        return false;
    }
    return true;
     
}

/* check if var_ref is readable for KW_READ*/
bool check_readable(typeStruct_t* pType){
    /* ignore when error*/
    if(pType->v_type==T_ERROR){
        return false;
    }
    /* constant can't read*/
    if(pType->is_const){
        print_error("Cannot use 'read' on a constant variable.");
        return false;
    }
    /* cannot read array*/
    if(pType->dim > 0){
        print_error("Cannot use 'read' on an array variable.");
        return false;
    }
    return true;
}


void go_to_loop_scope(){
    /*DEBUG//printf("Go into a loop scope\n");*/
    scopeTypeTmp = SCOPE_LOOP;
}

/* END_DECLARE*/
#line 1020 "parser.y"
typedef union {
    int value;
    char* text;
    int type;
    typeStruct_t typeStruct;
    typeList_t typeList;
} YYSTYPE;
#line 1049 "y.tab.c"
#define YYERRCODE 256
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
const short yylhs[] = {                                        -1,
    0,    0,   23,   23,   23,   21,   21,   21,   21,   21,
   21,    3,    4,    4,    4,    4,    4,    4,    2,    2,
    5,    5,    6,    6,    6,    1,   16,   16,   18,   25,
   27,   27,   28,   28,    7,    7,    7,    7,   26,   24,
   24,   30,   22,    9,    9,    8,    8,   10,   10,   32,
   29,   31,   31,   34,   34,   35,   35,   33,   33,   36,
   36,   37,   37,   37,   37,   37,   37,   37,   39,   39,
   43,   44,   45,   40,   46,   40,   47,   41,   49,   49,
   48,   48,   48,   51,   50,   50,   42,   42,   42,   38,
   38,   38,   13,   13,   15,   15,   17,   14,   52,   52,
   52,   52,   52,   52,   53,   53,   11,   11,   11,   11,
   11,   11,   11,   11,   11,   11,   11,   11,   11,   12,
   20,   20,   19,   19,
};
const short yylen[] = {                                         2,
    3,    2,    2,    2,    0,    2,    2,    2,    1,    1,
    1,    3,    1,    1,    1,    1,    1,    1,    1,    2,
    1,    3,    1,    3,    6,    1,    2,    1,    3,    4,
    1,    3,    3,    4,    1,    1,    1,    1,    6,    2,
    1,    0,    7,    1,    0,    2,    3,    3,    1,    0,
    5,    1,    0,    2,    1,    1,    1,    1,    0,    2,
    1,    1,    1,    1,    1,    1,    1,    2,    1,    1,
    5,    7,    0,    6,    0,    8,    0,   10,    1,    0,
    1,    1,    0,    3,    1,    3,    3,    2,    2,    2,
    3,    3,    1,    2,    2,    1,    3,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    3,    3,    3,    3,
    3,    2,    3,    3,    3,    2,    1,    1,    1,    4,
    1,    0,    1,    3,
};
const short yydefred[] = {                                      0,
   13,   14,   16,   15,   17,    0,   18,    0,   10,    0,
    0,    5,    9,   11,    0,   26,    0,   23,    0,    0,
    7,    5,    6,    8,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   12,    0,    0,    3,   41,    4,    0,
   30,    0,    0,    0,   49,    0,    0,    0,    0,    0,
   35,   36,   37,   38,    0,  117,    0,  118,  119,    0,
    0,   27,    0,   22,    0,    0,   40,    0,   33,   32,
    0,    0,   46,    0,    0,    0,    0,  112,    0,    0,
   94,    0,  106,  105,   99,  100,  104,  102,  101,  103,
    0,    0,    0,    0,    0,    0,    0,   29,    0,    0,
    0,   34,   47,   20,   39,    0,   48,  113,    0,    0,
    0,    0,   95,    0,    0,  109,  110,  111,    0,    0,
    0,    0,    0,   50,   43,    0,  120,   97,   25,    0,
    0,    0,    0,    0,   42,   56,    0,   57,    0,   52,
    0,    0,    0,    0,   77,   73,   75,    0,    0,    0,
    0,    0,   62,    0,    0,   61,   63,   64,   65,   66,
   67,   69,   70,    0,   54,    0,   88,   89,    0,    0,
    0,    0,    0,    0,    0,   68,    0,   51,   60,   90,
   87,    0,    0,    0,    0,    0,   91,   92,    0,    0,
    0,    0,    0,   85,    0,    0,    0,    0,    0,    0,
    0,    0,   79,    0,   86,   74,    0,    0,    0,    0,
   72,    0,   76,    0,   78,
};
const short yydgoto[] = {                                       8,
   55,   18,    9,   10,   19,   20,   56,   45,   46,   47,
  185,   58,   59,  186,   81,   32,   82,   33,  110,  111,
   37,   38,   25,   39,   13,   14,   27,   28,  153,  106,
  139,  132,  154,  140,  141,  155,  156,  157,  158,  159,
  160,  161,  162,  163,  170,  171,  169,  192,  204,  193,
  164,   96,   97,
};
const short yysindex[] = {                                    110,
    0,    0,    0,    0,    0,  172,    0,    0,    0, -282,
  110,    0,    0,    0, -282,    0,  -30,    0, -244, -229,
    0,    0,    0,    0,  110, -217, -207, -204,  151,  -40,
 -194, -189,   18,    0, -282,  110,    0,    0,    0,  -44,
    0, -282,  172, -282,    0,   75, -163,  -40,  -40,  -40,
    0,    0,    0,    0,  -26,    0,  115,    0,    0,   36,
   12,    0,  -89,    0, -282, -282,    0, -172,    0,    0,
 -282,   18,    0, -148,  151,  -34,  124,    0,  -40,  -40,
    0,   47,    0,    0,    0,    0,    0,    0,    0,    0,
  -40,  -40,  -40,  -40,  -40,  -40,  -40,    0,  -40,  -15,
   99,    0,    0,    0,    0,   17,    0,    0,  115, -147,
  101,  106,    0,   86,   86,    0,    0,    0,  144,  124,
   22,  151,  151,    0,    0,  -40,    0,    0,    0,  105,
  109,  110,  115, -148,    0,    0, -282,    0,  -57,    0,
  110,  -40, -131, -130,    0,    0,    0,  116,  -40, -282,
 -126, -124,    0,   39,  -57,    0,    0,    0,    0,    0,
    0,    0,    0, -117,    0,   40,    0,    0,  130,  132,
   17,  -40,   58,   47, -107,    0,  -40,    0,    0,    0,
    0,  -40,  -40, -103,  115,  139,    0,    0,  115,  115,
 -124, -101, -100,    0,  149,  153,   17,  -40, -282,   17,
  -40,  -72,    0,  -90,    0,    0,  155,   17,  -40,  -85,
    0,  161,    0,   17,    0,
};
const short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -213,    0,    0,  -82,
    0,    0,    0,    0,  215,    0,    0,  -66,  179,    0,
    0, -199,  -21,    0,    0,  228,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  189,    0,    0,    0,
    0,    0,    0,    0,  -14,    0, -181,    0,    0,    0,
    0,    0, -213,    0,    0,    0,    0,    0,    0,    0,
    0,   -3,    0,  111,    0,    0,   -1,    0,  196,    0,
    0,    6,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  119, -213,
    0,    0,    0,    0,    0,    0,    0,    0,  -23,  -29,
    0,    0,    0,   72,   89,    0,    0,    0,   19,   52,
    0,  179,  179,    0,    0,    0,    0,    0,    0,    0,
    0,  -73,  -11,    0,    0,    0,    0,    0,  120,    0,
  195,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -263,    0,    0,    0,    0,    0,    0,
    0,    7,    0,    0,    1,    0,    0,    0,   65,    5,
   26,    0,   21,    0,    0,    0,    0,   20,    0,    0,
    0,  281,    0,    0,    0,    0,    0,    0,  256,    0,
    0,    0,    0,    0,    0,
};
const short yygindex[] = {                                      0,
  392,   -7,   -5,   51,  273,    0,  -24,  244,  -88,    0,
  373,  -81,  289, -162,  241,  -16,    0,    0,    0,  225,
  326,   15,  307,    0,   44,    8,  294,    0,    4,    0,
    0,    0,    0,  197,    0,    0,  184,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  142,    0,    0,
  -91,    0,    0,
};
#define YYTABLESIZE 591
const short yytable[] = {                                      48,
   68,   31,   95,   16,   50,   21,  108,   93,   91,   29,
   92,  121,   94,   79,   12,   69,   62,  123,   24,   28,
  195,   93,   93,   93,  122,   22,   93,   93,   93,  124,
   93,   21,   93,  130,  131,  203,   73,   19,  207,  116,
   34,   98,   96,  102,   24,   81,   96,   96,   96,   53,
   96,   53,   96,   67,   23,  104,   15,  151,   35,  114,
   31,   82,  119,  103,   80,  124,  119,  119,  119,   40,
  119,   19,  119,  151,   19,   31,   95,   41,   93,   44,
   23,   93,   91,   42,   92,   20,   94,   65,   20,   66,
  194,  116,  115,   71,   95,  121,   60,   61,   96,   93,
   91,  123,   92,   24,   94,   84,   24,  205,   31,  125,
   93,  114,  107,  124,  107,   74,  107,  194,   51,   52,
   53,   54,   95,  116,   75,   44,  136,   93,   98,  108,
   96,  108,   94,  108,   99,  136,  105,   80,  123,  124,
  126,  127,   95,  114,  115,  134,  129,   93,   91,  135,
   92,   95,   94,  167,  168,  172,   93,   91,  176,   92,
   95,   94,  177,  178,  107,   93,   91,  180,   92,  182,
   94,  183,   44,   44,  184,  138,  115,  188,  196,  197,
   95,  108,  137,  198,  138,   93,   91,  199,   92,  200,
   94,  137,  201,  208,  209,  210,  107,   30,  128,  213,
  202,  214,   21,  206,   53,   53,   53,   53,   53,   53,
   53,  211,   53,  108,    2,   53,   53,  215,   31,   45,
  142,  143,  144,  145,  146,  147,  148,    1,   16,   44,
   49,  149,  150,   42,   83,   84,  122,   85,   86,   87,
   88,   89,   90,  122,   59,   16,   51,   52,   53,   54,
   51,   52,   53,   54,   93,   93,   30,   93,   93,   93,
   93,   93,   93,   28,  123,   28,   28,  116,  116,   58,
   93,   30,   93,   93,   96,   96,  124,   96,   96,   96,
   96,   96,   96,  116,   19,   98,  116,  114,  114,   81,
   96,   83,   96,   96,  119,  119,   83,  119,  119,  119,
  119,  119,  119,  114,   80,   82,  114,   64,   83,   84,
  119,   85,   86,   87,   88,   89,   90,   55,  107,   55,
  115,  115,  113,  121,  181,   11,   83,   84,   36,   85,
   86,   87,   88,   89,   90,   70,  115,  165,  179,  115,
  107,  107,  187,  107,  107,  107,  107,  107,  107,   84,
  212,    0,   84,    0,    0,    0,  107,  108,  108,  107,
  108,  108,  108,  108,  108,  108,    1,    2,    3,    4,
    5,    6,    7,  108,   83,   84,  108,   85,   86,   87,
   88,   89,   90,   83,   84,    0,   85,   86,   87,   88,
   89,   90,    0,    0,    0,   85,   86,   87,   88,   89,
   90,   17,   57,   71,    0,   71,   26,    1,    2,    3,
    4,    5,   43,    7,    0,    0,    0,    0,    0,    0,
   76,   77,   78,    0,    0,    0,   63,  152,    1,    2,
    3,    4,    5,   26,    7,   72,    0,    0,  175,    0,
    0,    0,    0,  152,    0,    0,    0,    0,    0,    0,
    0,  109,  112,    0,    0,    0,  100,  101,    0,    0,
    0,    0,   72,  114,  115,  116,  117,  118,  119,  120,
  191,  109,   55,   55,   55,   55,   55,   55,   55,    0,
   55,    0,    0,   55,   55,    0,    0,  152,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  191,  133,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  166,    0,    0,    0,    0,    0,
    0,  173,    0,    0,    0,    0,    0,    0,   63,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  174,    0,    0,    0,    0,    0,    0,    0,  189,
    0,    0,    0,    0,  190,    0,    0,    0,   71,   71,
   71,   71,   71,   71,   71,    0,   71,    0,    0,   71,
   71,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  190,    0,    0,    0,    0,    0,    0,    0,    0,
  174,
};
const short yycheck[] = {                                      40,
   45,   91,   37,  286,   45,   11,   41,   42,   43,   40,
   45,   41,   47,   40,    0,   40,   33,   41,   11,   41,
  183,  285,   37,  287,   40,   11,   41,   42,   43,   41,
   45,   37,   47,  122,  123,  198,   44,   41,  201,   41,
  285,   41,   37,   68,   37,   41,   41,   42,   43,  123,
   45,  125,   47,   39,   11,   72,    6,  139,  288,   41,
   91,   41,   37,   71,   91,  123,   41,   42,   43,  287,
   45,  285,   47,  155,  288,   91,   37,  285,   93,   29,
   37,   42,   43,  288,   45,  285,   47,   37,  288,   39,
  182,   93,   41,   43,   37,  125,  291,  287,   93,   42,
   43,  125,   45,  285,   47,   41,  288,  199,   91,  106,
  125,   93,   41,  125,   43,   41,   45,  209,  291,  292,
  293,  294,   37,  125,  288,   75,  132,   42,   93,   41,
  125,   43,   47,   45,  123,  141,  285,   91,   40,  123,
  288,   41,   37,  125,   93,   41,  125,   42,   43,   41,
   45,   37,   47,  285,  285,   40,   42,   43,  285,   45,
   37,   47,  287,  125,   93,   42,   43,  285,   45,   40,
   47,   40,  122,  123,  171,  132,  125,  285,  282,   41,
   37,   93,  132,  285,  141,   42,   43,  288,   45,   41,
   47,  141,   40,  266,  285,   41,  125,  287,   93,  285,
  197,   41,  285,  200,  278,  279,  280,  281,  282,  283,
  284,  208,  286,  125,    0,  289,  290,  214,  285,   41,
  278,  279,  280,  281,  282,  283,  284,    0,  286,   41,
  271,  289,  290,  123,  269,  270,   41,  272,  273,  274,
  275,  276,  277,  125,  125,  286,  291,  292,  293,  294,
  291,  292,  293,  294,  269,  270,  287,  272,  273,  274,
  275,  276,  277,  285,  288,  287,  288,  269,  270,  125,
  285,  287,  287,  288,  269,  270,  288,  272,  273,  274,
  275,  276,  277,  285,  288,  285,  288,  269,  270,  285,
  285,  285,  287,  288,  269,  270,   41,  272,  273,  274,
  275,  276,  277,  285,  285,  285,  288,   35,  269,  270,
  285,  272,  273,  274,  275,  276,  277,  123,   75,  125,
  269,  270,   82,   99,  285,    0,  269,  270,   22,  272,
  273,  274,  275,  276,  277,   42,  285,  141,  155,  288,
  269,  270,  285,  272,  273,  274,  275,  276,  277,  285,
  209,   -1,  288,   -1,   -1,   -1,  285,  269,  270,  288,
  272,  273,  274,  275,  276,  277,  257,  258,  259,  260,
  261,  262,  263,  285,  269,  270,  288,  272,  273,  274,
  275,  276,  277,  269,  270,   -1,  272,  273,  274,  275,
  276,  277,   -1,   -1,   -1,  272,  273,  274,  275,  276,
  277,   10,   30,  123,   -1,  125,   15,  257,  258,  259,
  260,  261,  262,  263,   -1,   -1,   -1,   -1,   -1,   -1,
   48,   49,   50,   -1,   -1,   -1,   35,  139,  257,  258,
  259,  260,  261,   42,  263,   44,   -1,   -1,  150,   -1,
   -1,   -1,   -1,  155,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   79,   80,   -1,   -1,   -1,   65,   66,   -1,   -1,
   -1,   -1,   71,   91,   92,   93,   94,   95,   96,   97,
  182,   99,  278,  279,  280,  281,  282,  283,  284,   -1,
  286,   -1,   -1,  289,  290,   -1,   -1,  199,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  209,  126,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  142,   -1,   -1,   -1,   -1,   -1,
   -1,  149,   -1,   -1,   -1,   -1,   -1,   -1,  137,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  150,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  177,
   -1,   -1,   -1,   -1,  182,   -1,   -1,   -1,  278,  279,
  280,  281,  282,  283,  284,   -1,  286,   -1,   -1,  289,
  290,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  209,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  199,
};
#define YYFINAL 8
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 294
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"'%'",0,0,"'('","')'","'*'","'+'",0,"'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,"']'",0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,"INT","BOOL","FLOAT","DOUBLE","STRING","CONST","VOID","END",
"LESS_THAN_ELSE","KW_ELSE","ARITHMETIC_PARENTHESES",
"LOWER_THEN_ARITHMETIC_PARENTHESES","OP_OR","OP_AND","OP_NOT","OP_LT","OP_LE",
"OP_EQ","OP_GE","OP_GT","OP_NE","KW_RETURN","KW_BREAK","KW_CONTINUE","KW_FOR",
"KW_WHILE","KW_DO","KW_IF","SEMICOLON","ID","ASSIGN","COMMA","KW_PRINT",
"KW_READ","INTEGER_CONSTANT","FLOATING_CONSTANT","BOOLEAN_CONSTANT",
"STRING_CONSTANT",
};
const char * const yyrule[] = {
"$accept : program",
"program : declaration_list funct_def decl_and_def_list",
"program : funct_def decl_and_def_list",
"decl_and_def_list : decl_and_def_list declaration_list",
"decl_and_def_list : decl_and_def_list definition_list",
"decl_and_def_list :",
"declaration_list : declaration_list const_decl",
"declaration_list : declaration_list var_decl",
"declaration_list : declaration_list funct_decl",
"declaration_list : const_decl",
"declaration_list : var_decl",
"declaration_list : funct_decl",
"var_decl : type var_list SEMICOLON",
"type : INT",
"type : BOOL",
"type : DOUBLE",
"type : FLOAT",
"type : STRING",
"type : VOID",
"identifier : scalar_id",
"identifier : scalar_id array_indice",
"var_list : var_entry",
"var_list : var_entry COMMA var_list",
"var_entry : identifier",
"var_entry : scalar_id ASSIGN expr",
"var_entry : scalar_id array_indice ASSIGN '{' zero_or_more_expr_list '}'",
"scalar_id : ID",
"array_indice : single_array_indice array_indice",
"array_indice : single_array_indice",
"single_array_indice : '[' INTEGER_CONSTANT ']'",
"const_decl : CONST type const_list SEMICOLON",
"const_list : single_const",
"const_list : single_const COMMA const_list",
"single_const : scalar_id ASSIGN literal_constant",
"single_const : scalar_id ASSIGN '-' literal_constant",
"literal_constant : INTEGER_CONSTANT",
"literal_constant : FLOATING_CONSTANT",
"literal_constant : BOOLEAN_CONSTANT",
"literal_constant : STRING_CONSTANT",
"funct_decl : type scalar_id '(' args ')' SEMICOLON",
"definition_list : definition_list funct_def",
"definition_list : funct_def",
"$$1 :",
"funct_def : type scalar_id '(' args ')' $$1 compound_stat",
"args : arg_list",
"args :",
"single_arg : type identifier",
"single_arg : CONST type identifier",
"arg_list : arg_list COMMA single_arg",
"arg_list : single_arg",
"$$2 :",
"compound_stat : '{' $$2 local_decl_list zero_or_more_stat_list '}'",
"local_decl_list : var_decl_list",
"local_decl_list :",
"var_decl_list : var_decl_entry var_decl_list",
"var_decl_list : var_decl_entry",
"var_decl_entry : var_decl",
"var_decl_entry : const_decl",
"zero_or_more_stat_list : stat_list",
"zero_or_more_stat_list :",
"stat_list : stat_list stat",
"stat_list : stat",
"stat : compound_stat",
"stat : simple_stat",
"stat : if_stat",
"stat : while_stat",
"stat : for_stat",
"stat : jump_stat",
"stat : funct_call SEMICOLON",
"if_stat : if_only_stat",
"if_stat : if_else_stat",
"if_only_stat : KW_IF '(' bool_expr ')' compound_stat",
"if_else_stat : KW_IF '(' bool_expr ')' compound_stat KW_ELSE compound_stat",
"$$3 :",
"while_stat : KW_WHILE $$3 '(' bool_expr ')' compound_stat",
"$$4 :",
"while_stat : KW_DO $$4 compound_stat KW_WHILE '(' bool_expr ')' SEMICOLON",
"$$5 :",
"for_stat : KW_FOR $$5 '(' for_expr SEMICOLON for_bool_expr SEMICOLON for_expr ')' compound_stat",
"for_bool_expr : bool_expr",
"for_bool_expr :",
"for_expr : expr",
"for_expr : one_or_more_for_assignment",
"for_expr :",
"single_assginment : var_ref ASSIGN expr",
"one_or_more_for_assignment : single_assginment",
"one_or_more_for_assignment : one_or_more_for_assignment COMMA single_assginment",
"jump_stat : KW_RETURN expr SEMICOLON",
"jump_stat : KW_BREAK SEMICOLON",
"jump_stat : KW_CONTINUE SEMICOLON",
"simple_stat : single_assginment SEMICOLON",
"simple_stat : KW_PRINT expr SEMICOLON",
"simple_stat : KW_READ var_ref SEMICOLON",
"var_ref : scalar_id",
"var_ref : scalar_id intexpr_array_indice",
"intexpr_array_indice : intexpr_single_array_indice intexpr_array_indice",
"intexpr_array_indice : intexpr_single_array_indice",
"intexpr_single_array_indice : '[' expr ']'",
"bool_expr : expr",
"relation_op : OP_LT",
"relation_op : OP_LE",
"relation_op : OP_GT",
"relation_op : OP_GE",
"relation_op : OP_NE",
"relation_op : OP_EQ",
"binary_logic_op : OP_AND",
"binary_logic_op : OP_OR",
"expr : expr '+' expr",
"expr : expr '-' expr",
"expr : expr '*' expr",
"expr : expr '/' expr",
"expr : expr '%' expr",
"expr : '-' expr",
"expr : '(' expr ')'",
"expr : expr relation_op expr",
"expr : expr binary_logic_op expr",
"expr : OP_NOT expr",
"expr : literal_constant",
"expr : funct_call",
"expr : var_ref",
"funct_call : scalar_id '(' zero_or_more_expr_list ')'",
"zero_or_more_expr_list : expr_list",
"zero_or_more_expr_list :",
"expr_list : expr",
"expr_list : expr_list COMMA expr",
};
#endif
#if YYDEBUG
#include <stdio.h>
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;
#line 1797 "parser.y"

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

#line 1707 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
#if defined(__cplusplus) || __STDC__
static int yygrowstack(void)
#else
static int yygrowstack()
#endif
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    newss = yyss ? (short *)realloc(yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

#ifndef YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG void
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* YYPARSE_PARAM */
#ifndef YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG YYPARSE_PARAM_TYPE YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM_TYPE YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! YYPARSE_PARAM */

int
yyparse (YYPARSE_PARAM_ARG)
    YYPARSE_PARAM_DECL
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yyvsp[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 1093 "parser.y"
{
            /*DEBUG//printf("Reduce to program\n");*/
            program_reduce_process();
            }
break;
case 2:
#line 1097 "parser.y"
{
            program_reduce_process();
	    }
break;
case 13:
#line 1118 "parser.y"
{tmp_type_init_scalar(yyvsp[0].type);yyval.type=yyvsp[0].type;}
break;
case 14:
#line 1119 "parser.y"
{tmp_type_init_scalar(yyvsp[0].type);yyval.type=yyvsp[0].type;}
break;
case 15:
#line 1120 "parser.y"
{tmp_type_init_scalar(yyvsp[0].type);yyval.type=yyvsp[0].type;}
break;
case 16:
#line 1121 "parser.y"
{tmp_type_init_scalar(yyvsp[0].type);yyval.type=yyvsp[0].type;}
break;
case 17:
#line 1122 "parser.y"
{tmp_type_init_scalar(yyvsp[0].type);yyval.type=yyvsp[0].type;}
break;
case 18:
#line 1123 "parser.y"
{tmp_type_init_scalar(yyvsp[0].type);yyval.type=yyvsp[0].type;}
break;
case 23:
#line 1136 "parser.y"
{
                /* create basic*/
                create_basic_entry(yyvsp[0].text,K_VARIABLE);
          }
break;
case 24:
#line 1140 "parser.y"
{
                /* create basic*/
                if(yyvsp[0].typeStruct.v_type >= 0){
                    /*DEBUG//printf("Declared '%s' %s with expr type : %s\n",$1,TYPE_NAME[typeTmp.v_type],TYPE_NAME[$3.v_type]);*/
                    if(do_type_coercion(&typeTmp,&yyvsp[0].typeStruct)){
                        /*DEBUG//printf("==>Coercion pass!\n");*/
                        create_basic_entry(yyvsp[-2].text,K_VARIABLE);
                    }
                    else{
                        /*DEBUG//printf("==>Coercion failure!\n");*/
                    }
                }
                else{
                    /*DEBUG//printf("Previous type error!\n");   */
                }
          }
break;
case 25:
#line 1156 "parser.y"
{ 
                /* check type list match*/
                if(check_array_initializer(&typeTmp,&yyvsp[-1].typeList)){    
                    /* create basic*/
                    create_basic_entry(yyvsp[-5].text,K_VARIABLE);
                }
          }
break;
case 26:
#line 1168 "parser.y"
{ yyval.text = yyvsp[0].text; }
break;
case 29:
#line 1177 "parser.y"
{
                        /*DEBUG//printf("Reduce to single indice, dim = %d\n",$2.val);*/
                        tmp_type_add_dim(yyvsp[-1].typeStruct.val);
                        }
break;
case 33:
#line 1192 "parser.y"
{
                /* normal positive ALSO NEED COERCION!!!!*/
                typeStruct_t type;
                memcpy(&type,&yyvsp[0].typeStruct,sizeof(type));
                type.is_raw = false;
                /* float generate*/
                if(typeTmp.v_type==T_FLOAT && type.v_type == T_DOUBLE){ 
                    type.v_type = T_FLOAT;
                    type.fval = (float)type.dval;
                }
                /* try to coercion, convert type struct type recorded in typeTmp*/
                if(do_type_coercion(&typeTmp,&type)){
                    process_constant_declare(yyvsp[-2].text,type);
                }
             }
break;
case 34:
#line 1207 "parser.y"
{
                typeStruct_t type;
                memcpy(&type,&yyvsp[0].typeStruct,sizeof(type));
                type.is_raw = false;
                /* float generate*/
                if(typeTmp.v_type==T_FLOAT && type.v_type == T_DOUBLE){ 
                    type.v_type = T_FLOAT;
                    type.fval = (float)type.dval;
                }
                if(do_type_coercion(&typeTmp,&type)){ /* try to coercion, convert type struct type recorded in typeTmp*/
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
                            /* only numeric constant can use negation '-'*/
                            print_error("Only numeric constant can use negation '-'");
                            success = false;
                            break;
                    }
                    if(success){
                        process_constant_declare(yyvsp[-3].text,type);
                    }
                }
             }
break;
case 39:
#line 1251 "parser.y"
{
                    /*push_table(TABLE_PRETEND);*/
                    /*DEBUG//printf("Reduce  to funct_decl \n");*/
                    /* add function declar entry*/
                    /* add basic entry, check mode is function declare*/
                    init_funcOption();
                    funcOption.isDeclare = true;
                    funcOption.retType = yyvsp[-5].type;
                    s_table_entry* ent = create_basic_entry(yyvsp[-4].text,K_FUNCTION);
                    if(ent){
                        /* set type*/
                        ent->type.v_type = yyvsp[-5].type;
                        /* add attribute*/
                        ent->attr.param_list.end = 0;
                        for(int i=0;i<yyvsp[-2].typeList.end;i++){
                            typeStruct_t* pTarget = &ent->attr.param_list.data[ent->attr.param_list.end];
                            ++ent->attr.param_list.end;
                            /* type*/
                            pTarget->v_type = yyvsp[-2].typeList.data[i].v_type;
                            /* dim*/
                            pTarget->dim = yyvsp[-2].typeList.data[i].dim;
                            /* dims*/
                            memcpy(pTarget->dims,yyvsp[-2].typeList.data[i].dims,sizeof(pTarget->dims));
                        }
                    }


                }
break;
case 42:
#line 1291 "parser.y"
{
                    /* add function declar entry*/
                    /* add basic entry, check mode is function declare*/
                    init_funcOption();
                    funcOption.isDeclare = false;
                    funcOption.pTypeList = &yyvsp[-1].typeList;
                    for(int i=0;i<yyvsp[-1].typeList.end;i++){
                        /*DEBUG//printf("list[%d] %s , dim:%d\n",i,TYPE_NAME[$4.data[i].v_type],$4.data[i].dim);*/
                    }
                    funcOption.retType = yyvsp[-4].type;
                    /*DEBUG//printf("Reduce to funct_def , has type %s\n",TYPE_NAME[$1]); */
                    s_table_entry* ent = create_basic_entry(yyvsp[-3].text,K_FUNCTION);
                    /* add pretend table after function entry added*/
                    /* scope : see it is void or non-void*/
                    /* if entry has an error, we will take current local type as real type*/
                    int effective_type;
                    if(!ent){ 
                        effective_type = yyvsp[-4].type;    
                    }
                    else{
                        effective_type = ent->type.v_type;        
                    }
                    scopeTypeTmp = effective_type == T_VOID ? SCOPE_NORMAL : SCOPE_FUNCTION;
                    /* if error occur, return type will adjust */
                    funcOption.retType = effective_type;
                    push_table(TABLE_PRETEND);
                    /*DEBUG//printf("creating function entry for %s\n",$2);*/
                    if(ent || funcOption.paramAddOK){
                        /* add attribute*/
                        if(ent){
                            ent->attr.param_list.end = 0;
                        }
                        for(int i=0;i<yyvsp[-1].typeList.end;i++){
                            if(yyvsp[-1].typeList.data[i].v_type == T_ERROR){ /* Error arg*/
                                continue;
                            }
                            if(!check_dimension(&yyvsp[-1].typeList.data[i])){ /* dimension check*/
                                continue;
                            }
                            if(ent){
                                typeStruct_t* pTarget = &ent->attr.param_list.data[ent->attr.param_list.end];
                                ++ent->attr.param_list.end;
                                /* type*/
                                pTarget->v_type = yyvsp[-1].typeList.data[i].v_type;
                                /* dim*/
                                pTarget->dim = yyvsp[-1].typeList.data[i].dim;
                                /* dims*/
                                memcpy(pTarget->dims,yyvsp[-1].typeList.data[i].dims,sizeof(pTarget->dims));
                            }
                            /* create param type*/
                            s_table_entry* param = create_basic_entry(yyvsp[-1].typeList.data[i].sval,K_PARAMETER);
                            if (param){
                                /*DEBUG//printf("==>Param (%s) added\n",KIND_NAME[param->kind]);*/
                                /* new param added*/
                                /* set type*/
                                param->type.v_type =  yyvsp[-1].typeList.data[i].v_type;
                                /* set dim*/
                                param->type.dim = yyvsp[-1].typeList.data[i].dim;
                                /* copy dims*/
                                memcpy(param->type.dims,yyvsp[-1].typeList.data[i].dims,sizeof(param->type.dims));
                            }
                            else{
                            
                                /*DEBUG//printf("==>Param add FAILURE\n");*/
                            }
                        } 

                    }
                    
                }
break;
case 43:
#line 1361 "parser.y"
{
                    /* check last stmt*/
                    if(lastStmtType != STMT_COMPOUND_RETURN && yyvsp[-6].type!=T_VOID){
                        print_error("Last statment of non-void function must be a 'return' statment");
                    }
                }
break;
case 45:
#line 1372 "parser.y"
{     
            yyval.typeList.end = 0;
         }
break;
case 46:
#line 1377 "parser.y"
{
                    /*DEBUG//printf("Reduce to single_arg\n");*/
                    yyval.typeStruct.v_type = typeTmp.v_type;
                    /* copy dim info*/
                    yyval.typeStruct.dim = typeTmp.dim;
                    memcpy(&yyval.typeStruct.dims,&typeTmp.dims,sizeof(yyval.typeStruct.dims));
                    tmp_type_clear_array_indice();
                    yyval.typeStruct.is_const = FALSE;
                    yyval.typeStruct.sval = yyvsp[0].text;
                }
break;
case 47:
#line 1387 "parser.y"
{
                    /*DEBUG//printf("Reduce to const single_arg\n");*/
                    yyval.typeStruct.v_type = typeTmp.v_type;
                    /* copy dim info*/
                    yyval.typeStruct.dim = typeTmp.dim;
                    memcpy(&yyval.typeStruct.dims,&typeTmp.dims,sizeof(yyval.typeStruct.dims));
                    tmp_type_clear_array_indice();
                    yyval.typeStruct.is_const = TRUE; 
                    yyval.typeStruct.sval = yyvsp[0].text;
                    }
break;
case 48:
#line 1399 "parser.y"
{
                /*DEBUG//printf("Reduce single_arg,arg_list to arg_list\n");*/
                /* copy old one*/
                memcpy(&yyval.typeList,&yyvsp[-2].typeList,sizeof(yyval.typeList));
                /* merge new one*/
                yyval.typeList.data[yyval.typeList.end].is_const = yyvsp[0].typeStruct.is_const;
                yyval.typeList.data[yyval.typeList.end].v_type = yyvsp[0].typeStruct.v_type;
                yyval.typeList.data[yyval.typeList.end].dim = yyvsp[0].typeStruct.dim;
                memcpy(&yyval.typeList.data[yyval.typeList.end].dims,&yyvsp[0].typeStruct.dims,sizeof(yyval.typeList.data[yyval.typeList.end].dims));
                yyval.typeList.data[yyval.typeList.end].sval = yyvsp[0].typeStruct.sval;
                yyval.typeList.end += 1;
                /* create parameter entry*/
                /*create_basic_entry($1.sval,K_PARAMETER); */
                }
break;
case 49:
#line 1413 "parser.y"
{
                /*DEBUG//printf("Reduce single_arg to arg_list\n");*/
                yyval.typeList.end = 1;
                yyval.typeList.data[0].is_const = yyvsp[0].typeStruct.is_const;
                yyval.typeList.data[0].v_type = yyvsp[0].typeStruct.v_type;
                yyval.typeList.data[0].dim = yyvsp[0].typeStruct.dim;
                memcpy(&yyval.typeList.data[0].dims,&yyvsp[0].typeStruct.dims,sizeof(yyval.typeList.data[0].dims));
                /* name info*/
                yyval.typeList.data[0].sval = yyvsp[0].typeStruct.sval;
                /* create parameter entry*/
                /*create_basic_entry($1.sval,K_PARAMETER);*/
            }
break;
case 50:
#line 1428 "parser.y"
{push_table(TABLE_NORMAL);}
break;
case 51:
#line 1428 "parser.y"
{
                    /*DEBUG//printf("Reduce to compund stat\n");*/
                    pop_table();
                    if(lastStmtType == STMT_RETURN){
                        /*DEBUG//printf("===>Last stmt is RETURN\n");*/
                        lastStmtType = STMT_COMPOUND_RETURN;
                    }
                    else{
                        /*DEBUG//printf("===>Last stmt is NOT RETURN\n");*/
                        lastStmtType = STMT_NORMAL;
                        
                    }     
                }
break;
case 63:
#line 1465 "parser.y"
{lastStmtType = STMT_NORMAL;}
break;
case 64:
#line 1466 "parser.y"
{lastStmtType = STMT_NORMAL;}
break;
case 65:
#line 1467 "parser.y"
{lastStmtType = STMT_NORMAL;}
break;
case 66:
#line 1468 "parser.y"
{lastStmtType = STMT_NORMAL;}
break;
case 73:
#line 1487 "parser.y"
{go_to_loop_scope();}
break;
case 75:
#line 1488 "parser.y"
{go_to_loop_scope();}
break;
case 77:
#line 1493 "parser.y"
{go_to_loop_scope();}
break;
case 84:
#line 1506 "parser.y"
{
                            check_variable_assign_able(&yyvsp[-2].typeStruct,&yyvsp[0].typeStruct);  
                        }
break;
case 87:
#line 1516 "parser.y"
{process_return_function_check(&yyvsp[-1].typeStruct);lastStmtType = STMT_RETURN;}
break;
case 88:
#line 1517 "parser.y"
{process_loop_check(); lastStmtType = STMT_NORMAL;}
break;
case 89:
#line 1518 "parser.y"
{process_loop_check();lastStmtType = STMT_NORMAL;}
break;
case 91:
#line 1523 "parser.y"
{check_printable(&yyvsp[-1].typeStruct);}
break;
case 92:
#line 1524 "parser.y"
{ check_readable(&yyvsp[-1].typeStruct); }
break;
case 93:
#line 1528 "parser.y"
{
                yyval.typeStruct.sval = yyvsp[0].text; /* name*/
                fill_type_for_name((typeStruct_t*)&yyval.typeStruct,yyvsp[0].text,DEFAULT_TYPE_FLAG);
                yyval.typeStruct.is_raw = false;
            }
break;
case 94:
#line 1534 "parser.y"
{
                yyval.typeStruct.sval = yyvsp[-1].text; /* name*/
                fill_type_for_name((typeStruct_t*)&yyval.typeStruct,yyvsp[-1].text,DEFAULT_TYPE_FLAG);
                if(yyval.typeStruct.v_type != T_ERROR){
                    int dim = yyval.typeStruct.dim;
                    int expand = 0;
                    /*DEBUG//printf("-->prepare to expand array with dim : %d\n",$2.dim);*/
                    for(int i=0;i<yyvsp[0].typeStruct.dim;i++){
                        if(dim > 0){ /* can expand*/
                            /*DEBUG//printf("==>expand info level %d of %d\n",i,$2.dim);*/
                            dim--;
                            expand++;
                        }
                        else{
                            /*DEBUG//printf("==>error on expand level %d\n",i);*/
                            /* cannot expand furthur*/
                            char errmsg[MAX_STRING_SIZE];
                            snprintf(errmsg,MAX_STRING_SIZE,"Cannot use array notation for non-array type '%s'",TYPE_NAME[yyval.typeStruct.v_type]);
                            print_error(errmsg);
                            yyval.typeStruct.v_type = T_ERROR; 
                            break;
                        }
                    }
                    /* shift array if no error occur*/
                    if(yyval.typeStruct.v_type != T_ERROR){
                        for(int i=0;i<dim;i++){
                            yyval.typeStruct.dims[i] = yyval.typeStruct.dims[i+expand];
                        }
                        yyval.typeStruct.dim = dim;
                    }
                }
                yyval.typeStruct.is_const = false;
            }
break;
case 95:
#line 1570 "parser.y"
{
                        /*DEBUG//printf("int:Reduce singe_array to array\n");*/
                        /* merge index*/
                        yyval.typeStruct = yyvsp[0].typeStruct;
                        yyval.typeStruct.dims[yyval.typeStruct.dim] = yyvsp[-1].typeStruct.dims[0];
                        yyval.typeStruct.dim++;
                    }
break;
case 96:
#line 1578 "parser.y"
{
                    /*DEBUG//printf("int:Reduce singe to array\n");*/
                     
                }
break;
case 97:
#line 1584 "parser.y"
{
                                yyval.typeStruct.dim = 1;
                                /*DEBUG//printf("int:Reduce to single indice\n");    */
                                /* check if expr is integer and not an array*/
                                if(yyvsp[-1].typeStruct.v_type != T_INT){
                                    yyval.typeStruct.v_type = T_ERROR;
                                    if(yyvsp[-1].typeStruct.v_type != T_ERROR){
                                        /* new error*/
                                        char errmsg[MAX_STRING_SIZE];
                                        snprintf(errmsg,MAX_STRING_SIZE,"Array index must be scalar integer type");
                                        print_error(errmsg);

                                    }
                                }
                                else{
                                    /* furthur check if constant and the number < 0*/
                                    /*DEBUG//printf("index is const : %s , val is %d\n",$2.is_const ? "true" : "false",$2.val);*/
                                    if(yyvsp[-1].typeStruct.is_const && yyvsp[-1].typeStruct.val < 0){
                                        yyval.typeStruct.v_type = T_ERROR;
                                        print_error("Array index must greater than or equal to 0");
                                            
                                    }
                                    else{
                                        yyval.typeStruct.v_type = T_INT;
                                    }


                                }

                            }
break;
case 98:
#line 1619 "parser.y"
{
                yyval.typeStruct = yyvsp[0].typeStruct;
                /* check type consist*/
                if(yyval.typeStruct.v_type != T_BOOL){
                    yyval.typeStruct.v_type = T_ERROR;
                    if(yyvsp[0].typeStruct.v_type != T_ERROR){
                        /* old one is not error*/
                        char errmsg[MAX_STRING_SIZE];
                        snprintf(errmsg,MAX_STRING_SIZE,"Expect a boolean expression here, but '%s' provided",TYPE_NAME[yyvsp[0].typeStruct.v_type]);
                        print_error(errmsg);
                    }
                }
            }
break;
case 107:
#line 1647 "parser.y"
{
            yyval.typeStruct.v_type = binary_numeric_expr_check((typeStruct_t*)&yyvsp[-2].typeStruct,(typeStruct_t*)&yyvsp[0].typeStruct,OP_ADD);
            yyval.typeStruct.is_const = false;
        }
break;
case 108:
#line 1652 "parser.y"
{
            yyval.typeStruct.v_type = binary_numeric_expr_check((typeStruct_t*)&yyvsp[-2].typeStruct,(typeStruct_t*)&yyvsp[0].typeStruct,OP_SUB);
            yyval.typeStruct.is_const = false;
        }
break;
case 109:
#line 1657 "parser.y"
{
            yyval.typeStruct.v_type = binary_numeric_expr_check((typeStruct_t*)&yyvsp[-2].typeStruct,(typeStruct_t*)&yyvsp[0].typeStruct,OP_MUL);
            yyval.typeStruct.is_const = false;
        }
break;
case 110:
#line 1662 "parser.y"
{
            yyval.typeStruct.v_type = binary_numeric_expr_check((typeStruct_t*)&yyvsp[-2].typeStruct,(typeStruct_t*)&yyvsp[0].typeStruct,OP_DIV);
            yyval.typeStruct.is_const = false;
        }
break;
case 111:
#line 1667 "parser.y"
{
            yyval.typeStruct.v_type = binary_numeric_expr_check((typeStruct_t*)&yyvsp[-2].typeStruct,(typeStruct_t*)&yyvsp[0].typeStruct,OP_MOD);
            yyval.typeStruct.is_const = false;
        }
break;
case 112:
#line 1671 "parser.y"
{
            /*DEBUG//printf("Reduce with negation operator, original const is %s\n",$2.is_const ?  "true" : "false");*/
            yyval.typeStruct = yyvsp[0].typeStruct;
            /* check type is numeric or not*/
            if(!isNumericType(yyvsp[0].typeStruct.v_type)){
                yyval.typeStruct.v_type = T_ERROR;
                if(yyvsp[0].typeStruct.v_type != T_ERROR){    
                    char errmsg[MAX_STRING_SIZE];
                    snprintf(errmsg,MAX_STRING_SIZE,"Only numeric expression can use negation '-', but '%s' provided.",TYPE_NAME[yyvsp[0].typeStruct.v_type]);
                    print_error(errmsg);
                }
                else{
                    /*DEBUG//printf("Previous error occur on operand for Negation Operator\n");*/
                }
            }
            if(yyval.typeStruct.is_const){
                perform_negation((typeStruct_t*)&yyval.typeStruct);    
            }

        }
break;
case 113:
#line 1691 "parser.y"
{ yyval.typeStruct = yyvsp[-1].typeStruct; }
break;
case 114:
#line 1692 "parser.y"
{
            yyval.typeStruct.v_type = binary_numeric_expr_check((typeStruct_t*)&yyvsp[-2].typeStruct,(typeStruct_t*)&yyvsp[0].typeStruct,OP_RELATION);
            yyval.typeStruct.is_const = false;
        }
break;
case 115:
#line 1696 "parser.y"
{
        /* both side need to be boolean type*/
        if(yyvsp[-2].typeStruct.v_type != T_BOOL){ /* p1 error*/
            yyval.typeStruct.v_type = T_ERROR;     
            if(yyvsp[-2].typeStruct.v_type != T_ERROR){
                /* if previous has no error, means this time has a convert error*/
                char errmsg[MAX_STRING_SIZE];
                snprintf(errmsg,MAX_STRING_SIZE,"Binary logical operator expect a boolean operand on left-hand-side, but '%s' provided.",TYPE_NAME[yyvsp[-2].typeStruct.v_type]);
                print_error(errmsg);
            }
            else{
                /*DEBUG//printf("Previous error occur on operand for Logical Operator\n");*/
            }
        }
        else if(yyvsp[0].typeStruct.v_type != T_BOOL){
            yyval.typeStruct.v_type = T_ERROR;    
            if(yyvsp[0].typeStruct.v_type != T_ERROR){
                /* if previous has no error, means this time has a convert error*/
                char errmsg[MAX_STRING_SIZE];
                snprintf(errmsg,MAX_STRING_SIZE,"Binary logical operator expect a boolean operand on right-hand-side, but '%s' provided.",TYPE_NAME[yyvsp[0].typeStruct.v_type]);
                print_error(errmsg);
            }
            else{
                printf("Previous error occur on operand for Logical Operator\n");
            }
            
        }
        else{
             /* success type, assign it */
            yyval.typeStruct = yyvsp[-2].typeStruct;
        }
        yyval.typeStruct.is_const = false;
     }
break;
case 116:
#line 1729 "parser.y"
{
        yyval.typeStruct = yyvsp[0].typeStruct; /* important, assign result */
        /* check if type is not bool*/
        if(yyval.typeStruct.v_type != T_BOOL){
            if(yyval.typeStruct.v_type != T_ERROR){
                /* if previous has no error, means this time has a convert error*/
                char errmsg[MAX_STRING_SIZE];
                snprintf(errmsg,MAX_STRING_SIZE,"Logical operator NOT:'!' expect a boolean operand, but '%s' provided.",TYPE_NAME[yyval.typeStruct.v_type]);
                print_error(errmsg);
            }
            else{
                /*DEBUG//printf("Previous error occur on operand for Logical NOT\n");*/
            }
            yyval.typeStruct.v_type = T_ERROR;
        }
        yyval.typeStruct.is_const = false;
     }
break;
case 120:
#line 1753 "parser.y"
{
                /* check function consistency*/
                int ret_type = check_function_call_consist(yyvsp[-3].text,&yyvsp[-1].typeList);
                if(ret_type>=0){
                    /*DEBUG//printf("-->function with return type '%s'called\n",TYPE_NAME[ret_type]);  */
                }
                else{
                    /*DEBUG//printf("Not found\n");    */
                }
                /* set typeStruct*/
                yyval.typeStruct.sval = yyvsp[-3].text;
                yyval.typeStruct.v_type = ret_type;
                yyval.typeStruct.is_const = false;

           }
break;
case 122:
#line 1772 "parser.y"
{
                            /* empty, build empty list*/
                            yyval.typeList.end = 0;
                        }
break;
case 123:
#line 1780 "parser.y"
{
                /* build from single expr*/
                yyval.typeList.end = 1;
                yyval.typeList.data[0] = yyvsp[0].typeStruct;
            }
break;
case 124:
#line 1786 "parser.y"
{
                /* copy list*/
                yyval.typeList = yyvsp[-2].typeList;
                /* concat list*/
                yyval.typeList.data[yyval.typeList.end] = yyvsp[0].typeStruct;
                yyval.typeList.end++;
            }
break;
#line 2575 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
