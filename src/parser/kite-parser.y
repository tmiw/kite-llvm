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
%require "2.6"
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

result_grabber: pre_input { driver.result = $<opValue>1; }
    ;

pre_input: 
      DOCSTRING { /* TODO */ } input { $<opValue>$ = $<opValue>3; }
    | VERSION_KEYWORD STRING_VALUE ';' { /* TODO */ } input { $<opValue>$ = $<opValue>5; }
    | input { $<opValue>$ = $<opValue>1; }
    ;

input:    /* empty */ {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ITERATE;
    }
    | statement ';' input {
        $<opValue>$ = $<opValue>3;
        if ($<opValue>1 != NULL)
        {
            $<opValue>$->children.push_back(*$<opValue>1);
        }
    }
    ;

statement:
    | data_manipulation_statement
    | options_set_statement
    | loop_statement
    /*| from_statement*/ /* TODO: inheritance without explicit class define. */
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
        $<opValue>$->children.push_back(*$<opValue>2);
        
        std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>3;
        for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
             i != param_names.end();
             i++)
        {
            $<opValue>$->children.push_back(*i);
        }
    } '[' input ']' {
        $<opValue>$->children.push_back(*$<opValue>7);
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
        $<opValue>$->children.push_back(*$<opValue>5);
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
        $<opValue>$ = $<opValue>1;
    }
    ;
    
param_name_list:    '(' ')'    {
        $<listValue>$ = new std::vector<kite::semantics::syntax_tree_node>();
    }
    | '(' param_elements ')'    {
        $<opValue>$ = $<opValue>2;
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
        $<opValue>$ = $<opValue>4;
    }
    ;
    
class_definition:    CLASS SYMBOL_NAME {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::CLASS;
        $<opValue>$->children.push_back(*$<stringValue>2);
    } optional_from optional_docstring '[' class_body ']'    {
        /* TODO: docstring */
        if ($<opValue>4 != NULL)
        {
            $<opValue>$->children.push_back(*$<opValue>4);
        }
        $<opValue>$->children.push_back(*$<opValue>7);
    }
    ;
    
optional_from:        {
        $<opValue>$ = NULL;
    }
    | from_statement { $<opValue>$ = $<opValue>1; }
    ;

from_statement:
    FROM variable_immediate   {
        $<opValue>$ = $<opValue>2;
    }
    ;

variable_immediate:     
    variable_immediate '.' SYMBOL_NAME {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DEREF_FILTER;
        $<opValue>$->children.push_back(*$<opValue>1);
        
        kite::semantics::syntax_tree deref_method;
        deref_method.op = kite::semantics::DEREF_PROPERTY
        deref_method.children.push_back(*$<stringValue>3);
        $<opValue>$->children.push_back(deref_method);
    }
    | SYMBOL_NAME {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DEREF_FILTER;
        $<opValue>$->children.push_back(*$<stringValue>1);
    }

class_body:    
    {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ITERATE; 
    }
    | class_elements { $<opValue>$ = $<opValue>1; }
    ;

class_elements:    class_element
    {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ITERATE;
        $<opValue>$->children.push_back(*$<opValue>1);
    }
    | class_elements ',' class_element
    {
        $<opValue>$ = $<opValue>1;
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    ;
    
class_element:    constructor_definition { $<opValue>$ = $<opValue>1; }
    | destructor_definition { $<opValue>$ = $<opValue>1; }
    | property_definition { $<opValue>$ = $<opValue>1; }
    | operator_definition { $<opValue>$ = $<opValue>1; }
    | class_definition { $<opValue>$ = $<opValue>1; }
    | data_manipulation_statement { $<opValue>$ = $<opValue>1; }
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
        $<opValue>$->children.push_back(*$<opValue>6);
    }
    ;
    
destructor_definition: DESTRUCT {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DESTRUCTOR;
    } optional_docstring '[' input ']' {
        /* TODO: docstring */
        $<opValue>$->children.push_back(*$<opValue>5);
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
    $<opValue>$->children.push_back(*$<stringVal>2);
    
    std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>3;
    for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
         i != param_names.end();
         i++)
    {
        $<opValue>$->children.push_back(*i);
    }
} optional_docstring '[' input ']' {
    /* TODO: docstring */
    $<opValue>$->children.push_back(*$<opValue>7);
}
    ;
    
operator_type:    
      PLUS              { $<stringVal>$ = $<stringVal>1; }
    | MINUS             { $<stringVal>$ = $<stringVal>1; }
    | MULTIPLY          { $<stringVal>$ = $<stringVal>1; }
    | DIVIDE            { $<stringVal>$ = $<stringVal>1; }
    | MOD               { $<stringVal>$ = $<stringVal>1; }
    | UNARY_PLUS        { $<stringVal>$ = $<stringVal>1; }
    | UNARY_MINUS       { $<stringVal>$ = $<stringVal>1; }
    | MAP               { $<stringVal>$ = $<stringVal>1; }
    | REDUCE            { $<stringVal>$ = $<stringVal>1; }
    | ARRAY_DEREF       { $<stringVal>$ = $<stringVal>1; }
    | ARRAY_SET         { $<stringVal>$ = $<stringVal>1; }
    | EQUALS            { $<stringVal>$ = $<stringVal>1; }
    | NOT_EQUALS        { $<stringVal>$ = $<stringVal>1; }
    | LESS_THAN         { $<stringVal>$ = $<stringVal>1; }
    | GREATER_THAN      { $<stringVal>$ = $<stringVal>1; }
    | LESS_OR_EQUALS    { $<stringVal>$ = $<stringVal>1; }
    | GREATER_OR_EQUALS { $<stringVal>$ = $<stringVal>1; }
    | AND               { $<stringVal>$ = $<stringVal>1; }
    | OR                { $<stringVal>$ = $<stringVal>1; }
    | NOT               { $<stringVal>$ = $<stringVal>1; }
    | XOR               { $<stringVal>$ = $<stringVal>1; }
    | LEFT_SHIFT        { $<stringVal>$ = $<stringVal>1; }
    | RIGHT_SHIFT       { $<stringVal>$ = $<stringVal>1; }
    | METHOD_CALL       { $<stringVal>$ = $<stringVal>1; }
    | PROPERTY          { $<stringVal>$ = $<stringVal>1; }
    ;

data_manipulation_statement:   
      assignment_statement { $<opValue>$ = $<opValue>1; }
    | method_declaration   { $<opValue>$ = $<opValue>1; }
    | eval_statement       { $<opValue>$ = $<opValue>1; }
    ;
    
assignment_statement:    deref_statement {
    $<opValue>$ = new kite::semantics::syntax_tree();
    $<opValue>$->op = kite::semantics::ASSIGN;
    $<opValue>$->children.push_back(*$<opValue>1);
} '=' data_manipulation_statement {
        $<opValue>$->children.push_back(*$<opValue>4);
    }
    | or_statement { $<opValue>$ = $<opValue>1; }
    ;
    
or_statement:        xor_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::OR;
        $<opValue>$->children.push_back(*$<opValue>1);
    } OR or_statement {
        $<opValue>$->children.push_back(*$<opValue>4);
    }
    | xor_statement { $<opValue>$ = $<opValue>1; }
    ;

xor_statement:    xor_statement XOR and_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::XOR;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | and_statement { $<opValue>$ = $<opValue>1; }
    ;
    
and_statement:    bitwise_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::AND;
        $<opValue>$->children.push_back(*$<opValue>1);
    } AND and_statement {
        $<opValue>$->children.push_back(*$<opValue>4);
    }
    | bitwise_statement { $<opValue>$ = $<opValue>1; }
    ;

bitwise_statement:    bitwise_statement LEFT_SHIFT equality_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::LEFT_SHIFT;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | bitwise_statement RIGHT_SHIFT equality_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::RIGHT_SHIFT;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | equality_statement { $<opValue>$ = $<opValue>1; }
    ;
    
equality_statement:    equality_statement EQUALS less_greater_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::EQUALS;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | equality_statement NOT_EQUALS less_greater_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::NOT_EQUALS;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | equality_statement IS less_greater_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::IS_CLASS;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | equality_statement ISOF less_greater_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ISOF_CLASS;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | less_greater_statement { $<opValue>$ = $<opValue>1; }
    ;
    
less_greater_statement:    less_greater_statement '<' add_sub_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::LESS_THAN;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | less_greater_statement '>' add_sub_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::GREATER_THAN;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | less_greater_statement LESS_OR_EQUALS add_sub_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::LESS_OR_EQUALS;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | less_greater_statement GREATER_OR_EQUALS add_sub_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::GREATER_OR_EQUALS;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | add_sub_statement { $<opValue>$ = $<opValue>1; }
    ;
    
add_sub_statement:    add_sub_statement '+' multdiv_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::ADD;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | add_sub_statement '-' multdiv_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::SUBTRACT;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | multdiv_statement { $<opValue>$ = $<opValue>1; }
    ;
    
multdiv_statement:    multdiv_statement '*' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::MULTIPLY;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | multdiv_statement '/' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DIVIDE;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | multdiv_statement '%' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::MODULO;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | unary_statement { $<opValue>$ = $<opValue>1; }
    ;
    
unary_statement: '+' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::UNARY_PLUS;
        $<opValue>$->children.push_back(*$<opValue>2);
    }
    | '-' unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::UNARY_MINUS;
        $<opValue>$->children.push_back(*$<opValue>2);
    }
    | NOT unary_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::NOT;
        $<opValue>$->children.push_back(*$<opValue>2);
    }
    | map_reduce_statement { $<opValue>$ = $<opValue>1; }
    ;
    
map_reduce_statement:    deref_statement MAP deref_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::MAP;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | deref_statement REDUCE deref_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::REDUCE;
        $<opValue>$->children.push_back(*$<opValue>1);
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | SYMBOL_NAME param_list { 
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DEREF_FILTER;
        $<opValue>$->children.push_back(std::string("this"));
        
        kite::semantics::syntax_tree deref_method;
        deref_method.op = kite::semantics::DEREF_METHOD;
        deref_method.children.push_back(*$<stringValue>1);
        std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>2;
        for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
             i != param_names.end();
             i++)
        {
            deref_method.children.push_back(*i);
        }
        $<opValue>$->children.push_back(deref_method);
    }
    | deref_statement { $<opValue>$ = $<opValue>1; }
    ;
    
deref_statement:    deref_statement '|' SYMBOL_NAME optional_param_list {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DEREF_FILTER;
        $<opValue>$->children.push_back(*$<opValue>1);
        
        kite::semantics::syntax_tree deref_method;
        deref_method.op = kite::semantics::DEREF_METHOD;
        deref_method.children.push_back(*$<stringValue>3);
        std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>4;
        for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
             i != param_names.end();
             i++)
        {
            deref_method.children.push_back(*i);
        }
        $<opValue>$->children.push_back(deref_method);
    }
    | deref_statement '[' data_manipulation_statement ']'    {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DEREF_FILTER;
        $<opValue>$->children.push_back(*$<opValue>1);
        
        kite::semantics::syntax_tree deref_method;
        deref_method.op = kite::semantics::DEREF_ARRAY
        deref_method.children.push_back(*$<opValue>3);
        $<opValue>$->children.push_back(deref_method);
    }
    | deref_statement '.' SYMBOL_NAME {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DEREF_FILTER;
        $<opValue>$->children.push_back(*$<opValue>1);
        
        kite::semantics::syntax_tree deref_method;
        deref_method.op = kite::semantics::DEREF_PROPERTY
        deref_method.children.push_back(*$<stringValue>3);
        $<opValue>$->children.push_back(deref_method);
    }
    | SYMBOL_NAME {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DEREF_FILTER;
        $<opValue>$->children.push_back(*$<stringValue>1);
    }
    | constant_statement {
        $<opValue>$ = $<opValue>1;
    }
    ;

optional_param_list: {
        $<opValue>$ = new std::vector<kite::semantics::syntax_tree_node>;
    }
    | param_list {
        $<opValue>$ = $<opValue>1;
    }
    ;

param_list:    '(' ')' {
        $<listValue>$ = new std::vector<kite::semantics::syntax_tree_node>;
    }
    | '(' params_list ')' {
        $<listValue>$ = $<listValue>2;
    }
    ;

params_list: data_manipulation_statement    {
        $<listValue>$ = new std::vector<kite::semantics::syntax_tree_node>;
        $<listValue>$->children.push_back(*$<opValue>1);
    }
    | data_manipulation_statement ',' params_list    {
        $<listValue>$ = $<listValue>3;
        $<listValue>$->children.push_front(*$<opValue>1);
    }
    ;
    
var_name:    SYMBOL_NAME {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DEREF_FILTER;
        $<opValue>$->children.push_back(*$<stringValue>1);
    }
    | var_name '.' SYMBOL_NAME {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DEREF_FILTER;
        $<opValue>$->children.push_back(*$<opValue>1);
        
        kite::semantics::syntax_tree deref_method;
        deref_method.op = kite::semantics::DEREF_PROPERTY
        deref_method.children.push_back(*$<opValue>3);
        $<opValue>$->children.push_back(deref_method);
    }
    ;
    
constant_statement:    
      STRING_VALUE { $<stringValue>$ = $<stringValue>1; }
    | INT_VALUE { $<intValue>$ = $<intValue>1; }
    | FLOAT_VALUE { $<doubleValue>$ = $<doubleValue>1; }
    | BOOL_VALUE { $<intValue>$ = $<intValue>1; }
    | NULL_VALUE { $<opValue>$ = NULL; }
    | INT_VALUE ':' INT_VALUE
    {
        /* TODO: range */
    }
    | INT_VALUE ':' INT_VALUE ':' INT_VALUE
    {
        /* TODO: range */
    }
    | REGEX_VALUE {
        /* TODO: regex */
    }
    | REGEX_VALUE_CI {
        /* TODO: regex */
    }
    | list_value { $<opValue>$ = $<opValue>1; }
    | construct_object_statement { $<opValue>$ = $<opValue>1; }
    | anon_method_declaration { $<opValue>$ = $<opValue>1; }
    | '(' data_manipulation_statement ')' { $<opValue>$ = $<opValue>1; }
    ;

construct_object_statement:    MAKE var_name param_list    {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::MAKE;
        $<opValue>$->children.push_back(*$<opValue>2);
        std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>3;
        for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
             i != param_names.end();
             i++)
        {
            $<opValue>$->children.push_back(*i);
        }
    }
    ;
    
list_value: '[' ']' {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::LIST_VAL;
    }
    | '[' list_elements ']' { $<opValue>$ = $<opValue>2; }
    ;
    
list_elements: list_element    {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::LIST_VAL;
        $<opValue>$->children.push_back(*$<opValue>1);
    }
    | list_element ',' list_elements    {
        $<opValue>$ = $<opValue>3;
        $<opValue>$->children.push_front(*$<opValue>1);
    }
    ;

list_element:    or_statement { $<opValue>$ = $<opValue>1; }
    ;
    
decide_statement:    DECIDE {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::DECIDE;
    } '[' decide_contents ']' {
        std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>4;
        for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
             i != param_names.end();
             i++)
        {
            $<opValue>$->children.push_back(*i);
        }
    }
    ;
    
decide_contents:    
      decide_condition {
          $<listValue>$ = new std::vector<kite::semantics::syntax_tree_node>;
          
          std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>1;
          for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
               i != param_names.end();
               i++)
          {
              $<listValue>$->push_back(*i);
          }
      }
    | decide_contents ',' decide_condition {
        $<listValue>$ = $<listValue>1;
        std::vector<kite::semantics::syntax_tree_node> &param_names = *$<listValue>3;
          for (std::vector<kite::semantics::syntax_tree_node>::iterator i = param_names.begin();
               i != param_names.end();
               i++)
          {
              $<listValue>$->push_back(*i);
          }
    }
    ;
    
decide_condition:    constant_statement {
        $<listValue>$ = new std::vector<kite::semantics::syntax_tree_node>;
        $<listValue>$->push_back(*$<opValue>1);
    } '[' input ']'    {
        $<listValue>$->push_back(*$<opValue>4);
    }
    ;
    
eval_statement:    EVAL or_statement {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::EVAL;
        $<opValue>$->children.push_back(*$<opValue>2);
    }
    ;

loop_statement:    loop_type '[' input ']'    {
        $<opValue>$ = $<opValue>1;
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    ;

loop_type:    WHILE {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::WHILE;
    } constant_statement    {
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    | UNTIL {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::UNTIL;
    } constant_statement    {
        $<opValue>$->children.push_back(*$<opValue>3);
    }
    ;

run_catch_statement: RUN {
        $<opValue>$ = new kite::semantics::syntax_tree();
        $<opValue>$->op = kite::semantics::RUN_CATCH;
    } '[' input ']' {
        $<opValue>$->children.push_back(*$<opValue>4);
    } CATCH '[' input ']' {
        $<opValue>$->children.push_back(*$<opValue>9);
    }
    ;
    
%%

void
yy::kite_parser::error (const yy::kite_parser::location_type& l,
                        const std::string& m)
{
    driver.error (l, m);
}