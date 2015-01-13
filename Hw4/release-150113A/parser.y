%{
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
                ++nextVarNum;
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
%}

 /* error : expression has type < 0, means there is an error */
%union {
    int value;
    char* text;
    int type;
    typeStruct_t typeStruct;
    typeList_t typeList;
}


%nonassoc <type> INT BOOL FLOAT DOUBLE STRING CONST VOID
%nonassoc END

%nonassoc LESS_THAN_ELSE
%nonassoc KW_ELSE

%nonassoc ARITHMETIC_PARENTHESES
%nonassoc LOWER_THEN_ARITHMETIC_PARENTHESES
%nonassoc '(' ')'


%left L_OP_OR
%left L_OP_AND
%left L_OP_NOT
%nonassoc L_OP_LT L_OP_LE L_OP_EQ L_OP_GE L_OP_GT L_OP_NE




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
%token <text> ID           /* identifier */
%token INT          /* keyword */
%token BOOL
%token FLOAT
%token DOUBLE
%token STRING
%token ASSIGN
%token COMMA
%token KW_PRINT
%token KW_READ



%token <typeStruct> INTEGER_CONSTANT
%token <typeStruct> FLOATING_CONSTANT
%token <typeStruct> BOOLEAN_CONSTANT
%token <typeStruct> STRING_CONSTANT
%token VOID
%token CONST


%type <text> scalar_id identifier
%type <type> var_decl type var_list var_entry
%type <typeStruct> literal_constant single_arg
%type <typeList> args arg_list 
%type <typeStruct> expr funct_call var_ref bool_expr
%type <typeStruct> intexpr_array_indice array_indice
%type <typeStruct> intexpr_single_array_indice  single_array_indice
%type <typeList> expr_list zero_or_more_expr_list 
%type <value> binary_logic_op L_OP_OR L_OP_AND L_OP_NOT
%type <value> relation_op L_OP_GT L_OP_GE L_OP_NE L_OP_EQ L_OP_LT L_OP_LE 


%%

program : declaration_list funct_def decl_and_def_list  {
            //DEBUG//printf("Reduce to program\n");
            program_reduce_process();
            }  
	| funct_def decl_and_def_list {
            program_reduce_process();
	    }
	;

decl_and_def_list	: decl_and_def_list declaration_list %prec END  
			| decl_and_def_list definition_list %prec END 
                        |
                        ;

declaration_list : declaration_list const_decl 
                 | declaration_list var_decl 
                 | declaration_list funct_decl 
                 | const_decl 
                 | var_decl 
                 | funct_decl 
		 ;

var_decl : type var_list  SEMICOLON
         ;

type : INT {tmp_type_init_scalar($1);$$=$1;}
     | BOOL {tmp_type_init_scalar($1);$$=$1;}
     | DOUBLE {tmp_type_init_scalar($1);$$=$1;} 
     | FLOAT {tmp_type_init_scalar($1);$$=$1;} 
     | STRING {tmp_type_init_scalar($1);$$=$1;} 
     | VOID {tmp_type_init_scalar($1);$$=$1;} 
     ; 

 /* scalar id or arrray id */
identifier : scalar_id 
           | scalar_id array_indice 
	   ;
 /* A list of variable entry, one or more */

var_list : var_entry
         | var_entry COMMA var_list
         ;
 /* Variable Entry, either "identifier" or "identifier = decl_expr" */
var_entry : identifier {
                // create basic
                create_basic_entry($1,K_VARIABLE);
          }
          | scalar_id ASSIGN expr{
                // create basic
                bool old_check = coercion_check_only;
                coercion_check_only = true;
                if($3.v_type >= 0){
                    //DEBUG//printf("Declared '%s' %s with expr type : %s\n",$1,TYPE_NAME[typeTmp.v_type],TYPE_NAME[$3.v_type]);
                    if(do_type_coercion(&typeTmp,&$3)){
                        //DEBUG//printf("==>Coercion pass!\n");
                        create_basic_entry($1,K_VARIABLE);
                        // assembly assignment , create fake and fill name
                        typeStruct_t fake = typeTmp;
                        fake.sval = $1;
                        asmGenAssignment(&fake,&$3);
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
          | scalar_id array_indice ASSIGN '{' zero_or_more_expr_list '}'{ 
                // check type list match
                if(check_array_initializer(&typeTmp,&$5)){    
                    // create basic
                    create_basic_entry($1,K_VARIABLE);
                }
          }
          ;



 /* Pure ID */
scalar_id : ID { $$ = $1; }
          ;




array_indice : single_array_indice array_indice 
             | single_array_indice 
             ;
single_array_indice : '[' INTEGER_CONSTANT ']' {
                        //DEBUG//printf("Reduce to single indice, dim = %d\n",$2.val);
                        tmp_type_add_dim($2.val);
                        }
                    ;


 /* constant variable */
const_decl : CONST type const_list SEMICOLON
           ;

const_list : single_const
           | single_const COMMA const_list
           ;

single_const : scalar_id ASSIGN literal_constant {
                // normal positive ALSO NEED COERCION!!!!
                typeStruct_t type;
                memcpy(&type,&$3,sizeof(type));
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
                    process_constant_declare($1,type);
                }
             } 
             | scalar_id ASSIGN '-' literal_constant %prec '*'{
                typeStruct_t type;
                memcpy(&type,&$4,sizeof(type));
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
                        process_constant_declare($1,type);
                    }
                }
             }
             ;


literal_constant : INTEGER_CONSTANT
                 | FLOATING_CONSTANT
                 | BOOLEAN_CONSTANT
                 | STRING_CONSTANT
                 ;



 /* function declaration */
funct_decl : type scalar_id '(' args ')' SEMICOLON {
                    //push_table(TABLE_PRETEND);
                    //DEBUG//printf("Reduce  to funct_decl \n");
                    // add function declar entry
                    // add basic entry, check mode is function declare
                    init_funcOption();
                    funcOption.isDeclare = true;
                    funcOption.retType = $1;
                    s_table_entry* ent = create_basic_entry($2,K_FUNCTION);
                    if(ent){
                        // set type
                        ent->type.v_type = $1;
                        // add attribute
                        ent->attr.param_list.end = 0;
                        for(int i=0;i<$4.end;i++){
                            typeStruct_t* pTarget = &ent->attr.param_list.data[ent->attr.param_list.end];
                            ++ent->attr.param_list.end;
                            // type
                            pTarget->v_type = $4.data[i].v_type;
                            // dim
                            pTarget->dim = $4.data[i].dim;
                            // dims
                            memcpy(pTarget->dims,$4.data[i].dims,sizeof(pTarget->dims));
                        }
                    }


                }
           ;



 /* definition list */
definition_list : definition_list funct_def 
                | funct_def 
                ;



 /* function definition */
funct_def : type scalar_id '(' args ')'  {
                    // clear variable number , for main, need to plus 1
                    nextVarNum = strcmp($2,"main")==0 ? INIT_VAR_NUM + 1 : INIT_VAR_NUM ;
                    paramVarNum = nextVarNum;
                    
                    // add function declar entry
                    // add basic entry, check mode is function declare
                    init_funcOption();
                    funcOption.isDeclare = false;
                    funcOption.name = $2;
                    funcOption.isMain = strcmp($2,"main")==0;
                    typeList_t* pTypeList = &$4;
                    funcOption.pTypeList = &$4;
                    for(int i=0;i<pTypeList->end;i++){
                        //DEBUG//printf("list[%d] %s , dim:%d\n",i,TYPE_NAME[$4.data[i].v_type],$4.data[i].dim);
                    }
                    funcOption.retType = $1;
                    //DEBUG//printf("Reduce to funct_def , has type %s\n",TYPE_NAME[$1]); 
                    s_table_entry* ent = create_basic_entry($2,K_FUNCTION);
                    // add pretend table after function entry added
                    // scope : see it is void or non-void
                    // if entry has an error, we will take current local type as real type
                    int effective_type;
                    if(!ent){ 
                        effective_type = $1;    
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
                            nextVarNum++; // var num is for parameter

                        } 
                        // genrate function head Assembly
                        asmGenFunctionHead($2);
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
                            }
                            else{
                            
                                //DEBUG//printf("==>Param add FAILURE\n");
                            }
                        }
                    }
                    
                } compound_stat 
                {
                    // check last stmt
                    if(lastStmtType != STMT_COMPOUND_RETURN && $1!=T_VOID){
                        print_error("Last statment of non-void function must be a 'return' statment");
                    }
                    asmGenFunctionTail();
                    funcOption.retType = T_ERROR;
                }
          ;
 

 /* Arguments, zero or more */
args :  arg_list
     |  /* empty list */ {     
            $$.end = 0;
         } 
     ;
 /* Single argument */
single_arg :  type identifier {
                    //DEBUG//printf("Reduce to single_arg\n");
                    $$.v_type = typeTmp.v_type;
                    // copy dim info
                    $$.dim = typeTmp.dim;
                    memcpy(&$$.dims,&typeTmp.dims,sizeof($$.dims));
                    tmp_type_clear_array_indice();
                    $$.is_const = FALSE;
                    $$.sval = $2;
                }
           |  CONST type identifier {
                    //DEBUG//printf("Reduce to const single_arg\n");
                    $$.v_type = typeTmp.v_type;
                    // copy dim info
                    $$.dim = typeTmp.dim;
                    memcpy(&$$.dims,&typeTmp.dims,sizeof($$.dims));
                    tmp_type_clear_array_indice();
                    $$.is_const = TRUE; 
                    $$.sval = $3;
                    }
           ;
 /* A list of args */
arg_list : arg_list COMMA single_arg {
                //DEBUG//printf("Reduce single_arg,arg_list to arg_list\n");
                // copy old one
                memcpy(&$$,&$1,sizeof($$));
                // merge new one
                $$.data[$$.end].is_const = $3.is_const;
                $$.data[$$.end].v_type = $3.v_type;
                $$.data[$$.end].dim = $3.dim;
                memcpy(&$$.data[$$.end].dims,&$3.dims,sizeof($$.data[$$.end].dims));
                $$.data[$$.end].sval = $3.sval;
                $$.end += 1;
                // create parameter entry
                //create_basic_entry($1.sval,K_PARAMETER); 
                }
         | single_arg     {
                //DEBUG//printf("Reduce single_arg to arg_list\n");
                $$.end = 1;
                $$.data[0].is_const = $1.is_const;
                $$.data[0].v_type = $1.v_type;
                $$.data[0].dim = $1.dim;
                memcpy(&$$.data[0].dims,&$1.dims,sizeof($$.data[0].dims));
                // name info
                $$.data[0].sval = $1.sval;
                // create parameter entry
                //create_basic_entry($1.sval,K_PARAMETER);
            }
         ;

 /* compound statment */
compound_stat : '{' {push_table(TABLE_NORMAL);} local_decl_list zero_or_more_stat_list '}' {
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

stat : compound_stat // doing in deeper 
     | simple_stat  {lastStmtType = STMT_NORMAL;}
     | if_stat  {lastStmtType = STMT_NORMAL;}
     | while_stat  {lastStmtType = STMT_NORMAL;}
     | for_stat  {lastStmtType = STMT_NORMAL;}
     | jump_stat // doing in deeper
     | funct_call SEMICOLON { 
        asmGenFunctionCall($1.sval,true); // discard return value if any
     }
     ;

 /* if-statment */


if_stat : KW_IF '(' bool_expr ')'  {pushLabel();asmGenIfHead();} compound_stat else_part
        ;
else_part : KW_ELSE {asmGenIfGoExit();asmGenIfFalse();} compound_stat {asmGenIfExit();popLabel();} 
          | %prec LESS_THAN_ELSE {asmGenIfFalse();popLabel();}
          ;

/*
if_stat : if_only_stat
        | if_else_stat 
        ;


if_only_stat : KW_IF '(' bool_expr ')' compound_stat %prec LESS_THAN_ELSE
             ;

if_else_stat : KW_IF '(' bool_expr ')' compound_stat KW_ELSE compound_stat 
             ;

*/
 /* while-statment */

while_stat : KW_WHILE {go_to_loop_scope(LOOP_WHILE);pushLabel();asmGenWhileHead();} '(' bool_expr ')' {asmGenWhileGoExit();} compound_stat {asmGenWhileTail();popLabel();}   
           | KW_DO {go_to_loop_scope(LOOP_DO_WHILE);pushLabel();asmGenDoWhileHead();} compound_stat KW_WHILE '(' {asmGenDoWhileTest();} bool_expr ')' {asmGenDoWhileTail();popLabel();} SEMICOLON 
           ;


 /* For-statment */
for_stat : KW_FOR   {go_to_loop_scope(LOOP_FOR);} '(' for_expr SEMICOLON {pushLabel();asmGenForBegin();} for_bool_expr SEMICOLON {asmGenForGotoTrue();asmGenForIncrease();} for_expr {asmGenForGotoBegin();} ')' {asmGenForTrue();} compound_stat {asmGenForTail();popLabel();}
         ;

for_bool_expr : bool_expr 
              |
              ;

for_expr : expr
         | one_or_more_for_assignment 
         | 
         ;

single_assginment : var_ref ASSIGN expr
                        {
                            asmGenAssignment(&$1,&$3);


                        }
                  ;

one_or_more_for_assignment : single_assginment
                           | one_or_more_for_assignment COMMA single_assginment
                           ;

 /* Jump statment */
jump_stat : KW_RETURN expr SEMICOLON {
                    process_return_function_check(&$2);
                    lastStmtType = STMT_RETURN;
                    if(!funcOption.isMain){
                        asmGenFunctionReturn(funcOption.retType,$2.v_type); 
                    }
                }
          | KW_BREAK SEMICOLON {
                    process_loop_check(); 
                    lastStmtType = STMT_NORMAL;
                    asmGenLoopControl(LOOP_CTRL_BREAK);
                }
          | KW_CONTINUE SEMICOLON {
                    process_loop_check();
                    lastStmtType = STMT_NORMAL;
                    asmGenLoopControl(LOOP_CTRL_CONTINUE);
                }
          ;

 /* Simple statment */
simple_stat : single_assginment SEMICOLON 
            | KW_PRINT {asmGenPrintHead();} expr SEMICOLON {
                    check_printable(&$3);
                    asmGenPrintTail(&$3);
                    
                }
            | KW_READ {asmGenReadHead();} var_ref SEMICOLON { 
                    check_readable(&$3); 
                    asmGenReadTail(&$3);
                    
                } 
            ;

var_ref : scalar_id 
            {
                $$.sval = $1; // name
                fill_type_for_name((typeStruct_t*)&$$,$1,DEFAULT_TYPE_FLAG);
                $$.is_raw = false;
            }
        | scalar_id intexpr_array_indice
            {
                $$.sval = $1; // name
                fill_type_for_name((typeStruct_t*)&$$,$1,DEFAULT_TYPE_FLAG);
                if($$.v_type != T_ERROR){
                    int dim = $$.dim;
                    int expand = 0;
                    //DEBUG//printf("-->prepare to expand array with dim : %d\n",$2.dim);
                    for(int i=0;i<$2.dim;i++){
                        if(dim > 0){ // can expand
                            //DEBUG//printf("==>expand info level %d of %d\n",i,$2.dim);
                            dim--;
                            expand++;
                        }
                        else{
                            //DEBUG//printf("==>error on expand level %d\n",i);
                            // cannot expand furthur
                            char errmsg[MAX_STRING_SIZE];
                            snprintf(errmsg,MAX_STRING_SIZE,"Cannot use array notation for non-array type '%s'",TYPE_NAME[$$.v_type]);
                            print_error(errmsg);
                            $$.v_type = T_ERROR; 
                            break;
                        }
                    }
                    // shift array if no error occur
                    if($$.v_type != T_ERROR){
                        for(int i=0;i<dim;i++){
                            $$.dims[i] = $$.dims[i+expand];
                        }
                        $$.dim = dim;
                    }
                }
                $$.is_const = false;
            }
        ;

intexpr_array_indice : intexpr_single_array_indice intexpr_array_indice 
                    {
                        //DEBUG//printf("int:Reduce singe_array to array\n");
                        // merge index
                        $$ = $2;
                        $$.dims[$$.dim] = $1.dims[0];
                        $$.dim++;
                    }
             | intexpr_single_array_indice 
                {
                    //DEBUG//printf("int:Reduce singe to array\n");
                     
                }
             ;
intexpr_single_array_indice : '[' expr ']' 
                            {
                                $$.dim = 1;
                                //DEBUG//printf("int:Reduce to single indice\n");    
                                // check if expr is integer and not an array
                                if($2.v_type != T_INT){
                                    $$.v_type = T_ERROR;
                                    if($2.v_type != T_ERROR){
                                        // new error
                                        char errmsg[MAX_STRING_SIZE];
                                        snprintf(errmsg,MAX_STRING_SIZE,"Array index must be scalar integer type");
                                        print_error(errmsg);

                                    }
                                }
                                else{
                                    // furthur check if constant and the number < 0
                                    //DEBUG//printf("index is const : %s , val is %d\n",$2.is_const ? "true" : "false",$2.val);
                                    if($2.is_const && $2.val < 0){
                                        $$.v_type = T_ERROR;
                                        print_error("Array index must greater than or equal to 0");
                                            
                                    }
                                    else{
                                        $$.v_type = T_INT;
                                    }


                                }

                            }
                    ;



bool_expr : expr
            {
                $$ = $1;
                // check type consist
                if($$.v_type != T_BOOL){
                    $$.v_type = T_ERROR;
                    if($1.v_type != T_ERROR){
                        // old one is not error
                        char errmsg[MAX_STRING_SIZE];
                        snprintf(errmsg,MAX_STRING_SIZE,"Expect a boolean expression here, but '%s' provided",TYPE_NAME[$1.v_type]);
                        print_error(errmsg);
                    }
                }
            }
          ;

relation_op : L_OP_LT
            | L_OP_LE
            | L_OP_GT
            | L_OP_GE
            | L_OP_NE
            | L_OP_EQ
            ;

binary_logic_op : L_OP_AND
                | L_OP_OR
                ;

expr : expr '+' expr 
        {
            asmGenBinaryExpression(&$$,&$1,&$3,OP_ADD);
        }

     | expr '-' expr
        {
            asmGenBinaryExpression(&$$,&$1,&$3,OP_SUB);
        }
     | expr '*' expr
        {
            asmGenBinaryExpression(&$$,&$1,&$3,OP_MUL);
        }
     | expr '/' expr
        {
            asmGenBinaryExpression(&$$,&$1,&$3,OP_DIV);
        }
     | expr '%' expr
        {
            asmGenBinaryExpression(&$$,&$1,&$3,OP_MOD);
        }
     | '-' expr %prec '*' {
            //DEBUG//printf("Reduce with negation operator, original const is %s\n",$2.is_const ?  "true" : "false");
            $$ = $2;
            // check type is numeric or not
            if(!isNumericType($2.v_type)){
                $$.v_type = T_ERROR;
                if($2.v_type != T_ERROR){    
                    char errmsg[MAX_STRING_SIZE];
                    snprintf(errmsg,MAX_STRING_SIZE,"Only numeric expression can use negation '-', but '%s' provided.",TYPE_NAME[$2.v_type]);
                    print_error(errmsg);
                }
                else{
                    //DEBUG//printf("Previous error occur on operand for Negation Operator\n");
                }
            }
            if($$.is_const){
                perform_negation((typeStruct_t*)&$$);    
            }
            // make negation assembly
            asmGenOperator(&$$,OP_NEG);
    
        }
     | '(' expr ')' { $$ = $2; }
     | expr relation_op expr %prec L_OP_EQ{
            asmGenBinaryExpression(&$$,&$1,&$3,$2);
        }
     | expr binary_logic_op expr %prec L_OP_AND {
        // both side need to be boolean type
        if($1.v_type != T_BOOL){ // p1 error
            $$.v_type = T_ERROR;     
            if($1.v_type != T_ERROR){
                // if previous has no error, means this time has a convert error
                char errmsg[MAX_STRING_SIZE];
                snprintf(errmsg,MAX_STRING_SIZE,"Binary logical operator expect a boolean operand on left-hand-side, but '%s' provided.",TYPE_NAME[$1.v_type]);
                print_error(errmsg);
            }
            else{
                //DEBUG//printf("Previous error occur on operand for Logical Operator\n");
            }
        }
        else if($3.v_type != T_BOOL){
            $$.v_type = T_ERROR;    
            if($3.v_type != T_ERROR){
                // if previous has no error, means this time has a convert error
                char errmsg[MAX_STRING_SIZE];
                snprintf(errmsg,MAX_STRING_SIZE,"Binary logical operator expect a boolean operand on right-hand-side, but '%s' provided.",TYPE_NAME[$3.v_type]);
                print_error(errmsg);
            }
            else{
                printf("Previous error occur on operand for Logical Operator\n");
            }
            
        }
        else{
             // success type, assign it 
            $$ = $1;
            // this time, no type coercion needed
            asmGenOperator(&$$,$2);
        }
        $$.is_const = false;
     }
     | L_OP_NOT expr {
        $$ = $2; // important, assign result 
        // check if type is not bool
        if($$.v_type != T_BOOL){
            if($$.v_type != T_ERROR){
                // if previous has no error, means this time has a convert error
                char errmsg[MAX_STRING_SIZE];
                snprintf(errmsg,MAX_STRING_SIZE,"Logical operator NOT:'!' expect a boolean operand, but '%s' provided.",TYPE_NAME[$$.v_type]);
                print_error(errmsg);
            }
            else{
                //DEBUG//printf("Previous error occur on operand for Logical NOT\n");
            }
            $$.v_type = T_ERROR;
        }
        $$.is_const = false;
        // assembly generate, NOT is equal to "XOR with true"
        // push a true
        fprintf(asmOut,"    iconst_1\n");
        asmGenOperator(&$$,$1);
     }
     | literal_constant {
        // load the value to the top of stack
        asmGenLiteral(&$1);
     }
     | funct_call {
        asmGenFunctionCall($1.sval,false);
     }
     | var_ref {
        // the operand stack will load this variable
        //DEBUG//printf("Reference to Variable %s\n",$1.sval);
        s_table_entry* ent = findVarEntry($1.sval);
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
     ;



funct_call : scalar_id '(' zero_or_more_expr_list ')' {
                // check function consistency
                int ret_type = check_function_call_consist($1,&$3);
                if(ret_type>=0){
                    //DEBUG//printf("-->function with return type '%s'called\n",TYPE_NAME[ret_type]);  
                }
                else{
                    //DEBUG//printf("Not found\n");    
                }
                // set typeStruct
                $$.sval = $1;
                $$.v_type = ret_type;
                $$.is_const = false;

           }
           ;

zero_or_more_expr_list :  expr_list  // keep
                       | 
                        {
                            // empty, build empty list
                            $$.end = 0;
                        }
                       ;

 /* one or more expr */
expr_list : expr 
            {
                // build from single expr
                $$.end = 1;
                $$.data[0] = $1;
            }
          | expr_list COMMA expr
            {
                // copy list
                $$ = $1;
                // concat list
                $$.data[$$.end] = $3;
                $$.end++;
            }
          ;


%%

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

