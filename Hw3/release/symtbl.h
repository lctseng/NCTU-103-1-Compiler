#include <stdbool.h>

//#define SHOW_ALL_ERROR
#ifndef ERR_FD
#define ERR_FD stderr
#endif

#define MAX_TABLE_ENTRY 100
#define MAX_TABLES 10
#define MAX_DIM 10
#define MAX_PARAM 15
#define MAX_STRING_SIZE 1024
#define MAX_STRING_POOL 4096
#define MAX_ERROR_MSG 4096

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


bool check_typeList_consist(typeList_t* a,typeList_t* b,bool allow_coercion);
void init_typeStruct(typeStruct_t*);
