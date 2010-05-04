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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "kite_compiler.h"
#include "kite_ast_base.h"

#define YYPARSE_PARAM parm
#define YYLEX_PARAM parm

/*
 * Dirty, dirty hack to make yyerror() work with Kite's exception system.
 * Credit: http://www.everything2.com/index.pl?node_id=1509675
 */
#define yyerror( msg )  dirty_yyerror( parm, msg )
void dirty_yyerror(void *, char *);
void checkReserved(void *, char *);
	
/* 
 * For some weird reason, we had to split the macro out this way
 * in order for the file/line numbering in exceptions to work properly.
 */
extern void *yyget_extra(void *);
extern int yylex(void *, void *, void *);

%}

%token IMPORT
%token METHOD
%token SYMBOL_NAME
%token FROM
%token CLASS
%token CONSTRUCT
%token DESTRUCT
%token PROPERTY
%token GLOBAL
%token PLUS
%token MINUS
%token MULTIPLY
%token DIVIDE
%token UNARY_PLUS
%token UNARY_MINUS
%token MAP
%token REDUCE
%token ARRAY_DEREF
%token EQUALS
%token NOT_EQUALS
%token LESS_THAN
%token GREATER_THAN
%token LESS_OR_EQUALS
%token GREATER_OR_EQUALS
%token AND
%token OR
%token NOT
%token STRING_VALUE
%token FLOAT_VALUE
%token INT_VALUE
%token MOD
%token OPERATOR
%token XOR
%token LEFT_SHIFT
%token RIGHT_SHIFT
%token EVAL
%token DECIDE
%token BOOL_VALUE
%token UNTIL
%token WHILE
%token RUN
%token CATCH
%token IS
%token ISOF
%token MAKE
%token METHOD_CALL
%token NULL_VALUE
%token DOCSTRING
%token BREAK
%token CONTINUE
%token RETURN
%token VERSION_KEYWORD
%token REGEX_VALUE
%token REGEX_VALUE_CI
%token ARRAY_SET

/* Needed since we can't quite lay out the associativity using the rules yet. */
%left MAP
%left REDUCE

%error-verbose
%defines
%pure-parser
%locations
%union {
    double doubleValue;
    long intValue;
    char *stringValue;
    void *opValue;
}

%%

input:    /* empty */
    | DOCSTRING input 
    | VERSION_KEYWORD STRING_VALUE ';' input
    | statement ';' input
    ;

statement:
    | data_manipulation_statement
    | loop_statement
    | from_statement
    | import_statement
    | break_statement
    | continue_statement
    | return_statement
    | class_definition
    | property_definition
    | decide_statement
    | run_catch_statement
    | constructor_definition
    | destructor_definition
    | operator_definition
    ;
    
import_statement:    IMPORT STRING_VALUE
    ;

break_statement:    BREAK
    ;

continue_statement: CONTINUE
    ;

return_statement: RETURN
    ;
        
optional_docstring:
    | DOCSTRING
    ;
        
method_declaration: METHOD SYMBOL_NAME optional_param_name_list optional_docstring '[' input ']'
    ;

anon_method_declaration: METHOD optional_param_name_list '[' input ']'
    ;
        
optional_global:   
    | GLOBAL
    ;
    
optional_param_name_list:
    | param_name_list
    ;
    
param_name_list:    '(' ')'
    | '(' param_elements ')'
    ;
    
param_elements: SYMBOL_NAME optional_docstring
	| SYMBOL_NAME optional_docstring ',' param_elements
    ;
    
class_definition:    CLASS SYMBOL_NAME optional_from optional_docstring '[' class_body ']'
    ;
    
optional_from:
    | from_statement
    ;

from_statement:
    FROM variable_immediate
    ;
    
variable_immediate: SYMBOL_NAME
    | variable_immediate '.' SYMBOL_NAME
    ;
    
class_body:    
    | class_elements
    ;

class_elements:    class_element
    | class_elements ',' class_element
    ;
    
class_element:    constructor_definition
    | destructor_definition
    | property_definition
    | operator_definition
    | class_definition
    | data_manipulation_statement
    ;

constructor_definition:    CONSTRUCT optional_param_name_list optional_docstring '[' input ']' 
	;
    
destructor_definition: DESTRUCT optional_docstring '[' input ']'
    ;
    
property_definition:    optional_global PROPERTY SYMBOL_NAME optional_docstring
	| optional_global PROPERTY SYMBOL_NAME optional_docstring '=' data_manipulation_statement
	;

operator_definition:    OPERATOR operator_type param_name_list optional_docstring '[' input ']'
    ;
    
operator_type:    PLUS
	| MINUS
    | MULTIPLY
    | DIVIDE
    | MOD
    | UNARY_PLUS
    | UNARY_MINUS
    | MAP
    | REDUCE
    | ARRAY_DEREF
    | ARRAY_SET
    | EQUALS
    | NOT_EQUALS
    | LESS_THAN
    | GREATER_THAN
    | LESS_OR_EQUALS
    | GREATER_OR_EQUALS
    | AND
    | OR
    | NOT
    | XOR
    | LEFT_SHIFT
	| RIGHT_SHIFT
    | METHOD_CALL
    | PROPERTY
    ;

data_manipulation_statement:    assignment_statement
    | method_declaration
    | eval_statement
    ;
    
assignment_statement:    deref_statement '=' data_manipulation_statement
    | or_statement
    ;
    
or_statement:        xor_statement OR or_statement
    | xor_statement
    ;

xor_statement:    xor_statement XOR and_statement
    | and_statement
    ;
    
and_statement:    bitwise_statement AND and_statement
	| bitwise_statement
    ;

bitwise_statement:    bitwise_statement LEFT_SHIFT equality_statement
    | bitwise_statement RIGHT_SHIFT equality_statement
    | equality_statement
    ;
    
equality_statement:    equality_statement EQUALS less_greater_statement
	| equality_statement NOT_EQUALS less_greater_statement
	| equality_statement IS less_greater_statement
	| equality_statement ISOF less_greater_statement
    | less_greater_statement
    ;
    
less_greater_statement:    less_greater_statement '<' add_sub_statement
	| less_greater_statement '>' add_sub_statement
	| less_greater_statement LESS_OR_EQUALS add_sub_statement
	| less_greater_statement GREATER_OR_EQUALS add_sub_statement
	| add_sub_statement
    ;
    
add_sub_statement:    add_sub_statement '+' multdiv_statement
    | add_sub_statement '-' multdiv_statement
    | multdiv_statement
    ;
    
multdiv_statement:    multdiv_statement '*' unary_statement
    | multdiv_statement '/' unary_statement
    | multdiv_statement '%' unary_statement
    | unary_statement
    ;
    
unary_statement: '+' unary_statement
    | '-' unary_statement
    | NOT unary_statement
    | map_reduce_statement
    ;
    
map_reduce_statement:    deref_statement MAP deref_statement
    | deref_statement REDUCE deref_statement
    | SYMBOL_NAME param_list
    | deref_statement
    ;
    
deref_statement:    deref_statement '|' SYMBOL_NAME optional_param_list
    | deref_statement '[' data_manipulation_statement ']'
    | deref_statement '.' SYMBOL_NAME
    | SYMBOL_NAME
    | constant_statement
    ;

optional_param_list:
	| param_list
    ;

param_list:    '(' ')'
    | '(' params_list ')'
    ;

params_list: data_manipulation_statement
    | data_manipulation_statement ',' params_list
    ;
    
var_name:    SYMBOL_NAME 
	| var_name '.' SYMBOL_NAME
    ;
    
constant_statement:    STRING_VALUE
    | INT_VALUE
    | FLOAT_VALUE
    | BOOL_VALUE
    | NULL_VALUE
    | INT_VALUE ':' INT_VALUE
    | INT_VALUE ':' INT_VALUE ':' INT_VALUE
    | REGEX_VALUE
    | REGEX_VALUE_CI
    | list_value
    | construct_object_statement
    | anon_method_declaration
    | '(' data_manipulation_statement ')'
    ;

construct_object_statement:    MAKE var_name param_list    ;
    
list_value: '[' ']'
    | '[' list_elements ']'
    ;
    
list_elements: list_element
    | list_element ',' list_elements
    ;

list_element:    or_statement
    ;
    
decide_statement:    DECIDE '[' decide_contents ']'
    ;
    
decide_contents:    decide_condition
    | decide_contents ',' decide_condition
    ;
    
decide_condition:    constant_statement '[' input ']'
    ;
    
eval_statement:    EVAL or_statement
    ;

loop_statement:    loop_type '[' input ']'
    ;

loop_type:    WHILE constant_statement
    | UNTIL constant_statement
    ;

run_catch_statement: RUN '[' input ']' CATCH '[' input ']'
    ;
    
%%

void dirty_yyerror(void *yyparse_param, char *str) 
{
    kite_compiler_t *comp = (kite_compiler_t*)yyget_extra(yyparse_param);
	fprintf(stderr, "Error encountered during compile: %s", str);
	exit(-1);
}

void checkReserved(void *p, char *name)
{
    if (!strcmp(name, "__exc") ||
        !strcmp(name, "this"))
	{
        dirty_yyerror(p, "Cannot use reserved names for parameters.");
    }
}
