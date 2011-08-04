/**
 * Copyright (c) 2010-2015, Ranjith TV
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer
 *     in the documentation and/or other materials provided with the distribution.
 *   * Neither the name the Ranjith nor the names of its contributors may be used
       to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
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
