
%{
#include <math.h>
#include "rmk.tab.h"
int currentline = 0;
%}

DIGIT       [0-9]
WORD        [_A-Za-z][_A-Za-z0-9]+
FILENAME    [_A-Za-z0-9]*(\.[_A-Za-z0-9])?
FILEPATH    (\/?[_A-Za-z0-9]*|\.{0,2}\/?)+(\.[_A-Za-z0-9])?
%%

{DIGIT}+    { yylval.doubVal = atoi(yytext);
			  return NUM;}

binprg  { yylval.intVal = BINPRG; return BINPRG; }
source  { yylval.intVal = SOURCE; return SOURCE; }
header  { yylval.intVal = HEADER; return HEADER; }
library { yylval.intVal = LIBRARY; return LIBRARY;}
include { yylval.intVal = INCLUDE; return INCLUDE;}

{WORD}     {yylval.strVal = yytext;  return WORD; }
{FILEPATH} {yylval.strVal = yytext;  return WORD;}

[ \t]+		{}
[\n]		{currentline++; return yytext[0];}
.			{return yytext[0];}

%%
