%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
extern void yyerror();

struct node{
	char *val;
	struct node *next;
};

struct list{
	struct node *head;
	struct node *tail;
};

struct Macrolist{
	struct list *macro;
	struct Macrolist *next;
};

struct Macrolist *macros = NULL;

struct list* findmacro(char *s){
	struct Macrolist *t = macros;
	while(t != NULL){
		if(strcmp(t->macro->head->val, s) == 0)
			return t->macro;
		t = t->next;
	}
	return NULL;
}

void addMacro(struct list *l){
	l->head = l->head->next;

	struct list *alreadyExists = findmacro(l->head->val);
	if(alreadyExists != NULL){
		yyerror();
		exit(0);
	}

	struct Macrolist *t = malloc(sizeof(struct Macrolist));
	t-> macro = l;
	t->next   = NULL;

	if(macros == NULL){
		macros = t;
	}
	else{
		t->next = macros;
		macros = t;
	}
}

void listInsert(struct list *l, char *s){
	struct node *temp;
	temp = malloc(sizeof(struct node));

	temp-> val = malloc(sizeof(char)*strlen(s));
	strcpy(temp-> val, s);
	temp->next = NULL;
	
	if(l->head == NULL){
		l->head = temp;
		l->tail = temp;
	}
	else{
		l->tail->next = temp;
		l->tail = temp;
	}
}

struct list* listMerge(struct list *l1, struct list *l2){
	if(l1->head == NULL)
		return l2;
	if(l2->head == NULL)
		return l1;

	l1->tail->next = l2->head;
	l1->tail = l2->tail;
	return l1;
}

struct list* initList(){
	struct list *tmp = malloc(sizeof(struct list));
	tmp-> head = NULL;
	tmp-> tail = NULL;
	return tmp; 
}

void printer(struct list *l){
	struct node *tmp = l->head;
	while(tmp != NULL){
		printf("%s ", tmp->val);
		/*if(strcmp(tmp->val,";") == 0)
			printf("\n");*/
		tmp = tmp->next;
	}
}

/*void printMacros(){
	printf("Macros::\n\n");

	struct Macrolist *t = macros;
	while(t != NULL){
		printer(t->macro);
		t = t->next;
		printf("\n");
	}

	printf("DOne!!\n\n");
}
*/

struct list* macroexpand(struct list *id, struct list *arglist, int rmbrace){
	struct list *currmacro = findmacro(id->head->val);
	if(currmacro == NULL){
		yyerror();
		exit(0);
	}

	struct list *newl = malloc(sizeof(struct list));

	//cycle to ) and establish dict
	//assume max 100 args
	char *keys[100], *vals[100];
	int size = 0;
	struct node *t = currmacro->head->next->next;
	struct node *t2 = arglist->head;

	while(strcmp(t->val,")") != 0){
		if(strcmp(t->val, ",") != 0){
			keys[size] = t->val;
			// if arglist is shorter than reqd then error
			// max 1000 chars per argument
			if(t2 == NULL){
				yyerror();
				exit(0);
			}
			vals[size] = malloc(1000);
			strcpy(vals[size], "");
			while(t2 != NULL && strcmp(t2->val,",")){
				strcat(vals[size], t2->val);
				t2 = t2->next;
			}
			if(t2 != NULL)
				t2 = t2->next;
			size++;
		}
		t = t->next;
		// if arglist is shorter than reqd then error
		/*if(t2 == NULL){
			yyerror();
			exit(0);
		}
		t2 = t2->next;*/
	}
	t = t->next;
	// if arglist is longer than reqd then error  
	if(t2 != NULL){
		yyerror();
		exit(0);
	}

	if( rmbrace == 1 && strcmp(t->val, "{") == 0)
		t = t->next;

	if(rmbrace == 0 && strcmp(t->val, "{") == 0){
		yyerror();
		exit(0);
	}

	struct node *p = malloc(sizeof(struct node));
	newl->head = p;
	while(t != NULL){
		//do if not dict key, else lex replace


		int flag = 0,i;
		for(i=0;i<size;i++)
			if(strcmp(keys[i], t->val) == 0){
				flag = 1;
				break;
			}


		if(!flag){
			p->val = malloc(strlen(t->val));
			strcpy(p->val, t->val);
		}
		else{
			p->val = malloc(strlen(vals[i]));
			strcpy(p->val, vals[i]);	
		}
		
		p->next = NULL;
		t = t->next;
		if(t != NULL){
			if(rmbrace == 1 && strcmp(t->val, "}")==0 && t->next == NULL)
				break;

			p->next = malloc(sizeof(struct node));
			p = p->next;
		}
	}
	newl->tail = p;

	return newl;
}

%}

%union {
	int	int_val;
	char* name;
	char op;
	struct list *tokenList;
}

%token <name> 		ID INTEGER
%token <int_val>  	RELOP LOGOP
%token <op>  		ARITHOP

%token <name>  	MACRO LBRACE LBRACE2 LBRACE3 RBRACE3 RBRACE2 RBRACE 
%token <name>  	CLASS PUBLIC STATIC tVOID MAIN STRING PRINT EXTENDS RETURN
%token <name>	tBOOLEAN tINT IF ELSE WHILE LENGTH tTRUE tFALSE THIS NEW
%token <name> 	SEMICOLON DOT COMMA EQUALS NOT

%type <tokenList> 	Goal MacroDefinition MainClass TypeDeclaration Identifier Expression
%type <tokenList> 	Type MethodDeclaration Statement PrimaryExpression Integer MacroDefExpression MacroDefStatement
%type <tokenList> 	MacroStar TypeStar TISStar MethodStar CTIStar TICOnce StatementList CEStar ECEOnce CIStar ICIOnce
%type <tokenList> 	sel_statement Term

%nonassoc NO_ELSE
%nonassoc ELSE

%nonassoc NO_EXPR
%nonassoc RELOP LOGOP ARITHOP LBRACE3 DOT

%start Goal

%%

Goal:  MacroStar MainClass 	TypeStar {
										$$ = listMerge($1,$2);
										$$ = listMerge($$,$3);
									 	// printMacros();
									 	printer($$);
									 }

MacroStar: MacroStar MacroDefinition	{ /*$$ = listMerge($1, $2);*/ addMacro($2); $$ = $1; }
		|  								{ $$ = initList(); }

TypeStar: TypeDeclaration TypeStar	{ $$ = listMerge($1, $2);}
		| 							{ $$ = initList(); }

MainClass: CLASS Identifier LBRACE PUBLIC STATIC tVOID MAIN LBRACE2 STRING LBRACE3 RBRACE3 Identifier RBRACE2 LBRACE PRINT LBRACE2 Expression RBRACE2 SEMICOLON RBRACE RBRACE 	{
					$$ = initList(); 
					listInsert($$, "class");
					$$ = listMerge($$, $2);
					listInsert($$, "{");
					listInsert($$, "public");
					listInsert($$, "static");
					listInsert($$, "void");
					listInsert($$, "main");
					listInsert($$, "(");
					listInsert($$, "String");
					listInsert($$, "[");
					listInsert($$, "]");
					$$ = listMerge($$, $12);
					listInsert($$, ")");
					listInsert($$, "{");
					listInsert($$, "System.out.println");
					listInsert($$, "(");
					$$ = listMerge($$, $17);
					listInsert($$, ")");
					listInsert($$, ";");
					listInsert($$, "}");
					listInsert($$, "}");
				}

TypeDeclaration: CLASS Identifier LBRACE TISStar MethodStar RBRACE {
					$$ = initList();
					listInsert($$, "class");
					$$ = listMerge($$, $2);
					listInsert($$, "{");
					$$ = listMerge($$, $4);
					$$ = listMerge($$, $5);
					listInsert($$, "}");
				}
				| CLASS Identifier EXTENDS Identifier LBRACE TISStar MethodStar RBRACE {
					$$ = initList();
					listInsert($$, "class");
					$$ = listMerge($$, $2);
					listInsert($$, "extends");
					$$ = listMerge($$, $4);
					listInsert($$, "{");
					$$ = listMerge($$, $6);
					$$ = listMerge($$, $7);
					listInsert($$, "}");
				}

MethodStar: MethodDeclaration MethodStar	{ $$ = listMerge($1, $2); }
		|		{ $$ = initList(); }

TISStar: TISStar Type Identifier SEMICOLON { 
					$$ = listMerge($1, $2);
					$$ = listMerge($$, $3);
					listInsert($$, ";");
				}
		| 	{ $$ = initList(); }

MethodDeclaration: PUBLIC Type Identifier LBRACE2 TICOnce RBRACE2 LBRACE TISStar StatementList RETURN Expression SEMICOLON RBRACE {
					$$ = initList();
					listInsert($$, "public");
					$$ = listMerge($$, $2);
					$$ = listMerge($$, $3);
					listInsert($$, "(");
					$$ = listMerge($$, $5);
					listInsert($$, ")");
					listInsert($$, "{");
					$$ = listMerge($$, $8);
					$$ = listMerge($$, $9);
					listInsert($$, "return");
					$$ = listMerge($$, $11);
					listInsert($$, ";");
					listInsert($$, "}");
				}

CTIStar: COMMA Type Identifier CTIStar	{
					$$ = initList();
					listInsert($$, ",");
					$$ = listMerge($$, $2);
					$$ = listMerge($$, $3);
					$$ = listMerge($$, $4);
				}
		| 	{ $$ = initList(); }

TICOnce: Type Identifier CTIStar	{
					$$ = listMerge($1, $2);
					$$ = listMerge($$, $3);
				}
		|	{ $$ = initList(); }

StatementList: 	Statement StatementList	{ $$ = listMerge($1, $2); }
			| 	{ $$ = initList(); }

Type: tINT LBRACE3 RBRACE3 { $$ = initList(); listInsert($$, "int"); listInsert($$, "["); listInsert($$, "]");}
	| tBOOLEAN	{ $$ = initList(); listInsert($$, "boolean");}
	| tINT 		{ $$ = initList(); listInsert($$, "int");}
	| Identifier { $$ = $1; }

CEStar: COMMA Expression CEStar	{ 
					$$ = initList();
					listInsert($$, ",");
					$$ = listMerge($$, $2);
					$$ = listMerge($$, $3);
				}
		|	{ $$ = initList(); }

ECEOnce: Expression CEStar	{ $$ = listMerge($1, $2); }
		|	{ $$ = initList(); }

Statement: LBRACE StatementList RBRACE 	{ $$ = initList(); listInsert($$, "{"); $$ = listMerge($$, $2); listInsert($$, "}"); }
		|  PRINT LBRACE2 Expression RBRACE2 SEMICOLON 	{ $$ = initList(); listInsert($$, "System.out.println"); listInsert($$, "("); $$ = listMerge($$,$3); listInsert($$, ")"); listInsert($$, ";"); }
		|  Identifier EQUALS Expression SEMICOLON	{ $$ = $1; listInsert($$, "="); $$ = listMerge($$, $3); listInsert($$,";"); }
		|  Identifier LBRACE3 Expression RBRACE3 EQUALS Expression SEMICOLON  { $$ = $1; listInsert($$,"["); $$ = listMerge($$, $3); listInsert($$,"]"); listInsert($$,"="); $$ = listMerge($$, $6); listInsert($$,";");}
		|  Identifier LBRACE2 ECEOnce RBRACE2 SEMICOLON	{ /*Macro*/ $$ = macroexpand($1, $3, 1); /*$$ = $1; listInsert($$, "("); $$ = listMerge($$, $3); listInsert($$, ")"); listInsert($$, ";");*/ }
		|  sel_statement	{ $$ = $1; }
		|  WHILE LBRACE2 Expression RBRACE2 Statement	{ $$ = initList(); listInsert($$, "while"); listInsert($$, "("); $$ = listMerge($$,$3); listInsert($$, ")"); $$ = listMerge($$,$5); }

sel_statement: IF LBRACE2 Expression RBRACE2 Statement ELSE Statement	{
					$$ = initList();
					listInsert($$, "if");
					listInsert($$, "(");
					$$ = listMerge($$, $3);
					listInsert($$, ")");
					$$ = listMerge($$, $5);
					listInsert($$, "else");
					$$ = listMerge($$, $7);
				}
			|  IF LBRACE2 Expression RBRACE2 Statement	%prec NO_ELSE {
					$$ = initList();
					listInsert($$, "if");
					listInsert($$, "(");
					$$ = listMerge($$, $3);
					listInsert($$, ")");
					$$ = listMerge($$, $5);
				}

Expression: PrimaryExpression RELOP PrimaryExpression	{ $$ = $1; if ($2 == 1) listInsert($$,"||"); else listInsert($$,"&&"); $$ = listMerge($$, $3);}
		|	PrimaryExpression LOGOP PrimaryExpression	{ $$ = $1; if ($2 == 1) listInsert($$,"<="); else listInsert($$,"!="); $$ = listMerge($$, $3);}
		|	PrimaryExpression ARITHOP PrimaryExpression	{ $$ = $1; char t[2]; t[0] = $2; t[1] = '\0'; 	  listInsert($$,t);    $$ = listMerge($$, $3);}
		|	PrimaryExpression LBRACE3 PrimaryExpression RBRACE3	{ $$ = $1; listInsert($$,"["); $$ = listMerge($$, $3); listInsert($$,"]"); }
		|	PrimaryExpression DOT Identifier LBRACE2 ECEOnce RBRACE2	{ $$ = $1; listInsert($$,"."); $$ = listMerge($$, $3); listInsert($$,"("); $$ = listMerge($$, $5); listInsert($$,")"); }
		| 	PrimaryExpression DOT LENGTH	{ $$ = $1; listInsert($$,"."); listInsert($$,"length"); }
		| 	PrimaryExpression %prec NO_EXPR { $$ = $1; }
		|	Identifier LBRACE2  ECEOnce RBRACE2	{ /*Macro*/ $$ = macroexpand($1, $3, 0); /*$$ = $1; listInsert($$,"("); $$ = listMerge($$, $3); listInsert($$,")");*/ }

PrimaryExpression: 	NEW tINT LBRACE3 Expression RBRACE3	{ $$ = initList(); listInsert($$, "new"); listInsert($$, "int"); listInsert($$, "["); $$ = listMerge($$, $4); listInsert($$, "]"); }
				|	NEW Identifier LBRACE2 RBRACE2	{ $$ = initList(); listInsert($$, "new"); $$ = listMerge($$, $2); listInsert($$, "("); listInsert($$, ")"); }
				|	LBRACE2 Expression RBRACE2	{ $$ = initList(); listInsert($$, "("); $$ = listMerge($$, $2); listInsert($$, ")"); }
				|	Term	{ $$ = $1; }
				|	NOT Expression 	{ $$ = initList(); listInsert($$, "!"); $$ = listMerge($$, $2); }

Term:	Integer 	{ $$ = $1; }
	|	tTRUE		{ $$ = initList(); listInsert($$, "true");}
	| 	tFALSE		{ $$ = initList(); listInsert($$, "false");}
	|	Identifier	{ $$ = $1; }
	|	THIS		{ $$ = initList(); listInsert($$, "this");}

MacroDefinition: MacroDefExpression	{ $$ = $1; }
			|	 MacroDefStatement	{ $$ = $1; }

CIStar: COMMA Identifier CIStar	{ $$ = initList(); listInsert($$, ","); $$ = listMerge($$, $2); $$ = listMerge($$, $3); }
		|	{ $$ = initList(); }

ICIOnce: Identifier CIStar 	{ $$ = listMerge($1, $2); }
		|	{ $$ = initList(); }

MacroDefStatement: MACRO Identifier LBRACE2 ICIOnce RBRACE2 LBRACE StatementList RBRACE {
					$$ = initList();
					listInsert($$, "#define");
					$$ = listMerge($$, $2);
					listInsert($$, "(");
					$$ = listMerge($$, $4);
					listInsert($$, ")");
					listInsert($$, "{");
					$$ = listMerge($$, $7);
					listInsert($$, "}");
				}

MacroDefExpression: MACRO Identifier LBRACE2 ICIOnce RBRACE2 LBRACE2 Expression RBRACE2	{
					$$ = initList();
					listInsert($$, "#define");
					$$ = listMerge($$, $2);
					listInsert($$, "(");
					$$ = listMerge($$, $4);
					listInsert($$, ")");
					listInsert($$, "(");
					$$ = listMerge($$, $7);
					listInsert($$, ")");
				}

Identifier: ID 	{
					$$ = initList();
					listInsert($$, $1);	
				}

Integer: INTEGER {
					$$ = initList();
					listInsert($$, $1);
				}
%%