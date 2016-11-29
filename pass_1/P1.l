%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "P1.tab.h" 
void yyerror(); 
int yyparse(void); 

int lcount = 0;

%}

%%

"class"					{ return CLASS; }
"public"				{ return PUBLIC; }
"static"				{ return STATIC; }
"void"					{ return tVOID; }
"main"					{ return MAIN; }
"String"				{ return STRING; }
"System.out.println"	{ return PRINT; } 
"extends"				{ return EXTENDS; }
"return"				{ return RETURN; }
"boolean"				{ return tBOOLEAN; }
"int"					{ return tINT; }
"if"					{ return IF; }
"else"					{ return ELSE; }
"while"					{ return WHILE; }
"length"				{ return LENGTH; }
"true"					{ return tTRUE; }
"false"					{ return tFALSE; }
"this"					{ return THIS; }
"new"					{ return NEW; }

0|([1-9][0-9]*)		{  
						yylval.name = malloc(strlen(yytext)+1);
						strcpy(yylval.name, yytext);
						return INTEGER; 
					}

[a-zA-Z_][a-zA-Z0-9_]*	{
							yylval.name = malloc(sizeof(char)*(strlen(yytext)+1));
							strcpy(yylval.name, yytext);
						 	return ID;
						}

"{"					{ return LBRACE; }
"}"					{ return RBRACE; }
"("					{ return LBRACE2; }
")"					{ return RBRACE2; }
"["					{ return LBRACE3; }
"]"					{ return RBRACE3; }
";"					{ return SEMICOLON; }
"."					{ return DOT; }
","					{ return COMMA; }
"="					{ return EQUALS; }
"&&"|"||"			{ if(yytext[0] == '&') yylval.int_val = 0; else yylval.int_val = 1; return RELOP; }
"!="|"<="			{ if(yytext[0] == '!') yylval.int_val = 0; else yylval.int_val = 1; return LOGOP; }
"+"|"-"|"*"|"/"		{ yylval.op = yytext[0]; return ARITHOP; }
"!"					{ return NOT; }

"#define"			{ return MACRO; }

"//"[^\n]*		;
"/*"("*"[^/]|[^*])*"*/" ;
[ \t]+			;		
"\n"			{ lcount++; }
<<EOF>>			{ return 0; }
.               {return 1;}

%%

void yyerror() { printf("// Failed to parse macrojava code."); /*printf("Error at line: %d\n", lcount+1);*/ }

int yywrap(void) { return 1; }

int main(void) {
	yyparse();	
	// yylex();
}
