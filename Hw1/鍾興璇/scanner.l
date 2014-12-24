digit  [0-9]
letter [a-zA-Z]

%{
	int count = 0;

%}

%%

{letter}({letter}|{digit})* 	{  printf("ID: %s", yytext);   count++; }



%%


int main() {
  yylex();
  printf("\n\n number of identifiers = %d\n" , count );
  return 0;
	
}



