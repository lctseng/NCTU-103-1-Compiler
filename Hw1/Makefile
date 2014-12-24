# Build all
all:
	 lex scanner.l
	 gcc lex.yy.c -lfl -o scanner
	 rm -f lex.yy.c
run:
	 @lex scanner.l
	 @gcc lex.yy.c -lfl -o scanner
	 @rm -f lex.yy.c
	 @echo "******** Test Output *********"
	 @./scanner input.c ;true
	 @echo "******** End of Test *********"
test:
	@lex scanner.l
	@gcc lex.yy.c -lfl -o scanner  -D ERR_FD=stdout
	@rm -f lex.yy.c
	@for type in comment delim error2 float float_error2 general_1 identifier integer keyword op pragma_errorcase pragma_errorcase2 scientific scientific_error string string_error; do \
	    echo "Running test for $$type" ;\
	    ./scanner ./test_sample/$$type > $$type.result ;\
	    diff ./test_sample/answer_sample/$$type $$type.result ;\
	    if [ $$? -eq 0 ] ; then \
		echo "***Pass***" ;\
	    fi ;\
	    rm -f $$type.result ;\
	done





