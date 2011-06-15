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
