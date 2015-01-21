.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
; Line #1: // print hello world

; Line #2: #pragma source off

; Line #3: #pragma token off

; Line #4: #pragma statistic off

; Line #5: #pragma symbol on

; Line #6: 

.method public static main([Ljava/lang/String;)V
.limit stack 110
.limit locals 110
    new java/util/Scanner
    dup
    getstatic java/lang/System/in Ljava/io/InputStream;
    invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
    putstatic output/_sc Ljava/util/Scanner;
; Line #7: int main(){

    ldc 1
    i2d
    dstore 1
    ldc 2
    i2d
    dstore 3
; Line #8:     double a = 1, b = 2;

; Line #9:     const double c = 3, d = 4;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    dload 1
    invokevirtual java/io/PrintStream/print(D)V
; Line #10:     print a;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    dload 3
    invokevirtual java/io/PrintStream/print(D)V
; Line #11:     print b;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc2_w 0.000000
    invokevirtual java/io/PrintStream/print(D)V
; Line #12:     print c;

    iconst_1
    ifeq LABEL_IF_FALSE_0
; Line #13:     if(true){

    ldc 0
    return
; Line #14:         return 0;

; Line #15:     }

LABEL_IF_FALSE_0:
    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc2_w 0.000000
    invokevirtual java/io/PrintStream/print(D)V
; Line #16:     print d;

    ldc 0
    return
; Line #17:     return 0;

    return
.end method
; Line #18: }

