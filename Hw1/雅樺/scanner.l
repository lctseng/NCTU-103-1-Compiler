%{
#define LIST                { strncat(buf, yytext, sizeof(buf) - strlen(buf) - 1); }
#define LIST1                { strncat(buf1, yytext, sizeof(buf1) - strlen(buf1) - 1); } 
#define token(t)            { LIST; if (Opt_Token) printf("<%s>\n", #t); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define MAX_LINE_LENGTH 257
#define MAX_ID_LENGTH 257
int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
char buf[MAX_LINE_LENGTH];
char buf1[MAX_LINE_LENGTH];
int num_of_vari = 0;
int found = 0;
struct VARI{
	char name[MAX_ID_LENGTH];
	int length;
	int counter;
};
struct VARI vari[500];
struct VARI tocompare;
int compare (struct VARI vari_1, struct VARI vari_2) {
	int k=0;
	if(vari_1.length!=vari_2.length){
		printf("not the same length:%d %d\n" ,vari_1.length,vari_2.length);		
		return 0;
	}
	else{
		if(strcmp(vari_1.name,vari_2.name)==0){
			printf("the same : %s %s\n" ,vari_1.name,vari_2.name);
			return 1;
		}
		else{
			printf("different : %s %s\n" ,vari_1.name,vari_2.name);
			return 0;
		}
	}
}	


%}

%Start STR ENTERING END COMMENT COMMENT2 pragma

%%
<COMMENT2>" " {LIST; BEGIN COMMENT2;}
<COMMENT2>\t {LIST; BEGIN COMMENT2;} 
<COMMENT2>. {LIST; BEGIN COMMENT2;}
<COMMENT2>\n {  LIST; 
				if (Opt_Source)
					printf("%d:%s\n", linenum, buf);
				++linenum;
				buf[0]='\0';
				BEGIN INITIAL;}
<INITIAL>"//" { BEGIN COMMENT2; LIST;}

<COMMENT>" " {LIST; BEGIN COMMENT;}
<COMMENT>\t {LIST; BEGIN COMMENT;} 
<COMMENT>. {LIST; BEGIN COMMENT;}
<COMMENT>\n {  LIST; 
				if (Opt_Source)
					printf("%d:%s\n", linenum, buf);
				++linenum;
				buf[0]='\0';
				BEGIN COMMENT;}
<COMMENT>"*/" { LIST; BEGIN INITIAL;}
<INITIAL>"/*" { BEGIN COMMENT; LIST;}


	/*WHITE SPACE*/
<INITIAL>" " { LIST; BEGIN INITIAL;}
<INITIAL>\t {LIST; BEGIN INITIAL;}


	/*DELIMITERS*/
<INITIAL>"(" {    tokenString("delim", "("); }
<INITIAL>")" {    tokenString("delim", ")"); }
<INITIAL>"," {    tokenString("delim", ","); }
<INITIAL>";" {    tokenString("delim", ";"); }
<INITIAL>"[" {    tokenString("delim", "["); }
<INITIAL>"]" {    tokenString("delim", "]"); }
<INITIAL>"{" {    tokenString("delim", "{"); }
<INITIAL>"}" {    tokenString("delim", "}"); }

	/*ARITHMETIC*/
<INITIAL>"+" {    token("+"); }
<INITIAL>"-" {    token("-"); }
<INITIAL>"*" {    token("*"); }
<INITIAL>"/" {    token("/"); }
<INITIAL>"%" {    token("%"); }
<INITIAL>"=" {    token("="); }

	/*RELATIONAL*/
<INITIAL>"<" {    token("<"); }
<INITIAL>">" {    token(">"); }
<INITIAL>"<=" {    token("<="); }
<INITIAL>"!=" {    token("!="); }
<INITIAL>">=" {    token(">="); }
<INITIAL>"==" {    token("=="); }

	/*LOGICAL*/
<INITIAL>"&&" {    token("&&"); }
<INITIAL>"||" {    token("||"); }
<INITIAL>"!" {    token("!"); }

	/*KEYWORDS*/
<INITIAL>"while" {    tokenString("KW", "while"); }
<INITIAL>"do" {    tokenString("KW", "do"); }
<INITIAL>"if" {    tokenString("KW", "if"); }
<INITIAL>"else" {    tokenString("KW", "else"); }
<INITIAL>"true" {    tokenString("KW", "true"); }
<INITIAL>"false" {    tokenString("KW", "false"); }
<INITIAL>"for" {    tokenString("KW", "for"); }
<INITIAL>"int" {    tokenString("KW", "int"); }
<INITIAL>"print" {    tokenString("KW", "print"); }
<INITIAL>"const" {    tokenString("KW", "const"); }
<INITIAL>"read" {    tokenString("KW", "read"); }
<INITIAL>"boolean" {    tokenString("KW", "boolean"); }
<INITIAL>"bool" {    tokenString("KW", "bool"); }
<INITIAL>"void" {    tokenString("KW", "void"); }
<INITIAL>"float" {    tokenString("KW", "float"); }
<INITIAL>"double" {    tokenString("KW", "double"); }
<INITIAL>"string" {    tokenString("KW", "string"); }
<INITIAL>"continue" {    tokenString("KW", "continue"); }
<INITIAL>"break" {    tokenString("KW", "break"); }
<INITIAL>"return" {    tokenString("KW", "return"); }

	/*IDENTIFIERS*/
<INITIAL>[a-zA-Z][0-9a-zA-Z]* {    tokenString("id", yytext);					 
						found = 0;
						int i=0;
						strcpy(tocompare.name,yytext);
						tocompare.length = yyleng;
					    for(i=0;i<num_of_vari;i++){
							if(compare(vari[i],tocompare)==1){
								vari[i].counter++;
								found=1;
								break;
							}
						};
						 if(found==0){
							strcpy (vari[num_of_vari].name,yytext);
							vari[num_of_vari].counter = 1;
							vari[num_of_vari].length = yyleng;
							//printf("yyleng = %d\n" ,yyleng);
							num_of_vari++;
						 };
					}
				 

	/*INTEGER CONSTANTS*/
<INITIAL>0|[1-9][0-9]* {    tokenString("integer", yytext); }

	/*FLOATING-POINT CONSTANTS*/
<INITIAL>[0-9]+\.[0-9]+ {    tokenString("float", yytext); }

	/*SCIENTIFIC NOTATIONS*/
<INITIAL>[0-9]*(\.[0-9]*)?E([+-])?[0-9]+ {    tokenString("scientific", yytext); }
<INITIAL>[0-9]*(\.[0-9]*)?e([+-])?[0-9]+ {    tokenString("scientific", yytext); }

	/*STRING CONSTANTS*/
<ENTERING>\\\" {LIST; strncat(buf1, "\"", sizeof(buf) - strlen(buf) - 1); BEGIN ENTERING;}
<ENTERING>\\\\ { LIST; strncat(buf1, "\\", sizeof(buf) - strlen(buf) - 1); BEGIN ENTERING;}
<ENTERING>\\n  { LIST; strncat(buf1, "\n", sizeof(buf) - strlen(buf) - 1); BEGIN ENTERING;}
<ENTERING>\\t  { LIST; strncat(buf1, "\t", sizeof(buf) - strlen(buf) - 1); BEGIN ENTERING;}
<ENTERING>\" { printf("<string:%s>\n",buf1); buf1[0]='\0'; BEGIN INITIAL;}
<ENTERING>[\40-\41 \43-\176] { LIST; LIST1; BEGIN ENTERING; } 

<ENTERING>\n { fprintf(stdout, "Error at line %d: %s\n", linenum, yytext); exit(-1);}
<INITIAL>\" { BEGIN ENTERING;}


	/*PRAGMA DIRECTIVES*/
<pragma>" source on" {LIST; Opt_Source=1; BEGIN INITIAL; }
<pragma>" source off" {LIST; Opt_Source=0; BEGIN INITIAL; }
<pragma>" token on" {LIST; Opt_Token=1; BEGIN INITIAL; }
<pragma>" token off" {LIST; Opt_Token=0; BEGIN INITIAL; }
<pragma>" statistic on" {LIST; Opt_Statistic=1; BEGIN INITIAL; }
<pragma>" statistic off" {LIST; Opt_Statistic=0; BEGIN INITIAL; }
<INITIAL>^\#pragma { LIST; BEGIN pragma;}
	


\n {
    LIST;
    if (Opt_Source){
        printf("%d:%s\n", linenum, buf);
	}
    ++linenum;
    buf[0] = '\0';
}

. { // Unrecognized character
    fprintf(stdout, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

%%

int main( int argc, char **argv )
{
    if (argc >= 2)
    {
        yyin = fopen( argv[1], "r" );
        if ( NULL == yyin ) {
            fprintf( stdout, "Cannot open: %s\n", argv[1] );
            exit(-1);
        }
    }
    else
    {
        yyin = stdin;
    }

    yylex();

    if (Opt_Statistic)
    {
		int i=0;
        // Print frequency table
		printf("frequencies of identifiers\n");
		for(i=0;i<num_of_vari;i++){
			printf("%-10s          %-10d\n", vari[i].name,vari[i].counter);
		}
    }

    exit(0);
}
