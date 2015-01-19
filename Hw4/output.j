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

; Line #8:     double a;

; Line #9:     int b;

    ldc 1.000100
    f2d
    dstore 1
; Line #10:     a = 1.0001;

    ldc 10
    istore 3
; Line #11:     b = 10;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    dload 1
    invokevirtual java/io/PrintStream/print(D)V
; Line #12:     print a;

    ldc 0
; Line #13:     return 0;

    return
.end method
; Line #14: }

; Line #15: 

; Line #16: 

; Line #17: 

