all:
	yacc -d -v parser.y
	lex scanner.l
	gcc -o parser lex.yy.c y.tab.c -ly -lfl
run: all
	./parser input.c
