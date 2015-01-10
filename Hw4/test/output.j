.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.field public static mySeed I
; Line #1: int mySeed;

; Line #2: const int INITIALIZE = 123456789;

; Line #3: 

.method public static srand(I)V
.limit stack 110
.limit locals 110
; Line #4: void srand( int seed ) {

    iload 0
    putstatic output/mySeed I
; Line #5: 	mySeed = seed;

    return
.end method
; Line #6: }

; Line #7: 

.method public static rand()I
.limit stack 110
.limit locals 110
; Line #8: int rand() {

    getstatic output/mySeed I
    ldc 234
    istore 101
    iload 101
    imul
    ldc 123
    istore 101
    iload 101
    iadd
    putstatic output/mySeed I
; Line #9: 	mySeed = mySeed * 234 + 123;

; Line #10: 	

    getstatic output/mySeed I
    ldc 0
    istore 101
    iload 101
    isub
    ifge LABEL_BOOLEXPR_TRUE_0
    iconst_0 ; false = 0
    goto LABEL_BOOLEXPR_FALSE_0
LABEL_BOOLEXPR_TRUE_0:
    iconst_1 ; true = 1
LABEL_BOOLEXPR_FALSE_0:
    ifeq LABEL_IF_FALSE_1
; Line #11: 	if( mySeed >= 0 ) {

    getstatic output/mySeed I
    ireturn
; Line #12: 		return mySeed;

; Line #13: 	}

; Line #14: 	

    goto LABEL_IF_EXIT_1
LABEL_IF_FALSE_1:
; Line #15: 	else {

    getstatic output/mySeed I
    ineg
    ireturn
; Line #16: 		return -mySeed;

LABEL_IF_EXIT_1:
; Line #17: 	}	

    ldc 0
    ireturn
; Line #18: 	return 0;

    return
.end method
; Line #19: }

; Line #20: 

.method public static main([Ljava/lang/String;)V
.limit stack 110
.limit locals 110
    new java/util/Scanner
    dup
    getstatic java/lang/System/in Ljava/io/InputStream;
    invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
    putstatic output/_sc Ljava/util/Scanner;
; Line #21: int main() {

    ldc 1
    istore 1
; Line #22: 	int i = 1, grade;

; Line #23: 	

    ldc 123456789
    invokestatic output/srand(I)V
; Line #24: 	srand( INITIALIZE );	// print "student (number)    (grade)    pass/fail"

; Line #25: 	

; Line #26: 	// print "student (number)    (grade)    pass/fail"

LABEL_WHILE_BEGIN_2:
    iload 1
    ldc 10
    istore 101
    iload 101
    isub
    ifle LABEL_BOOLEXPR_TRUE_3
    iconst_0 ; false = 0
    goto LABEL_BOOLEXPR_FALSE_3
LABEL_BOOLEXPR_TRUE_3:
    iconst_1 ; true = 1
LABEL_BOOLEXPR_FALSE_3:
    ifeq LABEL_WHILE_EXIT_2
; Line #27: 	while( i <= 10 ) {

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "student "
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #28: 		print "student ";

    getstatic java/lang/System/out Ljava/io/PrintStream;
    iload 1
    invokevirtual java/io/PrintStream/print(I)V
; Line #29: 		print i;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "	"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #30: 		print "\t";

    invokestatic output/rand()I
    ldc 71
    istore 101
    iload 101
    irem
    ldc 30
    istore 101
    iload 101
    iadd
    istore 2
; Line #31: 		grade = rand() % 71 + 30;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    iload 2
    invokevirtual java/io/PrintStream/print(I)V
; Line #32: 		print grade;

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "	"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #33: 		print "\t";

; Line #34: 		

    iload 2
    ldc 60
    istore 101
    iload 101
    isub
    ifge LABEL_BOOLEXPR_TRUE_4
    iconst_0 ; false = 0
    goto LABEL_BOOLEXPR_FALSE_4
LABEL_BOOLEXPR_TRUE_4:
    iconst_1 ; true = 1
LABEL_BOOLEXPR_FALSE_4:
    ifeq LABEL_IF_FALSE_5
; Line #35: 		if( grade >= 60 ) {

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "pass
"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #36: 			print "pass\n";

; Line #37: 		}

; Line #38: 		

    goto LABEL_IF_EXIT_5
LABEL_IF_FALSE_5:
; Line #39: 		else {

    getstatic java/lang/System/out Ljava/io/PrintStream;
    ldc "fail
"
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #40: 			print "fail\n";

LABEL_IF_EXIT_5:
; Line #41: 		}

; Line #42: 		

    iload 1
    ldc 1
    istore 101
    iload 101
    iadd
    istore 1
; Line #43: 		i = i + 1;

    goto LABEL_WHILE_BEGIN_2
LABEL_WHILE_EXIT_2:
; Line #44: 	}

; Line #45: 

    ldc 0
; Line #46: 	return 0;

    return
.end method
; Line #47: }

; Line #48: 

; Line #49: 

; Line #50: /*

; Line #51: Input:

; Line #52: No input

; Line #53: 

; Line #54: Output:

; Line #55: student 1       91      pass

; Line #56: student 2       44      fail

; Line #57: student 3       66      pass

; Line #58: student 4       81      pass

; Line #59: student 5       100     pass

; Line #60: student 6       72      pass

; Line #61: student 7       35      fail

; Line #62: student 8       70      pass

; Line #63: student 9       33      fail

; Line #64: student 10      67      pass

; Line #65: */

