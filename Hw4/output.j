.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
; Line #1: // print hello world

; Line #2: #pragma source off

; Line #3: #pragma token off

; Line #4: #pragma statistic off

; Line #5: #pragma symbol on

; Line #6: 

.field public static gga I
; Line #7: int gga = 10;

.field public static ggd D
; Line #8: double ggd = 5.58;

; Line #9: 

.method public static printDouble(DIDF)V
.limit stack 110
.limit locals 110
; Line #10: void printDouble(double d,int l,double c,float r){

    getstatic java/lang/System/out Ljava/io/PrintStream;
    dload 0
    invokevirtual java/io/PrintStream/print(D)V
; Line #11:     print d;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #12:     print "\n";

    getstatic java/lang/System/out Ljava/io/PrintStream;
    iload 2
    invokevirtual java/io/PrintStream/print(I)V
; Line #13:     print l;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #14:     print "\n";

    getstatic java/lang/System/out Ljava/io/PrintStream;
    dload 3
    invokevirtual java/io/PrintStream/print(D)V
; Line #15:     print c;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #16:     print "\n";

    getstatic java/lang/System/out Ljava/io/PrintStream;
    fload 5
    invokevirtual java/io/PrintStream/print(F)V
; Line #17:     print r;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #18:     print "\n";

    getstatic java/lang/System/out Ljava/io/PrintStream;
    getstatic output/gga I
    invokevirtual java/io/PrintStream/print(I)V
; Line #19:     print gga;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #20:     print "\n";

    return
.end method
; Line #21: }

; Line #22: 

.method public static main([Ljava/lang/String;)V
.limit stack 110
.limit locals 110
    new java/util/Scanner
    dup
    getstatic java/lang/System/in Ljava/io/InputStream;
    invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
    putstatic output/_sc Ljava/util/Scanner;
    ldc 10
    putstatic output/gga I
    ldc 5.580000
    f2d
    putstatic output/ggd D
; Line #23: int main(){

; Line #24:     double a;

; Line #25:     int b;

    ldc 1.000100
    f2d
    dstore 1
; Line #26:     a = 1.0001;

    ldc 10
    istore 3
; Line #27:     b = 10;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    getstatic output/ggd D
    invokevirtual java/io/PrintStream/print(D)V
; Line #28:     print ggd;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "\n"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #29:     print "\n";

    iload 3
    i2d
    iload 3
    iload 3
    i2d
    iload 3
    i2f
    invokestatic output/printDouble(DIDF)V
; Line #30:     printDouble(b,b,b,b);

    ldc 0
; Line #31:     return 0;

    return
.end method
; Line #32: }

; Line #33: 

; Line #34: 

