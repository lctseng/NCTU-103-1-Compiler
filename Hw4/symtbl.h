#include <stdbool.h>

//#define SHOW_ALL_ERROR
#ifndef ERR_FD
#define ERR_FD stderr
#endif

#define OUT_NAME "output"
#define SOURCE_IN_ASSEMBLY

#define MAX_TABLE_ENTRY 100
#define MAX_TABLES 10
#define MAX_DIM 10
#define MAX_PARAM 15
#define MAX_STRING_SIZE 1024
#define MAX_STRING_POOL 4096
#define MAX_ERROR_MSG 4096
#define MAX_JAVA_STACK 110
#define MAX_JAVA_LOCAL 110 
#define MAX_LABEL_STACK 1024
#define MAX_FUNCTION_CALL 100
#define MAX_GLOBAL_QUEUE_LENGTH 9126

#define VN_START (MAX_JAVA_LOCAL - 10)
#define VN_TMP1 (VN_START + 1)
#define VN_TMP2 (VN_START + 2)
#define VN_TMP3 (VN_START + 3)
#define VN_TMP4 (VN_START + 4)
#define INIT_VAR_NUM 0

#define TRUE 1
#define FALSE 0

#define TABLE_NORMAL 1
#define TABLE_PRETEND 2

#define ENTRY_NOT_FOUND 1
#define ENTRY_MISMATCH 2

#define T_ERROR -1
#define T_INT 0
#define T_FLOAT 1
#define T_DOUBLE 2
#define T_BOOL 3
#define T_STRING 4
#define T_VOID 5
#define K_FUNCTION 1
#define K_PARAMETER 2
#define K_VARIABLE 4
#define K_CONSTANT 8

#define DEFAULT_TYPE_FLAG (K_CONSTANT|K_PARAMETER|K_VARIABLE)


#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4
#define OP_MOD 5
#define OP_RELATION 6
#define OP_NEG 7
#define OP_AND 8
#define OP_OR 9
#define OP_NOT 10
#define OP_GT 11
#define OP_GE 12
#define OP_EQ 13
#define OP_NE 14
#define OP_LT 15
#define OP_LE 16

#define LOOP_NONE 0
#define LOOP_WHILE 1
#define LOOP_DO_WHILE 2
#define LOOP_FOR 3

#define LOOP_CTRL_BREAK 1
#define LOOP_CTRL_CONTINUE 2

#define SCOPE_ERROR 1 // error settings
#define SCOPE_NORMAL 2 // no return,break,continue : GLOBAL
#define SCOPE_FUNCTION 4 // no break,continue : FUNCTION
#define SCOPE_LOOP 8 // WHILE FOR

#define STMT_ERROR 0
#define STMT_NORMAL 1
#define STMT_RETURN 2
#define STMT_COMPOUND_RETURN 3


typedef struct {
    int v_type;
    int val;
    float fval;
    double dval;
    char* sval;
    bool is_const;
    int dim;
    int dims[10];
    bool is_raw; // is it a raw floating? it can be float or double
} typeStruct_t;
// a list of types
typedef struct {
    int end;
    typeStruct_t data[MAX_PARAM];
} typeList_t;


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
    int varNum; // valid only when is a variable or parameter
} s_table_entry;

// symbol table
typedef struct{
    s_table_entry entries[MAX_TABLE_ENTRY];
    int end;
    int level;
    int scopeType;
    int loopType;
    int loopLabelNumber;
  
} symbol_table;

// table stack
typedef struct{
    symbol_table tables[MAX_TABLES];
    int end;
    int level;
    int pretend;
} table_stack;

// label stack
typedef struct{
    int end;
    int data[MAX_LABEL_STACK];
} stack_t;

// label mgnt
typedef struct{
    stack_t stk;
    int count;
} labelMgmt_t;
// function call struct
typedef struct{
    typeList_t list;
    int index;
} funcCall_t;
// function call stack
typedef struct{
    int end;
    funcCall_t data[MAX_FUNCTION_CALL];
} funcCallStk_t;




typedef bool (*EntryChecker)(s_table_entry*);
// functions
bool check_typeList_consist(typeList_t* a,typeList_t* b,bool allow_coercion);
void init_typeStruct(typeStruct_t*);

// initial a labelMgmt_t
void initLabelMgmt(labelMgmt_t* pMgmt);
// find entry by name
s_table_entry* findEntryByName(const char* name,EntryChecker);
// find the var entry of the variable, search outward to global
s_table_entry* findVarEntry(const char* name);
// find function entry
s_table_entry* findFuncEntry(const char* name);
// check typeStruct is not NULL and not T_ERROR
bool checkTypeStructSanity(typeStruct_t* pType);
// fill the content in typeStruct accroding to sym_attr 
void fillTypeStructWithSymAttr(typeStruct_t* pType,sym_attr* pAttr);
// push label
void pushLabel();
// pop label
int popLabel();
// get top label
int topLabel();
// entry checker
bool entryIsVar(s_table_entry* ent);
bool entryIsFunction(s_table_entry* ent);
// init funcCall stack
void initFuncCallStk();
void pushFuncCall(const char* name);
void popFuncCall();

//*** assembly related ***//
void asmQueueGlobal(const char* str);
void asmGlobalWrapper(const char* str);
void asmGenProgramHead();
void asmGenScanner();
void asmGenFunctionHead(const char* name);
void asmGenFunctionTail();
void asmGenPrintHead();
// generate code for literal constant expressed in typeStruct_t
void asmGenLiteral(typeStruct_t* pConst);
// typeStruct specfic the print type
void asmGenPrintTail(typeStruct_t* pType);
void asmGenReadHead();
// typeStruct specfic the read type
void asmGenReadTail(typeStruct_t* pType);
// generate corecion code if needed
void asmGenCoercion(int dstType,int srcType);
// store entry 
void asmGenStoreEntry(s_table_entry* ent);
// load stack into local variable
void asmGenLoadStackToLocal(typeStruct_t* pType,int varNum);
// store local to stack
void asmGenStoreLocalToStack(typeStruct_t* pType,int varNum);
// gen operation code , given type and operator
void asmGenOperator(typeStruct_t* pType,int op);
// gen relation operator
void asmGenRelationalOperator(int cmpType,int op);
// gen binary expression with coercion
void asmGenBinaryExpression(typeStruct_t* pDst,typeStruct_t* pSrc1,typeStruct_t* pSrc2,int op_type);
// gen variable assignment
void asmGenAssignment(typeStruct_t* pDst,typeStruct_t* pSrc);
// generate if-related head
void asmGenIfHead();
// code for goto exit IF
void asmGenIfGoExit();
// code for generate label false location
void asmGenIfFalse();
// code for generate label exit location
void asmGenIfExit();
// generate while-related head
void asmGenWhileHead();
// code for goto exit WHILE
void asmGenWhileGoExit();
// code for generate label exit location
void asmGenWhileExit();
void asmGenWhileTail();
void asmGenWhileGotoBegin();
// generate do-while-related head
void asmGenDoWhileHead();
void asmGenDoWhileTail();
void asmGenDoWhileTest(); // test for boolean label
// for loop
// begin of bool expr, begin label
void asmGenForBegin();
// end of increase , goto begin
void asmGenForGotoBegin();
// end of bool expr , goto true or goto exit
void asmGenForGotoTrue();
// begin of increase label
void asmGenForIncrease();
// end of statments, goto increase ; build end label 
void asmGenForTail();
// true label
void asmGenForTrue();
// code for break/continue appear in loop
void asmGenLoopControl(int ctrl);
// code for break in loop table
void asmGenWhileBreak(symbol_table* pTable);
void asmGenDoWhileBreak(symbol_table* pTable);
void asmGenForBreak(symbol_table* pTable);
// code for Continue in loop table
void asmGenWhileContinue(symbol_table* pTable);
void asmGenDoWhileContinue(symbol_table* pTable);
void asmGenForContinue(symbol_table* pTable);
// function return code, will do coercion for returned expression
void asmGenFunctionReturn(int dstType,int srcType);
// function call, option is "discard return value"
void asmGenFunctionCall(const char* name,bool discard);
// function args type generate by typeList
void asmGenFunctionTypebyList(typeList_t* pList);
void asmGenFunctionCallCoercion(typeStruct_t* pType);
