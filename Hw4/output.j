.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
; Line #1: // print hello world

; Line #2: #pragma source off

; Line #3: #pragma token off

; Line #4: #pragma statistic off

; Line #5: #pragma symbol on

; Line #6: 

; Line #7: 

.field public static a I
.field public static b I
; Line #8: int a, b;

.field public static d Z
; Line #9: bool d;

; Line #10: 

.method public static bar()Z
.limit stack 110
.limit locals 110
; Line #11: bool bar(){

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "BAR!!
"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #12:     print "BAR!!\n";

    iconst_1
    iconst_0
    iand
    ldc 5
    ldc 10
    istore 101
    iload 101
    isub
    ifgt LABEL_BOOLEXPR_TRUE_0
    iconst_0 ; false = 0
    goto LABEL_BOOLEXPR_FALSE_0
LABEL_BOOLEXPR_TRUE_0:
    iconst_1 ; true = 1
LABEL_BOOLEXPR_FALSE_0:
    ior
    ireturn
; Line #13:     return true && false || 5 > 10;

    return
.end method
; Line #14: }

; Line #15: 

.method public static foo(II)I
.limit stack 110
.limit locals 110
; Line #16: int foo( int a , int ab ) {

; Line #17:     int i, result;

    ldc 0
    istore 3
; Line #18:     result = 0;

    ldc 1
    istore 2
; Line #19:     i = 1;

LABEL_WHILE_BEGIN_1:
    iload 2
    iload 0
    istore 101
    iload 101
    isub
    ifle LABEL_BOOLEXPR_TRUE_2
    iconst_0 ; false = 0
    goto LABEL_BOOLEXPR_FALSE_2
LABEL_BOOLEXPR_TRUE_2:
    iconst_1 ; true = 1
LABEL_BOOLEXPR_FALSE_2:
    ifeq LABEL_WHILE_EXIT_1
; Line #20:     while( i <= a ) {

    iload 3
    iload 2
    istore 101
    iload 101
    iadd
    istore 3
; Line #21:         result = result + i;

    iload 2
    ldc 1
    istore 101
    iload 101
    iadd
    istore 2
; Line #22:         i = i + 1;

    goto LABEL_WHILE_BEGIN_1
LABEL_WHILE_EXIT_1:
; Line #23:     }

    iload 3
    iload 1
    istore 101
    iload 101
    iadd
    ireturn
; Line #24:     return result + ab ;

    return
.end method
; Line #25: }

; Line #26: 

.method public static main([Ljava/lang/String;)V
.limit stack 110
.limit locals 110
    new java/util/Scanner
    dup
    getstatic java/lang/System/in Ljava/io/InputStream;
    invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
    putstatic output/_sc Ljava/util/Scanner;
; Line #27: int main() {

    ldc 10
    istore 1
; Line #28:     int c = 10 ;

    getstatic output/_sc Ljava/util/Scanner;
    invokevirtual java/util/Scanner/nextInt()I
    putstatic output/a I
; Line #29:     read a;

; Line #30:     

    getstatic output/a I
    ldc 999
    invokestatic output/foo(II)I
    istore 1
; Line #31:     c = foo( a , 999);

    getstatic java/lang/System/out Ljava/io/PrintStream;
    iload 1
    invokevirtual java/io/PrintStream/print(I)V
; Line #32:     print c;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc 100
    ldc 1
    invokestatic output/foo(II)I
    invokevirtual java/io/PrintStream/print(I)V
; Line #33:     print foo(100,1);

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "
"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #34:     print "\n";

    iload 1
    ldc 100
    istore 101
    iload 101
    isub
    ifge LABEL_BOOLEXPR_TRUE_3
    iconst_0 ; false = 0
    goto LABEL_BOOLEXPR_FALSE_3
LABEL_BOOLEXPR_TRUE_3:
    iconst_1 ; true = 1
LABEL_BOOLEXPR_FALSE_3:
    ifeq LABEL_IF_FALSE_4
; Line #35:     if( c >= 100 ) {

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "c >= 100 
"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #36:         print "c >= 100 \n";

; Line #37:     }

    goto LABEL_IF_EXIT_4
LABEL_IF_FALSE_4:
; Line #38:     else {

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "c < 100 
"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #39:         print "c < 100 \n";

LABEL_IF_EXIT_4:
; Line #40:     }

    ldc 0
; Line #41:     return 0;

; Line #42: 

    return
.end method
; Line #43: }

; Line #44: 

; Line #45: 

; Line #46: 

