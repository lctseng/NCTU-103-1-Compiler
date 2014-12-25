all: non-gcc
	gcc --std=c99  -Wno-implicit-function-declaration -o parser  y.tab.c lex.yy.c -ly -lfl
non-gcc:
	yacc -d -v   parser.y
	flex scanner.l
run: stdout
	./parser input.c
stdout: non-gcc
	gcc --std=c99  -Wno-implicit-function-declaration -DERR_FD=stdout -o parser  y.tab.c lex.yy.c -ly -lfl
debug: stdout
	./parser ./test_sample/table/test2
