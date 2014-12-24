%%
.* {ECHO;yyless(0);}
%%
main(){
   yylex();
   return 0;

}

int yywrap(){
   printf("EOF\n");
   return 1; 
}
