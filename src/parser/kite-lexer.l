%{
/*****************************************************************************
 * Copyright (c) 2007, Mooneer Salem
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
 
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <string>
#include "kite-driver.hh"
#include "kite-parser.hh"

/* Work around an incompatibility in flex (at least versions
   2.5.31 through 2.5.33): it generates code that does
   not conform to C89.  See Debian bug 333231
   <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.  */
#undef yywrap
#define yywrap() 1
 
/* By default yylex returns int, we use token_type.
   Unfortunately yyterminate by default returns 0, which is
   not of token_type.  */
#define yyterminate() return token::END
%}

%x operator str sym docstr regex str2
%option noyywrap nounput batch debug

blank [ \t]

%{
#define YY_USER_ACTION  yylloc->columns (yyleng);
%}

%%

%{
    yylloc->step ();
%}

{blank}+   yylloc->step ();
[\n]+      yylloc->lines (yyleng); yylloc->step ();

#[^\r\n]*  yylloc->step ();

%{
    typedef yy::kite_parser::token token;
%}

import                  return token::IMPORT;
method                  return token::METHOD;
from                    return token::FROM; 
class                   return token::CLASS;
construct               return token::CONSTRUCT;
destruct                return token::DESTRUCT;
property                return token::PROPERTY;
global                  return token::GLOBAL;
operator                BEGIN(operator); return token::OPERATOR;
eval                    return token::EVAL;
decide                  return token::DECIDE;
break                   return token::BREAK;
continue                return token::CONTINUE;
return                  return token::RETURN;
until                   return token::UNTIL;
while                   return token::WHILE;
run                     return token::RUN;
catch                   return token::CATCH;
is                      return token::IS;
isof                    return token::ISOF;
make                    return token::MAKE;
and                     return token::AND;
or                      return token::OR;
not                     return token::NOT;
xor                     return token::XOR;
null                    return token::NULL_VALUE;
version                 return token::VERSION_KEYWORD;
options[ \t\r\n]+set    return token::OPTIONS_SET;

"<<"                    return token::LEFT_SHIFT;
">>"                    return token::RIGHT_SHIFT;
"=="                    return token::EQUALS;
"!="                    return token::NOT_EQUALS;
"<="                    return token::LESS_OR_EQUALS;
">="                    return token::GREATER_OR_EQUALS;
"<|"                    return token::REDUCE;
"<-"                    return token::MAP;

true                    yylval->intValue = 1; return BOOL_VALUE;
false                   yylval->intValue = 0; return BOOL_VALUE;

"r/"                    BEGIN(regex); yylval->stringValue = new std::string("");

\"                      BEGIN(str); yylval->stringValue = new std::string("");

\'                      BEGIN(str2); yylval->stringValue = new std::string("");

"/["                    BEGIN(docstr); yylval->stringValue = new std::string("");

<regex>[^/]+            yylval->stringValue->append(yytext);

<regex>"\\/"            yylval->stringValue->append("/");

<regex>"/i"             BEGIN(INITIAL); return token::REGEX_VALUE_CI;

<regex>"/"              BEGIN(INITIAL); return token::REGEX_VALUE;

<docstr>.               yylval->stringValue->append(yytext);

<docstr>\n              yylval->stringValue->append(yytext); yylloc->lines(1); yylloc->step();

<docstr>"]/"            BEGIN(INITIAL); yylval->stringValue->append(yytext); return token::DOCSTRING;

<str>\"                 BEGIN(INITIAL); return token::STRING_VALUE;

<str>\\n                yylval->stringValue->append("\n");

<str>\\t                yylval->stringValue->append("\t");

<str>\\r                yylval->stringValue->append("\r");

<str>\\b                yylval->stringValue->append("\b");

<str>\\f                yylval->stringValue->append("\f");

<str>\\[0-7]{1,3} {
	char buf[2];
	buf[0] = (char)strtoul(yytext + 1, NULL, 8);
	buf[1] = 0;
	yylval->stringValue->append(buf);
}

<str>\\x[0-9A-Fa-f]{1,2} { 
	char buf[2];
	buf[0] = (char)strtoul(yytext + 1, NULL, 16);
	buf[1] = 0;
	yylval->stringValue->append(buf);
}

<str>\\\"               yylval->stringValue->append("\\\"");

<str>\\\\               yylval->stringValue->append("\\\\");

<str>[ \t]+             yylval->stringValue->append(yytext);

<str>\r?\n              yylval->stringValue->append(yytext); yylloc->lines(1); yylloc->step();

<str>.                  yylval->stringValue->append(yytext);

<str2>\'                BEGIN(INITIAL); return token::STRING_VALUE;

<str2>\\n               yylval->stringValue->append("\n");

<str2>\\t               yylval->stringValue->append("\t");

<str2>\\r               yylval->stringValue->append("\r");

<str2>\\b               yylval->stringValue->append("\b");

<str2>\\f               yylval->stringValue->append("\f");

<str2>\\[0-7]{1,3} {
	char buf[2];
	buf[0] = (char)strtoul(yytext + 1, NULL, 8);
	buf[1] = 0;
	yylval->stringValue->append(buf);
}

<str2>\\x[0-9A-Fa-f]{1,2} { 
	char buf[2];
	buf[0] = (char)strtoul(yytext + 1, NULL, 16);
	buf[1] = 0;
	yylval->stringValue->append(buf);
}

<str2>\\\"              yylval->stringValue->append("\\\"");

<str2>\\\\              yylval->stringValue->append("\\\\");

<str2>[ \t]+            yylval->stringValue->append(yytext);

<str2>\r?\n             yylval->stringValue->append(yytext); yylloc->lines(1); yylloc->step();

<str2>.                 yylval->stringValue->append(yytext);

[+-]?[1-9][0-9]*        yylval->intValue = atoi(yytext); return token::INT_VALUE;

[+-]?0[xX][0-9A-Fa-f]+  yylval->intValue = strtol(yytext, NULL, 16); return token::INT_VALUE;

[+-]?0[bB][01]+         yylval->intValue = strtol(yytext + 2, NULL, 2); return token::INT_VALUE;

[+-]?0[0-7]+	        yylval->intValue = strtol(yytext, NULL, 8); return token::INT_VALUE;

[+-]?0                  yylval->intValue = 0; return token::INT_VALUE;

[+-]?(([0-9]*\.([0-9]+)([eE][-+]?[0-9]+)?)|[0-9]+([eE][-+]?[0-9]+)) { 
	yylval->doubleValue = atof(yytext);
	return token::FLOAT_VALUE;
}

\||\. {
	/* so reserved words can be used as valid symbols */ 
	BEGIN(sym);
	return yytext[0];
}

<INITIAL,sym>[A-Za-z_][A-Za-z0-9_]* { 
	BEGIN(INITIAL);
	yylval->stringValue = new std::string(yytext);
	return token::SYMBOL_NAME;
}

<sym,operator,INITIAL>.         if (YYSTATE == INITIAL) return yytext[0];

<operator>"plus"                yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return PLUS;
<operator>"minus"               yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return MINUS;
<operator>"multiply"            yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return MULTIPLY;
<operator>"divide"              yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return DIVIDE;
<operator>"mod"                 yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return MOD;
<operator>"unplus"              yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return UNARY_PLUS;
<operator>"unminus"             yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return UNARY_MINUS;
<operator>"map"                 yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return MAP;
<operator>"reduce"              yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return REDUCE;
<operator>"array"               yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return ARRAY_DEREF;
<operator>"array_set"           yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return ARRAY_SET;
<operator>"equals"              yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return EQUALS;
<operator>"nequals"             yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return NOT_EQUALS;
<operator>"lt"                  yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return LESS_THAN;
<operator>"gt"                  yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return GREATER_THAN;
<operator>"leq"                 yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return LESS_OR_EQUALS;
<operator>"geq"                 yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return GREATER_OR_EQUALS;
<operator>"and"                 yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return AND;
<operator>"or"                  yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return OR;
<operator>"not"                 yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return NOT;
<operator>"xor"                 yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return XOR;
<operator>"lshift"              yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return LEFT_SHIFT;
<operator>"rshift"              yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return RIGHT_SHIFT;
<operator>"call"                yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return METHOD_CALL;
<operator>"property"            yylval->stringValue = new std::string(yytext); BEGIN(INITIAL); return PROPERTY;

%%

void
kite_driver::scan_begin ()
{
    yy_flex_debug = trace_scanning;
    if (file.empty () || file == "-")
        yyin = stdin;
    else if (!(yyin = fopen (file.c_str (), "r")))
    {
        error ("cannot open " + file + ": " + strerror(errno));
    }
}
     
void
kite_driver::scan_end ()
{
    fclose (yyin);
}