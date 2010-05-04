%{
/*****************************************************************************
 * Copyright (c) 2010, Mooneer Salem
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Kite Language organization nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MOONEER SALEM ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MOONEER SALEM BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/	
%}

%x operator str sym docstr regex str2
%option noyywrap
%option bison-bridge bison-locations reentrant
%{
#include "y.tab.h"
#include "kite_compiler.h"

static void fillFirst(yyscan_t scanner);
static void fillLast(yyscan_t scanner);
static void fillBoth(yyscan_t scanner);

#define APPEND_STRING(str) \
	{ \
		compiler->curStr = \
			realloc(compiler->curStr, \
					strlen(compiler->curStr) + strlen(str) + 1); \
		strcat(compiler->curStr, str); \
	}

#define APPEND_CHAR(chr) \
	{ \
		char buf[2]; \
		buf[0] = chr; \
		buf[1] = 0; \
		compiler->curStr = \
			realloc(compiler->curStr, strlen(compiler->curStr) + 2); \
		strcat(compiler->curStr, buf); \
	}
	
%}

%%

#[^\r\n]* {  }

import	{ fillBoth(yyscanner); return IMPORT; }
method { fillBoth(yyscanner); return METHOD; }
from { fillBoth(yyscanner); return FROM; }
class { fillBoth(yyscanner); return CLASS; }
construct { fillBoth(yyscanner); return CONSTRUCT; }
destruct { fillBoth(yyscanner); return DESTRUCT; }
property { fillBoth(yyscanner); return PROPERTY; }
global { fillBoth(yyscanner); return GLOBAL; }
operator { BEGIN(operator); fillBoth(yyscanner); return OPERATOR; }
eval { fillBoth(yyscanner); return EVAL; }
decide { fillBoth(yyscanner); return DECIDE; }
break { fillBoth(yyscanner); return BREAK; }
continue { fillBoth(yyscanner); return CONTINUE; }
return { fillBoth(yyscanner); return RETURN; }
until { fillBoth(yyscanner); return UNTIL; }
while { fillBoth(yyscanner); return WHILE; }
run { fillBoth(yyscanner); return RUN; }
catch { fillBoth(yyscanner); return CATCH; }
is { fillBoth(yyscanner); return IS; }
isof { fillBoth(yyscanner); return ISOF; }
make { fillBoth(yyscanner); return MAKE; }
and { fillBoth(yyscanner); return AND; }
or { fillBoth(yyscanner); return OR; }
not { fillBoth(yyscanner); return NOT; }
xor { fillBoth(yyscanner); return XOR; }
null { fillBoth(yyscanner); return NULL_VALUE; }
version { fillBoth(yyscanner); return VERSION_KEYWORD; }

"<<" { fillBoth(yyscanner); return LEFT_SHIFT; }
">>" { fillBoth(yyscanner); return RIGHT_SHIFT; }
"==" { fillBoth(yyscanner); return EQUALS; }
"!=" { fillBoth(yyscanner); return NOT_EQUALS; }
"<=" { fillBoth(yyscanner); return LESS_OR_EQUALS; }
">=" { fillBoth(yyscanner); return GREATER_OR_EQUALS; }
"<|" { fillBoth(yyscanner); return REDUCE; }
"<-" { fillBoth(yyscanner); return MAP; }

true { fillBoth(yyscanner); yylval->intValue = 1; return BOOL_VALUE; }
false { fillBoth(yyscanner); yylval->intValue = 0; return BOOL_VALUE; }

"r/" {
    kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	fillFirst(yyscanner);
	BEGIN(regex);
	compiler->curStr = strdup("");
}

\" {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	fillFirst(yyscanner);
	BEGIN(str);
	compiler->curStr = strdup("");
}

\' {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	fillFirst(yyscanner);
	BEGIN(str2);
	compiler->curStr = strdup("");
}

"/[" {
    kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	fillFirst(yyscanner);
	BEGIN(docstr);
	compiler->curStr = strdup("");
}

<regex>[^/]+ {
    kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING(yytext);
	compiler->currentCol += strlen(yytext);
}

<regex>"\\/" {
    kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("/");
	compiler->currentCol += strlen(yytext);
}

<regex>"/i" {
    kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	fillLast(yyscanner);
	BEGIN(INITIAL);
	yylval->stringValue = compiler->curStr;
	return REGEX_VALUE_CI;
}

<regex>"/" {
    kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	fillLast(yyscanner);
	BEGIN(INITIAL);
	yylval->stringValue = compiler->curStr;
	return REGEX_VALUE;
}

<docstr>. {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING(yytext);
	compiler->currentCol++;
}

<docstr>\n {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING(yytext);
	compiler->currentCol++;
}

<docstr>"]/" {
    kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	fillLast(yyscanner);
	BEGIN(INITIAL);
	yylval->stringValue = compiler->curStr;
	return DOCSTRING;
}

<str>\" {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	fillLast(yyscanner);
	BEGIN(INITIAL);
	yylval->stringValue = compiler->curStr;
	return STRING_VALUE;
}
<str>\\n {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\n");
	compiler->currentCol++;
}
<str>\\t {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\t");
	compiler->currentCol++;
}
<str>\\r {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\r");
	compiler->currentCol++;
}
<str>\\b {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\b");
	compiler->currentCol++;
}
<str>\\f {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\f");
	compiler->currentCol++;
}
<str>\\[0-7]{1,3} { 
	unsigned int character = strtoul(yytext + 1, NULL, 8);
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_CHAR(character);
	compiler->currentCol += strlen(yytext);
}
<str>\\x[0-9A-Fa-f]{1,2} { 
	unsigned int character = strtoul(yytext + 2, NULL, 16);
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_CHAR(character);
	compiler->currentCol += strlen(yytext);
}
<str>\\\" { 
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\"");
	compiler->currentCol += strlen(yytext);
}
<str>\\\\ { 
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\\");
	compiler->currentCol += strlen(yytext);
}
<str>[ \t]+ {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING(yytext);
}
<str>\r?\n {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING(yytext);
	compiler->currentLine++; compiler->currentCol = 1; 
}
<str>. {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING(yytext);
	compiler->currentCol++;
}

<str2>\' {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	fillLast(yyscanner);
	BEGIN(INITIAL);
	yylval->stringValue = compiler->curStr;
	return STRING_VALUE;
}
<str2>\\n {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\n");
	compiler->currentCol++;
}
<str2>\\t {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\t");
	compiler->currentCol++;
}
<str2>\\r {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\r");
	compiler->currentCol++;
}
<str2>\\b {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\b");
	compiler->currentCol++;
}
<str2>\\f {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\f");
	compiler->currentCol++;
}
<str2>\\[0-7]{1,3} { 
	unsigned int character = strtoul(yytext + 1, NULL, 8);
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_CHAR(character);
	compiler->currentCol += strlen(yytext);
}
<str2>\\x[0-9A-Fa-f]{1,2} { 
	unsigned int character = strtoul(yytext + 2, NULL, 16);
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_CHAR(character);
	compiler->currentCol += strlen(yytext);
}
<str2>\\\" { 
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\"");
	compiler->currentCol += strlen(yytext);
}
<str2>\\\\ { 
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING("\\");
	compiler->currentCol += strlen(yytext);
}
<str2>[ \t]+ {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING(yytext);
}
<str2>\r?\n {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING(yytext);
	compiler->currentLine++; compiler->currentCol = 1; 
}
<str2>. {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	APPEND_STRING(yytext);
	compiler->currentCol++;
}
[+-]?[1-9][0-9]* { 
	fillBoth(yyscanner);
	yylval->intValue = atoi(yytext);
	return INT_VALUE;
}
[+-]?0[xX][0-9A-Fa-f]+ { 
	fillBoth(yyscanner);
	yylval->intValue = strtol(yytext, NULL, 16);
	return INT_VALUE;
}
[+-]?0[bB][01]+ { 
	fillBoth(yyscanner);
	yylval->intValue = strtol(yytext + 2, NULL, 2);
	return INT_VALUE;
}
[+-]?0[0-7]+ { 
	fillBoth(yyscanner);
	yylval->intValue = strtol(yytext, NULL, 8);
	return INT_VALUE;
}
[+-]?0 { 
	fillBoth(yyscanner);
	yylval->intValue = 0;
	return INT_VALUE;
}
[+-]?(([0-9]*\.([0-9]+)([eE][-+]?[0-9]+)?)|[0-9]+([eE][-+]?[0-9]+)) { 
	fillBoth(yyscanner);
	yylval->doubleValue = atof(yytext);
	return FLOAT_VALUE;
}
\||\. {
	/* so reserved words can be used as valid symbols */ 
	BEGIN(sym);
	fillBoth(yyscanner);
	return yytext[0];
}
<INITIAL,sym>[A-Za-z_][A-Za-z0-9_]* { 
	fillBoth(yyscanner); 
	BEGIN(INITIAL);
	yylval->stringValue = strdup(yytext);
	return SYMBOL_NAME;
}
[ \t]+ { 
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	compiler->currentCol += strlen(yytext);
}
\r?\n {
	kite_compiler_t *compiler = (kite_compiler_t*)yyextra;
	compiler->currentLine++; compiler->currentCol = 1; 
}
<sym,operator,INITIAL>. { fillBoth(yyscanner); if (YYSTATE == INITIAL) return yytext[0]; }

<operator>"plus" { fillBoth(yyscanner); BEGIN(INITIAL); return PLUS; }
<operator>"minus" { fillBoth(yyscanner); BEGIN(INITIAL); return MINUS; }
<operator>"multiply" { fillBoth(yyscanner); BEGIN(INITIAL); return MULTIPLY; }
<operator>"divide" { fillBoth(yyscanner); BEGIN(INITIAL); return DIVIDE; }
<operator>"mod" { fillBoth(yyscanner); BEGIN(INITIAL); return MOD; }
<operator>"unplus" { fillBoth(yyscanner); BEGIN(INITIAL); return UNARY_PLUS; }
<operator>"unminus" { fillBoth(yyscanner); BEGIN(INITIAL); return UNARY_MINUS; }
<operator>"map" { fillBoth(yyscanner); BEGIN(INITIAL); return MAP; }
<operator>"reduce" { fillBoth(yyscanner); BEGIN(INITIAL); return REDUCE; }
<operator>"array" { fillBoth(yyscanner); BEGIN(INITIAL); return ARRAY_DEREF; }
<operator>"array_set" { fillBoth(yyscanner); BEGIN(INITIAL); return ARRAY_SET; }
<operator>"equals" { fillBoth(yyscanner); BEGIN(INITIAL); return EQUALS; }
<operator>"nequals" { fillBoth(yyscanner); BEGIN(INITIAL); return NOT_EQUALS; }
<operator>"lt" { fillBoth(yyscanner); BEGIN(INITIAL); return LESS_THAN; }
<operator>"gt" { fillBoth(yyscanner); BEGIN(INITIAL); return GREATER_THAN; }
<operator>"leq" { fillBoth(yyscanner); BEGIN(INITIAL); return LESS_OR_EQUALS; }
<operator>"geq" { fillBoth(yyscanner); BEGIN(INITIAL); return GREATER_OR_EQUALS; }
<operator>"and" { fillBoth(yyscanner); BEGIN(INITIAL); return AND; }
<operator>"or" { fillBoth(yyscanner); BEGIN(INITIAL); return OR; }
<operator>"not" { fillBoth(yyscanner); BEGIN(INITIAL); return NOT; }
<operator>"xor" { fillBoth(yyscanner); BEGIN(INITIAL); return XOR; }
<operator>"lshift" { fillBoth(yyscanner); BEGIN(INITIAL); return LEFT_SHIFT; }
<operator>"rshift" { fillBoth(yyscanner); BEGIN(INITIAL); return RIGHT_SHIFT; }
<operator>"call" { fillBoth(yyscanner); BEGIN(INITIAL); return METHOD_CALL; }
<operator>"property" { fillBoth(yyscanner); BEGIN(INITIAL); return PROPERTY; }

%%

static void fillFirst(yyscan_t scanner) {
	YYLTYPE *t = yyget_lloc(scanner);
	kite_compiler_t *compiler = (kite_compiler_t*)yyget_extra(scanner);
	t->first_line = compiler->currentLine;
	t->first_column = compiler->currentCol;
	t->last_column = 0;
	yyset_lloc(t, scanner);
}

static void fillLast(yyscan_t scanner) {
	YYLTYPE *t = yyget_lloc(scanner);
	kite_compiler_t *compiler = (kite_compiler_t*)yyget_extra(scanner);
	compiler->currentCol += strlen(yyget_text(scanner));
	t->last_line = compiler->currentLine;
	t->last_column = compiler->currentCol;
	yyset_lloc(t, scanner);
}

static void fillBoth(yyscan_t scanner) {
	fillFirst(scanner);
	fillLast(scanner);
}
