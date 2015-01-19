.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
; Line #1: // print hello world

; Line #2: #pragma source off

; Line #3: #pragma token off

; Line #4: #pragma statistic off

; Line #5: #pragma symbol on

; Line #6: 

.method public static printDouble(DIDF)V
.limit stack 110
.limit locals 110
; Line #7: void printDouble(double d,int l,double c,float r){

    getstatic java/lang/System/out Ljava/io/PrintStream;
    dload 0
    invokevirtual java/io/PrintStream/print(D)V
; Line #8:     print d;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #9:     print "\n";

    getstatic java/lang/System/out Ljava/io/PrintStream;
    iload 2
    invokevirtual java/io/PrintStream/print(I)V
; Line #10:     print l;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #11:     print "\n";

    getstatic java/lang/System/out Ljava/io/PrintStream;
    dload 3
    invokevirtual java/io/PrintStream/print(D)V
; Line #12:     print c;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #13:     print "\n";

    getstatic java/lang/System/out Ljava/io/PrintStream;
    fload 5
    invokevirtual java/io/PrintStream/print(F)V
; Line #14:     print r;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #15:     print "\n";

    return
.end method
; Line #16: }

; Line #17: 

.method public static main([Ljava/lang/String;)V
.limit stack 110
.limit locals 110
    new java/util/Scanner
    dup
    getstatic java/lang/System/in Ljava/io/InputStream;
    invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
    putstatic output/_sc Ljava/util/Scanner;
; Line #18: int main(){

; Line #19:     double a;

; Line #20:     int b;

    ldc 1.000100
    f2d
    dstore 1
; Line #21:     a = 1.0001;

    ldc 10
    istore 3
; Line #22:     b = 10;

    iload 3
    i2d
    iload 3
    iload 3
    i2d
    iload 3
    i2f
    invokestatic output/printDouble(DIDF)V
; Line #23:     printDouble(b,b,b,b);

    ldc 0
; Line #24:     return 0;

    return
.end method
; Line #25: }

; Line #26: 

; Line #27: 

