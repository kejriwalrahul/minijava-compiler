/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "P1.y" /* yacc.c:339  */

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


#line 280 "P1.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "P1.tab.h".  */
#ifndef YY_YY_P1_TAB_H_INCLUDED
# define YY_YY_P1_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    INTEGER = 259,
    RELOP = 260,
    LOGOP = 261,
    ARITHOP = 262,
    MACRO = 263,
    LBRACE = 264,
    LBRACE2 = 265,
    LBRACE3 = 266,
    RBRACE3 = 267,
    RBRACE2 = 268,
    RBRACE = 269,
    CLASS = 270,
    PUBLIC = 271,
    STATIC = 272,
    tVOID = 273,
    MAIN = 274,
    STRING = 275,
    PRINT = 276,
    EXTENDS = 277,
    RETURN = 278,
    tBOOLEAN = 279,
    tINT = 280,
    IF = 281,
    ELSE = 282,
    WHILE = 283,
    LENGTH = 284,
    tTRUE = 285,
    tFALSE = 286,
    THIS = 287,
    NEW = 288,
    SEMICOLON = 289,
    DOT = 290,
    COMMA = 291,
    EQUALS = 292,
    NOT = 293,
    NO_ELSE = 294,
    NO_EXPR = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 215 "P1.y" /* yacc.c:355  */

	int	int_val;
	char* name;
	char op;
	struct list *tokenList;

#line 368 "P1.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_P1_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 385 "P1.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   178

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  65
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  174

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   246,   246,   253,   254,   256,   257,   259,   284,   293,
     305,   306,   308,   313,   315,   332,   339,   341,   345,   347,
     348,   350,   351,   352,   353,   355,   361,   363,   364,   366,
     367,   368,   369,   370,   371,   372,   374,   384,   393,   394,
     395,   396,   397,   398,   399,   400,   402,   403,   404,   405,
     406,   408,   409,   410,   411,   412,   414,   415,   417,   418,
     420,   421,   423,   435,   447,   452
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "INTEGER", "RELOP", "LOGOP",
  "ARITHOP", "MACRO", "LBRACE", "LBRACE2", "LBRACE3", "RBRACE3", "RBRACE2",
  "RBRACE", "CLASS", "PUBLIC", "STATIC", "tVOID", "MAIN", "STRING",
  "PRINT", "EXTENDS", "RETURN", "tBOOLEAN", "tINT", "IF", "ELSE", "WHILE",
  "LENGTH", "tTRUE", "tFALSE", "THIS", "NEW", "SEMICOLON", "DOT", "COMMA",
  "EQUALS", "NOT", "NO_ELSE", "NO_EXPR", "$accept", "Goal", "MacroStar",
  "TypeStar", "MainClass", "TypeDeclaration", "MethodStar", "TISStar",
  "MethodDeclaration", "CTIStar", "TICOnce", "StatementList", "Type",
  "CEStar", "ECEOnce", "Statement", "sel_statement", "Expression",
  "PrimaryExpression", "Term", "MacroDefinition", "CIStar", "ICIOnce",
  "MacroDefStatement", "MacroDefExpression", "Identifier", "Integer", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
};
# endif

#define YYPACT_NINF -97

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-97)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -97,    14,    65,   -97,    25,    25,    15,   -97,   -97,   -97,
     -97,    22,    48,    25,   -97,    15,    25,    30,    54,   -97,
      45,    38,    62,   -97,    25,     9,    25,   -97,    44,    40,
      79,    69,    37,    38,    72,    26,   -97,    91,    90,    89,
      25,   -97,   -97,    69,    96,    97,    98,    95,    69,   -97,
      -3,   -97,    37,   -97,   -97,   -97,    23,    37,   100,    10,
     -97,   101,   -97,   -97,   104,    25,   105,   -97,   -97,    85,
      40,   107,    37,    37,    37,   -97,   -97,    37,    37,    37,
     110,   113,   115,   -97,   -97,    37,    37,    37,    37,     8,
      37,   106,   117,   -97,   -97,   114,   -97,   118,   119,   120,
     121,    94,   123,   102,   -97,    37,   124,   -97,   -97,   -97,
     -97,   127,   -97,   130,   129,   132,    26,   -97,   111,    69,
      69,   116,    37,   -97,   112,   -97,   136,   -97,   -97,    37,
     -97,   139,   141,    25,   -97,   128,   -97,   -97,    94,    37,
     -97,   143,    25,   148,   122,    69,   -97,   125,   -97,   147,
     -97,    26,   -97,   -97,   -97,   152,    68,    25,   142,   144,
      -3,   122,   154,    37,   -97,    37,   131,   149,   155,   134,
     -97,   156,   157,   -97
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,     1,     0,     0,     6,     3,    57,    56,
      64,     0,     0,     0,     2,     6,    61,     0,     0,     5,
       0,    59,     0,    13,     0,     0,     0,    60,     0,    11,
       0,    20,     0,    59,     0,     0,    22,    23,     0,    11,
       0,    24,    13,    20,     0,     0,     0,     0,    20,    34,
       0,    65,     0,    52,    53,    55,     0,     0,     0,    44,
      49,    54,    51,    58,     0,     0,     0,     8,    10,     0,
      11,     0,     0,     0,     0,    62,    19,    28,     0,     0,
       0,     0,     0,    50,    63,     0,     0,     0,     0,     0,
      28,     0,     0,    21,    12,     0,    29,     0,     0,     0,
       0,    26,     0,     0,    48,     0,     0,    38,    54,    39,
      40,     0,    43,     0,     0,     0,    18,     9,     0,     0,
       0,     0,     0,    27,     0,    31,     0,    47,    41,    28,
      45,     0,     0,     0,    30,    37,    35,    33,    26,     0,
      46,     0,     0,     0,    16,     0,    25,     0,    42,     0,
      13,     0,    17,    36,    32,     0,    20,     0,     0,     0,
      24,    16,     0,     0,    15,     0,     0,     0,     0,     0,
      14,     0,     0,     7
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -97,   -97,   -97,   151,   -97,   -97,   -35,   -40,   -97,    11,
     -97,   -38,   -29,    35,   -87,   -96,   -97,   -19,    13,   -97,
     -97,   145,   -97,   -97,   -97,    -4,   -97
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    14,     6,    15,    38,    29,    39,   152,
     132,    47,    40,   123,   100,    48,    49,   101,    59,    60,
       7,    27,    20,     8,     9,    61,    62
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      11,    12,    70,   114,    68,    71,    65,    77,    78,    18,
      76,    10,    21,    58,     3,    85,    86,    87,    31,    32,
      30,    88,    33,   135,   136,    41,    10,    50,    10,    10,
      13,    41,    16,    80,    79,    95,    69,   112,    83,    50,
      10,    51,   141,    10,    50,    89,    22,    52,    81,   153,
      36,    37,    82,    97,    98,    99,    35,    17,    25,   102,
     103,    92,    34,    23,    36,    37,    41,    53,    54,    55,
      56,    10,    10,     4,    26,    57,    24,    43,    43,    28,
       5,   108,   108,   108,   108,   113,   126,   133,    42,    44,
      44,    64,    36,    37,    45,    45,    46,    46,   107,   109,
     110,   111,    66,   138,    67,    35,    72,    73,    74,    75,
     156,    90,    41,    84,    91,    50,    50,    93,   159,    94,
     147,    96,   157,   104,   105,   106,   115,   116,   117,   144,
     122,   118,   119,   120,   121,   124,   125,   127,   149,   128,
     129,    50,   130,   131,   166,   134,   167,    41,   140,   139,
     137,   142,   160,   161,   143,   145,   148,   150,   151,   154,
     155,   158,   169,   162,   165,   168,    19,   163,   171,   170,
     172,   173,   164,   146,     0,     0,     0,     0,    63
};

static const yytype_int16 yycheck[] =
{
       4,     5,    42,    90,    39,    43,    35,    10,    11,    13,
      48,     3,    16,    32,     0,     5,     6,     7,     9,    10,
      24,    11,    26,   119,   120,    29,     3,    31,     3,     3,
      15,    35,    10,    52,    37,    70,    40,    29,    57,    43,
       3,     4,   129,     3,    48,    35,    16,    10,    25,   145,
      24,    25,    56,    72,    73,    74,    16,     9,    13,    78,
      79,    65,    18,     9,    24,    25,    70,    30,    31,    32,
      33,     3,     3,     8,    36,    38,    22,     9,     9,    17,
      15,    85,    86,    87,    88,    89,   105,   116,     9,    21,
      21,    19,    24,    25,    26,    26,    28,    28,    85,    86,
      87,    88,    11,   122,    14,    16,    10,    10,    10,    14,
     150,    10,   116,    13,    10,   119,   120,    12,   156,    34,
     139,    14,   151,    13,    11,    10,    20,    10,    14,   133,
      36,    13,    13,    13,    13,    12,    34,    13,   142,    12,
      10,   145,    13,    11,   163,    34,   165,   151,    12,    37,
      34,    12,   156,   157,    13,    27,    13,     9,    36,    34,
      13,     9,    13,    21,    10,    34,    15,    23,    34,    14,
      14,    14,   161,   138,    -1,    -1,    -1,    -1,    33
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    42,    43,     0,     8,    15,    45,    61,    64,    65,
       3,    66,    66,    15,    44,    46,    10,     9,    66,    44,
      63,    66,    16,     9,    22,    13,    36,    62,    17,    48,
      66,     9,    10,    66,    18,    16,    24,    25,    47,    49,
      53,    66,     9,     9,    21,    26,    28,    52,    56,    57,
      66,     4,    10,    30,    31,    32,    33,    38,    58,    59,
      60,    66,    67,    62,    19,    53,    11,    14,    47,    66,
      48,    52,    10,    10,    10,    14,    52,    10,    11,    37,
      58,    25,    66,    58,    13,     5,     6,     7,    11,    35,
      10,    10,    66,    12,    34,    47,    14,    58,    58,    58,
      55,    58,    58,    58,    13,    11,    10,    59,    66,    59,
      59,    59,    29,    66,    55,    20,    10,    14,    13,    13,
      13,    13,    36,    54,    12,    34,    58,    13,    12,    10,
      13,    11,    51,    53,    34,    56,    56,    34,    58,    37,
      12,    55,    12,    13,    66,    27,    54,    58,    13,    66,
       9,    36,    50,    56,    34,    13,    48,    53,     9,    52,
      66,    66,    21,    23,    50,    10,    58,    58,    34,    13,
      14,    34,    14,    14
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    45,    46,    46,
      47,    47,    48,    48,    49,    50,    50,    51,    51,    52,
      52,    53,    53,    53,    53,    54,    54,    55,    55,    56,
      56,    56,    56,    56,    56,    56,    57,    57,    58,    58,
      58,    58,    58,    58,    58,    58,    59,    59,    59,    59,
      59,    60,    60,    60,    60,    60,    61,    61,    62,    62,
      63,    63,    64,    65,    66,    67
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     2,     0,     2,     0,    21,     6,     8,
       2,     0,     4,     0,    13,     4,     0,     3,     0,     2,
       0,     3,     1,     1,     1,     3,     0,     2,     0,     3,
       5,     4,     7,     5,     1,     5,     7,     5,     3,     3,
       3,     4,     6,     3,     1,     4,     5,     4,     3,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       2,     0,     8,     8,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 246 "P1.y" /* yacc.c:1646  */
    {
										(yyval.tokenList) = listMerge((yyvsp[-2].tokenList),(yyvsp[-1].tokenList));
										(yyval.tokenList) = listMerge((yyval.tokenList),(yyvsp[0].tokenList));
									 	// printMacros();
									 	printer((yyval.tokenList));
									 }
#line 1579 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 253 "P1.y" /* yacc.c:1646  */
    { /*$$ = listMerge($1, $2);*/ addMacro((yyvsp[0].tokenList)); (yyval.tokenList) = (yyvsp[-1].tokenList); }
#line 1585 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 254 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 1591 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 256 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = listMerge((yyvsp[-1].tokenList), (yyvsp[0].tokenList));}
#line 1597 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 257 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 1603 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 259 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList(); 
					listInsert((yyval.tokenList), "class");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-19].tokenList));
					listInsert((yyval.tokenList), "{");
					listInsert((yyval.tokenList), "public");
					listInsert((yyval.tokenList), "static");
					listInsert((yyval.tokenList), "void");
					listInsert((yyval.tokenList), "main");
					listInsert((yyval.tokenList), "(");
					listInsert((yyval.tokenList), "String");
					listInsert((yyval.tokenList), "[");
					listInsert((yyval.tokenList), "]");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-9].tokenList));
					listInsert((yyval.tokenList), ")");
					listInsert((yyval.tokenList), "{");
					listInsert((yyval.tokenList), "System.out.println");
					listInsert((yyval.tokenList), "(");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-4].tokenList));
					listInsert((yyval.tokenList), ")");
					listInsert((yyval.tokenList), ";");
					listInsert((yyval.tokenList), "}");
					listInsert((yyval.tokenList), "}");
				}
#line 1632 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 284 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), "class");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-4].tokenList));
					listInsert((yyval.tokenList), "{");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-2].tokenList));
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList));
					listInsert((yyval.tokenList), "}");
				}
#line 1646 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 293 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), "class");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-6].tokenList));
					listInsert((yyval.tokenList), "extends");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-4].tokenList));
					listInsert((yyval.tokenList), "{");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-2].tokenList));
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList));
					listInsert((yyval.tokenList), "}");
				}
#line 1662 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 305 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = listMerge((yyvsp[-1].tokenList), (yyvsp[0].tokenList)); }
#line 1668 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 306 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 1674 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 308 "P1.y" /* yacc.c:1646  */
    { 
					(yyval.tokenList) = listMerge((yyvsp[-3].tokenList), (yyvsp[-2].tokenList));
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList));
					listInsert((yyval.tokenList), ";");
				}
#line 1684 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 313 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 1690 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 315 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), "public");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-11].tokenList));
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-10].tokenList));
					listInsert((yyval.tokenList), "(");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-8].tokenList));
					listInsert((yyval.tokenList), ")");
					listInsert((yyval.tokenList), "{");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-5].tokenList));
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-4].tokenList));
					listInsert((yyval.tokenList), "return");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-2].tokenList));
					listInsert((yyval.tokenList), ";");
					listInsert((yyval.tokenList), "}");
				}
#line 1711 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 332 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), ",");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-2].tokenList));
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList));
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList));
				}
#line 1723 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 339 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 1729 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 341 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = listMerge((yyvsp[-2].tokenList), (yyvsp[-1].tokenList));
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList));
				}
#line 1738 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 345 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 1744 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 347 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = listMerge((yyvsp[-1].tokenList), (yyvsp[0].tokenList)); }
#line 1750 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 348 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 1756 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 350 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "int"); listInsert((yyval.tokenList), "["); listInsert((yyval.tokenList), "]");}
#line 1762 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 351 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "boolean");}
#line 1768 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 352 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "int");}
#line 1774 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 353 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[0].tokenList); }
#line 1780 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 355 "P1.y" /* yacc.c:1646  */
    { 
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), ",");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList));
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList));
				}
#line 1791 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 361 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 1797 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 363 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = listMerge((yyvsp[-1].tokenList), (yyvsp[0].tokenList)); }
#line 1803 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 364 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 1809 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 366 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "{"); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList)); listInsert((yyval.tokenList), "}"); }
#line 1815 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 367 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "System.out.println"); listInsert((yyval.tokenList), "("); (yyval.tokenList) = listMerge((yyval.tokenList),(yyvsp[-2].tokenList)); listInsert((yyval.tokenList), ")"); listInsert((yyval.tokenList), ";"); }
#line 1821 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 368 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[-3].tokenList); listInsert((yyval.tokenList), "="); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList)); listInsert((yyval.tokenList),";"); }
#line 1827 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 369 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[-6].tokenList); listInsert((yyval.tokenList),"["); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-4].tokenList)); listInsert((yyval.tokenList),"]"); listInsert((yyval.tokenList),"="); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList)); listInsert((yyval.tokenList),";");}
#line 1833 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 370 "P1.y" /* yacc.c:1646  */
    { /*Macro*/ (yyval.tokenList) = macroexpand((yyvsp[-4].tokenList), (yyvsp[-2].tokenList), 1); /*$$ = $1; listInsert($$, "("); $$ = listMerge($$, $3); listInsert($$, ")"); listInsert($$, ";");*/ }
#line 1839 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 371 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[0].tokenList); }
#line 1845 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 372 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "while"); listInsert((yyval.tokenList), "("); (yyval.tokenList) = listMerge((yyval.tokenList),(yyvsp[-2].tokenList)); listInsert((yyval.tokenList), ")"); (yyval.tokenList) = listMerge((yyval.tokenList),(yyvsp[0].tokenList)); }
#line 1851 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 374 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), "if");
					listInsert((yyval.tokenList), "(");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-4].tokenList));
					listInsert((yyval.tokenList), ")");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-2].tokenList));
					listInsert((yyval.tokenList), "else");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList));
				}
#line 1866 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 384 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), "if");
					listInsert((yyval.tokenList), "(");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-2].tokenList));
					listInsert((yyval.tokenList), ")");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList));
				}
#line 1879 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 393 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[-2].tokenList); if ((yyvsp[-1].int_val) == 1) listInsert((yyval.tokenList),"||"); else listInsert((yyval.tokenList),"&&"); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList));}
#line 1885 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 394 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[-2].tokenList); if ((yyvsp[-1].int_val) == 1) listInsert((yyval.tokenList),"<="); else listInsert((yyval.tokenList),"!="); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList));}
#line 1891 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 395 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[-2].tokenList); char t[2]; t[0] = (yyvsp[-1].op); t[1] = '\0'; 	  listInsert((yyval.tokenList),t);    (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList));}
#line 1897 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 396 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[-3].tokenList); listInsert((yyval.tokenList),"["); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList)); listInsert((yyval.tokenList),"]"); }
#line 1903 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 397 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[-5].tokenList); listInsert((yyval.tokenList),"."); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-3].tokenList)); listInsert((yyval.tokenList),"("); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList)); listInsert((yyval.tokenList),")"); }
#line 1909 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 398 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[-2].tokenList); listInsert((yyval.tokenList),"."); listInsert((yyval.tokenList),"length"); }
#line 1915 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 399 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[0].tokenList); }
#line 1921 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 400 "P1.y" /* yacc.c:1646  */
    { /*Macro*/ (yyval.tokenList) = macroexpand((yyvsp[-3].tokenList), (yyvsp[-1].tokenList), 0); /*$$ = $1; listInsert($$,"("); $$ = listMerge($$, $3); listInsert($$,")");*/ }
#line 1927 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 402 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "new"); listInsert((yyval.tokenList), "int"); listInsert((yyval.tokenList), "["); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList)); listInsert((yyval.tokenList), "]"); }
#line 1933 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 403 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "new"); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-2].tokenList)); listInsert((yyval.tokenList), "("); listInsert((yyval.tokenList), ")"); }
#line 1939 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 404 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "("); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList)); listInsert((yyval.tokenList), ")"); }
#line 1945 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 405 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[0].tokenList); }
#line 1951 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 406 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "!"); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList)); }
#line 1957 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 408 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[0].tokenList); }
#line 1963 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 409 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "true");}
#line 1969 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 410 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "false");}
#line 1975 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 411 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[0].tokenList); }
#line 1981 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 412 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), "this");}
#line 1987 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 414 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[0].tokenList); }
#line 1993 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 415 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = (yyvsp[0].tokenList); }
#line 1999 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 417 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); listInsert((yyval.tokenList), ","); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList)); (yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[0].tokenList)); }
#line 2005 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 418 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 2011 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 420 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = listMerge((yyvsp[-1].tokenList), (yyvsp[0].tokenList)); }
#line 2017 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 421 "P1.y" /* yacc.c:1646  */
    { (yyval.tokenList) = initList(); }
#line 2023 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 423 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), "#define");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-6].tokenList));
					listInsert((yyval.tokenList), "(");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-4].tokenList));
					listInsert((yyval.tokenList), ")");
					listInsert((yyval.tokenList), "{");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList));
					listInsert((yyval.tokenList), "}");
				}
#line 2039 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 435 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), "#define");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-6].tokenList));
					listInsert((yyval.tokenList), "(");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-4].tokenList));
					listInsert((yyval.tokenList), ")");
					listInsert((yyval.tokenList), "(");
					(yyval.tokenList) = listMerge((yyval.tokenList), (yyvsp[-1].tokenList));
					listInsert((yyval.tokenList), ")");
				}
#line 2055 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 447 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), (yyvsp[0].name));	
				}
#line 2064 "P1.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 452 "P1.y" /* yacc.c:1646  */
    {
					(yyval.tokenList) = initList();
					listInsert((yyval.tokenList), (yyvsp[0].name));
				}
#line 2073 "P1.tab.c" /* yacc.c:1646  */
    break;


#line 2077 "P1.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 456 "P1.y" /* yacc.c:1906  */
