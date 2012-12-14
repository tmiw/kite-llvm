%{
/*****************************************************************************
 * Copyright (c) 2012, Mooneer Salem
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

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "2.7"
%defines
%define parser_class_name "kite_parser"
%code requires {
    #include <string>
    class kite_driver;
}

// The parsing context.
%parse-param { kite_driver& driver }
%lex-param   { kite_driver& driver }

%locations
%initial-action
{
    // Initialize the initial location.
    @$.begin.filename = @$.end.filename = &driver.file;
};

%debug
%error-verbose     

%union 
{
    double doubleValue;
    long intValue;
    std::string *stringValue;
    kite::stdlib::System::object *internalValue;
    kite::semantics::syntax_tree *opValue;
    std::vector<kite::semantics::syntax_tree_node> *listValue;
}

%code {
    #include "kite-driver.h"
}

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
%token OPTIONS_SET

/* Needed since we can't quite lay out the associativity using the rules yet. */
%left MAP
%left REDUCE

%%

pre_input: 
      DOCSTRING { /* TODO */ } input { $$ = $3; }
    | VERSION_KEYWORD STRING_VALUE ';' { /* TODO */ } input { $$ = $5; }
    | input { $$ = $1; }
    ;

input:    /* empty */ {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ITERATE;
    }
    | statement ';' input {
        $$ = $3;
        if ($1 != NULL)
        {
            $<opValue>$->children.push_back(*$1);
        }
    }
    ;

statement:
    | data_manipulation_statement
    | options_set_statement
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

options_set_statement:  OPTIONS_SET STRING_VALUE STRING_VALUE {
    /* TODO */
};

import_statement:    IMPORT STRING_VALUE     {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::IMPORT;
        $<opValue>$->children.push_back(*$<stringValue>2);
    }
    ;

break_statement:    BREAK
    {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::BREAK;
    }
    ;

continue_statement: CONTINUE
    {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::CONTINUE;
    }
    ;

return_statement: RETURN
    {
        /* TODO */
    }
    ;
        
optional_docstring:
    {
        $<stringValue>$ = new std::string;
    }
    | DOCSTRING {
        $<stringValue>$ = $<stringValue>1;
    }
    ;
        
method_declaration: METHOD SYMBOL_NAME optional_param_name_list optional_docstring {
        /* TODO: docstring */
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::METHOD;
        $<opValue>$->children.push_back(*$2);
        
        std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>3;
        for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
             i != param_names.end();
             i++)
        {
            $<opValue>$->children.push_back(*i);
        }
    } '[' input ']' {
        $<opValue>$->children.push_back(*$7);
    }
    ;

anon_method_declaration: METHOD optional_param_name_list     {
        /* TODO: docstring */
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::METHOD;
        $<opValue>$->children.push_back(std::string("__AnonMethod"));
        
        std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>2;
        for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
             i != param_names.end();
             i++)
        {
            $<opValue>$->children.push_back(*i);
        }
    } '[' input ']' {
        $<opValue>$->children.push_back(*$5);
    }
    ;
        
optional_global:    {
        $<intValue>$ = 0;
    }
    | GLOBAL    {
        $<intValue>$ = 1;
    }
    ;
    
optional_param_name_list:    {
        $<listValue>$ = new std::vector<kite::semantics::syntax_tree_node>();
    }
    | param_name_list    {
        $$ = $1;
    }
    ;
    
param_name_list:    '(' ')'    {
        $<listValue>$ = new std::vector<kite::semantics::syntax_tree_node>();
    }
    | '(' param_elements ')'    {
        $$ = $2;
    }
    ;
    
param_elements: SYMBOL_NAME optional_docstring   {
        /* TODO: docstring */
        $<listValue>$ = new std::vector<kite::semantics::syntax_tree_node>();
        $<listValue>$->push_back(*$<stringValue>1);
    }
    | SYMBOL_NAME optional_docstring ',' param_elements {
        /* TODO: docstring */
        $<listValue>4->push_back(*$<stringValue>1);
        $$ = $4;
    }
    ;
    
class_definition:    CLASS SYMBOL_NAME {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::CLASS;
        $<opValue>$->children.push_back(*$<stringValue>2);
    } optional_from optional_docstring '[' class_body ']'    {
        /* TODO: docstring */
        if ($4 != NULL)
        {
            $<opValue>$->children.push_back(*$4);
        }
        $<opValue>$->children.push_back(*$7);
    }
    ;
    
optional_from:        {
        $<opValue>$ = NULL;
    }
    | from_statement { $$ = $1; }
    ;

from_statement:
    FROM deref_statement   {
        $$ = $2;
    }
    ;
    
class_body:    
    {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ITERATE; 
    }
    | class_elements { $$ = $1; }
    ;

class_elements:    class_element
    {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ITERATE;
        $<opValue>$->children.push_back(*$1);
    }
    | class_elements ',' class_element
    {
        $$ = $1;
        $<opValue>$->children.push_back(*$3);
    }
    ;
    
class_element:    constructor_definition { $$ = $1; }
    | destructor_definition { $$ = $1; }
    | property_definition { $$ = $1; }
    | operator_definition { $$ = $1; }
    | class_definition { $$ = $1; }
    | data_manipulation_statement { $$ = $1; }
    ;

constructor_definition:    CONSTRUCT optional_param_name_list {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::CONSTRUCTOR;
        
        std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>2;
        for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
             i != param_names.end();
             i++)
        {
            $<opValue>$->children.push_back(*i);
        }
    } optional_docstring '[' input ']' {
        /* TODO: docstring */
        $<opValue>$->children.push_back(*$6);
    }
    ;
    
destructor_definition: DESTRUCT {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DESTRUCTOR;
    } optional_docstring '[' input ']' {
        /* TODO: docstring */
        $<opValue>$->children.push_back(*$5);
    }
    ;
    
property_definition:    optional_global PROPERTY SYMBOL_NAME optional_docstring   {
        /* TODO: global does nothing here. */
        /* TODO: docstring */
    }
    | optional_global PROPERTY SYMBOL_NAME optional_docstring '=' {
        /* TODO: implementation */
        } data_manipulation_statement {
            /* TODO: implementation */
        }
    ;

operator_definition:    OPERATOR operator_type param_name_list {
    $<opValue>$ = new kite::semantics::syntax_tree();
    $<opValue>$->op = kite::semantics::OPERATOR;
    $<opValue>$->children.push_back(*$2);
    
    std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>3;
    for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
         i != param_names.end();
         i++)
    {
        $<opValue>$->children.push_back(*i);
    }
} optional_docstring '[' input ']' {
    /* TODO: docstring */
    $<opValue>$->children.push_back(*$7);
}
    ;
    
operator_type:    
      PLUS              { $$ = $1; }
    | MINUS             { $$ = $1; }
    | MULTIPLY          { $$ = $1; }
    | DIVIDE            { $$ = $1; }
    | MOD               { $$ = $1; }
    | UNARY_PLUS        { $$ = $1; }
    | UNARY_MINUS       { $$ = $1; }
    | MAP               { $$ = $1; }
    | REDUCE            { $$ = $1; }
    | ARRAY_DEREF       { $$ = $1; }
    | ARRAY_SET         { $$ = $1; }
    | EQUALS            { $$ = $1; }
    | NOT_EQUALS        { $$ = $1; }
    | LESS_THAN         { $$ = $1; }
    | GREATER_THAN      { $$ = $1; }
    | LESS_OR_EQUALS    { $$ = $1; }
    | GREATER_OR_EQUALS { $$ = $1; }
    | AND               { $$ = $1; }
    | OR                { $$ = $1; }
    | NOT               { $$ = $1; }
    | XOR               { $$ = $1; }
    | LEFT_SHIFT        { $$ = $1; }
    | RIGHT_SHIFT       { $$ = $1; }
    | METHOD_CALL       { $$ = $1; }
    | PROPERTY          { $$ = $1; }
    ;

data_manipulation_statement:   
      assignment_statement { $$ = $1; }
    | method_declaration   { $$ = $1; }
    | eval_statement       { $$ = $1; }
    ;
    
assignment_statement:    deref_statement {
    $<opValue>$ = new kite::semantics::syntax_tree();
    $<opValue>$->op = kite::semantics::ASSIGN;
    $<opValue>$->children.push_back(*$1);
} '=' data_manipulation_statement {
        $<opValue>$->children.push_back(*$4);
    }
    | or_statement { $$ = $1; }
    ;
    
or_statement:        xor_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::OR;
        $<opValue>$->children.push_back(*$1);
    } OR or_statement {
        $<opValue>$->children.push_back(*$4);
    }
    | xor_statement { $$ = $1; }
    ;

xor_statement:    xor_statement XOR and_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::XOR;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | and_statement { $$ = $1; }
    ;
    
and_statement:    bitwise_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::AND;
        $<opValue>$->children.push_back(*$1);
    } AND and_statement {
        $<opValue>$->children.push_back(*$4);
    }
    | bitwise_statement { $$ = $1; }
    ;

bitwise_statement:    bitwise_statement LEFT_SHIFT equality_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::LEFT_SHIFT;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | bitwise_statement RIGHT_SHIFT equality_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::RIGHT_SHIFT;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | equality_statement { $$ = $1; }
    ;
    
equality_statement:    equality_statement EQUALS less_greater_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::EQUALS;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | equality_statement NOT_EQUALS less_greater_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::NOT_EQUALS;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | equality_statement IS less_greater_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::IS_CLASS;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | equality_statement ISOF less_greater_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ISOF_CLASS;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | less_greater_statement { $$ = $1; }
    ;
    
less_greater_statement:    less_greater_statement '<' add_sub_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::LESS_THAN;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | less_greater_statement '>' add_sub_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::GREATER_THAN;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | less_greater_statement LESS_OR_EQUALS add_sub_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::LESS_OR_EQUALS;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | less_greater_statement GREATER_OR_EQUALS add_sub_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::GREATER_OR_EQUALS;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | add_sub_statement { $$ = $1; }
    ;
    
add_sub_statement:    add_sub_statement '+' multdiv_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ADD;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | add_sub_statement '-' multdiv_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::SUBTRACT;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | multdiv_statement { $$ = $1; }
    ;
    
multdiv_statement:    multdiv_statement '*' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::MULTIPLY;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | multdiv_statement '/' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DIVIDE;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | multdiv_statement '%' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::MODULO;
        $<opValue>$->children.push_back(*$1);
        $<opValue>$->children.push_back(*$3);
    }
    | unary_statement { $$ = $1; }
    ;
    
unary_statement: '+' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::UNARY_PLUS;
        $<opValue>$->children.push_back(*$2);
    }
    | '-' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::UNARY_MINUS;
        $<opValue>$->children.push_back(*$2);
    }
    | NOT unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::NOT;
        $<opValue>$->children.push_back(*$2);
    }
    | map_reduce_statement { $$ = $1; }
    ;
    
map_reduce_statement:    deref_statement MAP deref_statement {
        (*kite_compiler_actions[COMPILE_MAP])((kite_compiler_t*)yyget_extra(parm), @2.first_line);
    }
    | deref_statement REDUCE deref_statement {
        (*kite_compiler_actions[COMPILE_REDUCE])((kite_compiler_t*)yyget_extra(parm), @2.first_line);
    }
    | SYMBOL_NAME { 
        (*kite_compiler_actions[COMPILE_FUNC_CALL_BEGIN])((kite_compiler_t*)yyget_extra(parm), @1.first_line, $<stringValue>1);
        free($<stringValue>1);
    } param_list {
        (*kite_compiler_actions[COMPILE_FUNC_CALL_ARGS])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    }
    | deref_statement
    ;
    
deref_statement:    deref_statement '|' SYMBOL_NAME {
        (*kite_compiler_actions[COMPILE_DEREF_AND_CALL_BEGIN])((kite_compiler_t*)yyget_extra(parm), @2.first_line, $<stringValue>3);
        free($<stringValue>3);
    } optional_param_list {
        (*kite_compiler_actions[COMPILE_DEREF_AND_CALL_ARGS])((kite_compiler_t*)yyget_extra(parm), @3.first_line, $<opValue>5);
        $<opValue>$ = $<opValue>1;
    }
    | deref_statement '[' data_manipulation_statement ']'    {
        (*kite_compiler_actions[COMPILE_ARRAY_DEREF])((kite_compiler_t*)yyget_extra(parm), @3.first_line);
        $<opValue>$ = $<opValue>1;
    }
    | deref_statement '.' SYMBOL_NAME {
        (*kite_compiler_actions[COMPILE_DEREF_2])((kite_compiler_t*)yyget_extra(parm), @3.first_line, $<stringValue>3);
        free($<stringValue>3);
        $<opValue>$ = $<opValue>1;
    }
    | SYMBOL_NAME {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_DEREF_1])((kite_compiler_t*)yyget_extra(parm), @1.first_line, $<stringValue>1);
        free($<stringValue>1);
    }
    | constant_statement {
        $<opValue>$ = $<opValue>1;
    }
    ;

optional_param_list: {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_EMPTY_PARAM_LIST])((kite_compiler_t*)yyget_extra(parm), @0.first_line);
    }
    | param_list {
        $<opValue>$ = $<opValue>1;
    }
    ;

param_list:    '(' ')' {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_EMPTY_PARAM_LIST])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    }
    | '(' params_list ')' {
        $<opValue>$ = $<opValue>2;
    }
    ;

params_list: data_manipulation_statement    {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_SINGLE_FUNC_PARAM])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    }
    | data_manipulation_statement ',' params_list    {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_COMBINE_FUNC_PARAMS])((kite_compiler_t*)yyget_extra(parm), @2.first_line);
    }
    ;
    
var_name:    SYMBOL_NAME {
        /*kite_compiler_t *compiler = (kite_compiler_t*)yyget_extra(parm);
        kite_thread_t *thd = compiler->thd;
        kite_opcode_t *op = kite_compile_push(kite_new_ident(thd, $<stringValue>1));
        COMPILE_INSTRUCTION(op, @1.first_line);
        COMPILE_INSTRUCTION(kite_compile_deref_1(TRUE), @1.first_line);
        free($<stringValue>1);
        $<opValue>$ = op;*/
        $<opValue>$ = (*kite_compiler_actions[COMPILE_DEREF_1])((kite_compiler_t*)yyget_extra(parm), @1.first_line, $<stringValue>1);
        free($<stringValue>1);
    }
    | var_name '.' SYMBOL_NAME {
        /*COMPILE_INSTRUCTION(kite_compile_push(kite_new_ident(thd, $<stringValue>3)),
             @2.first_line);
        COMPILE_INSTRUCTION(kite_compile_deref_2(), @2.first_line);
        free($<stringValue>3);
        $<opValue>$ = $<opValue>1;*/
        (*kite_compiler_actions[COMPILE_DEREF_2])((kite_compiler_t*)yyget_extra(parm), @3.first_line, $<stringValue>3);
        free($<stringValue>3);
        $<opValue>$ = $<opValue>1;
    }
    ;
    
constant_statement:    STRING_VALUE     {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_CONSTANT_STRING])((kite_compiler_t*)yyget_extra(parm), @1.first_line, $<stringValue>1);
        free($<stringValue>1);
    }
    | INT_VALUE {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_CONSTANT_INTEGER])((kite_compiler_t*)yyget_extra(parm), @1.first_line, $<intValue>1);
    }
    | FLOAT_VALUE {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_CONSTANT_FLOAT])((kite_compiler_t*)yyget_extra(parm), @1.first_line, $<doubleValue>1);
    }
    | BOOL_VALUE {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_CONSTANT_BOOL])((kite_compiler_t*)yyget_extra(parm), @1.first_line, $<intValue>1);
    }
    | NULL_VALUE {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_CONSTANT_NULL])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    }
    | INT_VALUE ':' INT_VALUE
    {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_CONSTANT_RANGE])((kite_compiler_t*)yyget_extra(parm), @2.first_line, $<intValue>1, $<intValue>3, 1);
    }
    | INT_VALUE ':' INT_VALUE ':' INT_VALUE
    {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_CONSTANT_RANGE])((kite_compiler_t*)yyget_extra(parm), @2.first_line, $<intValue>1, $<intValue>3, $<intValue>5);
    }
    | REGEX_VALUE {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_CONSTANT_REGEX])((kite_compiler_t*)yyget_extra(parm), @1.first_line, $<stringValue>1, FALSE);
        free($<stringValue>1);
    }
    | REGEX_VALUE_CI {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_CONSTANT_REGEX])((kite_compiler_t*)yyget_extra(parm), @1.first_line, $<stringValue>1, TRUE);
        free($<stringValue>1);
    }
    | list_value { $<opValue>$ = NULL; }
    | construct_object_statement { $<opValue>$ = NULL; }
    | anon_method_declaration { $<opValue>$ = NULL; }
    | '(' data_manipulation_statement ')' { $<opValue>$ = NULL; }
    ;

construct_object_statement:    MAKE var_name param_list    {
        (*kite_compiler_actions[COMPILE_MAKE])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    }
    ;
    
list_value: '[' ']' {
        (*kite_compiler_actions[COMPILE_ZERO_ELEMENT_LIST])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    }
    | '[' list_elements ']'
    ;
    
list_elements: list_element    {
        (*kite_compiler_actions[COMPILE_ONE_ELEMENT_LIST])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    }
    | list_element ',' list_elements    {
        (*kite_compiler_actions[COMPILE_TWO_ELEMENT_LIST])((kite_compiler_t*)yyget_extra(parm), @2.first_line);
    }
    ;

list_element:    or_statement
    ;
    
decide_statement:    DECIDE {
        (*kite_compiler_actions[COMPILE_DECIDE_BEGIN])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    } '[' decide_contents ']' {
        (*kite_compiler_actions[COMPILE_DECIDE_END])((kite_compiler_t*)yyget_extra(parm), @5.first_line);
    }
    ;
    
decide_contents:    decide_condition
    | decide_contents ',' decide_condition
    ;
    
decide_condition:    constant_statement {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_DECIDE_CONDITION_BEGIN])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    } '[' input ']'    {
        (*kite_compiler_actions[COMPILE_DECIDE_CONDITION_END])((kite_compiler_t*)yyget_extra(parm), @5.first_line, $<opValue>2);
    }
    ;
    
eval_statement:    EVAL {
        (*kite_compiler_actions[COMPILE_EVAL_BEGIN])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    } or_statement    {
        (*kite_compiler_actions[COMPILE_EVAL_END])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    }
    ;

loop_statement:    loop_type '[' input ']'    {
        (*kite_compiler_actions[COMPILE_LOOP_WRAPPER])((kite_compiler_t*)yyget_extra(parm), @4.first_line, $<opValue>1);
    }
    ;

loop_type:    WHILE {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_WHILE_BEGIN])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    } constant_statement    {
        (*kite_compiler_actions[COMPILE_WHILE_END])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
        $<opValue>$ = $<opValue>2;
    }
    | UNTIL {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_UNTIL_BEGIN])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    } constant_statement    {
        (*kite_compiler_actions[COMPILE_UNTIL_END])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
        $<opValue>$ = $<opValue>2;
    }
    ;

run_catch_statement: RUN {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_RUN_BEGIN])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    } '[' input ']' {
        $<opValue>$ = (*kite_compiler_actions[COMPILE_RUN_END])((kite_compiler_t*)yyget_extra(parm), @1.first_line);
    } CATCH {
        (*kite_compiler_actions[COMPILE_CATCH_BEGIN])((kite_compiler_t*)yyget_extra(parm), @7.first_line, $<opValue>2);
    } '[' input ']' {
        (*kite_compiler_actions[COMPILE_CATCH_END])((kite_compiler_t*)yyget_extra(parm), @7.first_line, $<opValue>6);
    }
    ;
    
%%

void dirty_yyerror(void *yyparse_param, char *str) {
    kite_object_t *exc;
    kite_compiler_t *comp = (kite_compiler_t*)yyget_extra(yyparse_param);
    kite_thread_t *thd = comp->thd;
    
    kite_funccall_info_t *entry = calloc(sizeof(kite_funccall_info_t), 1);
    entry->this = NULL;
    entry->file = comp->file;
    entry->line = comp->currentLine;
    kite_push_stack(&thd->func_stack, FALSE, entry);

    exc = kite_new_exception(thd, "System.exceptions.SyntaxError", 
                             str);
    kite_vm_call_method(thd, exc, "throw", kite_new_list(thd), TRUE);    
}

void checkReserved(void *p, char *name) {
    if (!strcmp(name, "__exc") ||
        !strcmp(name, "this")) {
        dirty_yyerror(p, "Cannot use reserved names for parameters.");
    }
}