
%{
    #define YYSTYPE double
    #include <math.h>
    #include <string.h>
    #include "rmk.h"
    
    int yylex ();
    void yyerror (char const *);
%}

%union {
	int	   intVal;
	double doubVal;
	char * strVal;
	struct NameValPair * pair;
}

%token <intVal> NUM BINPRG SOURCE HEADER INCLUDE LIBRARY
%token <strVal> WORD

%type <intVal> property
%type <strVal> value
%type <pair> values

%start input

%%

input:
      | input line
      ;
      
line : '\n'
	  | rules '\n'
	  ;

rules : property '=' values {addNameValPairProperty($1, $3);}
	  ;	
	  
values : value values {$$ = addNVPair($2, 0, $1);free($1);}
        | value		{$$ = createNVPair(0, $1); free($1);}
        ;
        
value : WORD {$$ = (char *) malloc(strlen($1)+2); strcpy($$, $1);}
	  ;
	  
property : BINPRG	{$$ = $1;}
         | SOURCE   {$$ = $1;}
         | HEADER   {$$ = $1;}
         | LIBRARY	{$$ = $1;}
         | INCLUDE	{$$ = $1;}
         ;  
