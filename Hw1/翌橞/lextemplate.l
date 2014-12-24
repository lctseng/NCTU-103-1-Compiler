%{
#define LIST                { strncat(buf, yytext, sizeof(buf) - strlen(buf) - 1); }
#define token(t)            { LIST; if (Opt_Token) printf("<%s>\n", #t); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define MAX_LINE_LENGTH 257
#define MAX_ID_LENGTH 257
int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
char buf[MAX_LINE_LENGTH];
%}


digit	[0-9]
integer	0|[1-9][0-9]*
float	(0|[1-9][0-9]*)"."[0-9]+
letter	[a-zA-z]
space	[ \t]

%Start COMMENT COMMENT_ PRAGMA SOURCE CHECK STR STR_END TOKEN STATISTIC

%%
"(" {   tokenString("delim", "(");  }
")" {   tokenString("delim", ")"); 	}
"," {   tokenString("delim", ","); 	}
";" {	tokenString("delim", ";"); 	}
"[" {   tokenString("delim", "[");  }
"]" {   tokenString("delim", "]");  }
"{" {   tokenString("delim", "{");  }
"}" {   tokenString("delim", "}");  }

"+"	{	token("+");	}
"-" {	token("-");	}
"*" {	token("*");	}
"/" {	token("/");	}
"%" {	token("%");	}
"=" {	token("=");	}
"<" {	token("<");	}
"<=" {	token("<=");}
"!=" {	token("!=");}
">=" {	token(">=");}
">"	{	token(">");	}
"==" {	token("==");}
"&&" {	token("&&");}
"||" {	token("||");}
"!" {	token("!");	}


"while" {	tokenString("KW", "while");	}
"do" 	{	tokenString("KW", "do");	}
"if" 	{	tokenString("KW", "if");	}
"else"	{	tokenString("KW", "else");	}
"true"	{	tokenString("KW", "true");	}
"false" {	tokenString("KW", "false");	}
"for"	{	tokenString("KW", "for");	}
"int"	{	tokenString("KW", "int");	}
"print" {	tokenString("KW", "print");	}
"const" {	tokenString("KW", "const");	}
"read"	{	tokenString("KW", "read");	}
"boolean" {	tokenString("KW", "boolean");	}
"bool"	{	tokenString("KW", "bool");	}
"void"	{	tokenString("KW", "void");	}
"float" {	tokenString("KW", "float");	}
"double" {	tokenString("KW", "double");	}
"string" {	tokenString("KW", "string");	}
"continue" {tokenString("KW", "continue");	}
"break" {	tokenString("KW", "break");		}
"return" {	tokenString("KW", "return");	}



"#pragma"{space}*	{	LIST; BEGIN PRAGMA;}
<PRAGMA>source{space}*	{ LIST;BEGIN SOURCE;}
<PRAGMA>token{space}*	{ LIST;BEGIN TOKEN;}
<PRAGMA>statistic{space}*	{ LIST;BEGIN STATISTIC;}
<PRAGMA>"\n"			{	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
							exit(1);
						}
<PRAGMA>.				{	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
							exit(1);
						}
							
<SOURCE>on{space}*		{ LIST; Opt_Source=1;BEGIN CHECK;}
<SOURCE>off{space}*		{ LIST; Opt_Source=0;BEGIN CHECK;}
<SOURCE>"\n"			{	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
							exit(1);}
<SOURCE>.				{	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
							exit(1);}
<TOKEN>on{space}*		{ LIST; Opt_Token=1;BEGIN CHECK;}
<TOKEN>off{space}*		{ LIST; Opt_Token=0;BEGIN CHECK;}
<TOKEN>"\n"				{	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
							exit(1);}
<TOKEN>.				{	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
							exit(1);}
<STATISTIC>on{space}*	{ LIST; Opt_Statistic=1;BEGIN CHECK;}
<STATISTIC>off{space}*	{ LIST; Opt_Statistic=0;BEGIN CHECK;}
<STATISTIC>"\n"			{	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
							exit(1);
						}
<STATISTIC>.				{	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
							exit(1);}








<CHECK>"/*"	{ LIST; BEGIN COMMENT;}
<CHECK>"//"	{ LIST; BEGIN COMMENT_;}
<CHECK>[^\n] {	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
				exit(1);
			}
<CHECK>"\n"	{ 	LIST;	
					if (Opt_Source)
					printf("%d:%s\n", linenum, buf);
						++linenum;
					buf[0] = '\0';
					BEGIN INITIAL;
			}



			
			
			
			
			
			
			

"//"		{ LIST; BEGIN COMMENT_;}
<COMMENT_>[^\n]*	{ LIST; BEGIN COMMENT_;}
<COMMENT_>"\n"	{ 	LIST;	
					if (Opt_Source)
					printf("%d:%s\n", linenum, buf);
						++linenum;
					buf[0] = '\0';
					BEGIN INITIAL;
				}



<COMMENT>"*/"	{ LIST; BEGIN INITIAL;}

<COMMENT>"\n"	{ 	LIST;	
					if (Opt_Source)
						printf("%d:%s\n", linenum, buf);
					++linenum;
					buf[0] = '\0';
					BEGIN COMMENT;
				}
				
<COMMENT>.		{ LIST; BEGIN COMMENT;}
<INITIAL>"/*"	{ LIST; BEGIN COMMENT;}


<INITIAL>{letter}({letter}|{digit})*	{ tokenString("id", yytext);}
<INITIAL>{integer}					{ tokenString("integer", yytext);	}
<INITIAL>{float}						{ tokenString("float", yytext); 	}
<INITIAL>({integer}|{float})+[Ee][+-]?{digit}*	{ tokenString("scientific", yytext);}
<INITIAL>{space}+						{ LIST;}


"\n" {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}

\\ { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

. { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

%%

int main( int argc, char **argv )
{
    if (argc >= 2)
    {
        yyin = fopen( argv[1], "r" );
        if ( NULL == yyin ) {
            fprintf( stderr, "Cannot open: %s\n", argv[1] );
            exit(-1);
        }
    }
    else
    {
        yyin = stdin;
    }

    yylex();
	
	printf("%d:%s\n", linenum, buf);
	
    if (Opt_Statistic)
    {
        // Print frequency table
    }

    exit(0);
}
