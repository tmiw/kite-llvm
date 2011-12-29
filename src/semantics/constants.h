/*****************************************************************************
 * Copyright (c) 2011, Mooneer Salem
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
 
#ifndef KITE_SEMANTICS__CONSTANTS_H
#define KITE_SEMANTICS__CONSTANTS_H

#include <boost/assign.hpp>
using namespace boost::assign;

namespace kite
{
    namespace semantics
    {
        enum builtin_types
        {
            INTEGER,
            FLOAT,
            BOOLEAN,
            STRING,
            OBJECT,
            METHOD_TY,
        };
        
        enum code_operation
        {
            ADD,
            SUBTRACT,
            MULTIPLY,
            DIVIDE,
            MODULO,
            LEFT_SHIFT,
            RIGHT_SHIFT,
            LESS_THAN,
            LESS_OR_EQUALS,
            GREATER_THAN,
            GREATER_OR_EQUALS,
            EQUALS,
            NOT_EQUALS,
            NOT,
            OR,
            XOR,
            AND,
            MAP,
            REDUCE,
            __END_BINARY_OPS, // DO NOT USE
            UNARY_PLUS,
            UNARY_MINUS,
            DEREF_ARRAY,
            __END_OVERRIDABLE_OPS, // DO NOT USE
            ITERATE,
            CONST,
            DEREF_FILTER,
            DEREF_PROPERTY,
            DEREF_METHOD,
            DEREF_METHOD_RELATIVE_SELF, // xyz(...) without '|'.
            VARIABLE,
            ASSIGN,
            WHILE,
            UNTIL,
            DECIDE,
            METHOD,
            CLASS,
            MAKE,
            RUN_CATCH,
            CONSTRUCTOR,
            DESTRUCTOR,
            IS_CLASS,
            ISOF_CLASS,
            OPERATOR,
            LIST_VAL,
            METHOD_REF,
            IMPORT,
            BREAK,
            CONTINUE,
        };
        
        typedef std::map<semantics::code_operation, std::string> OperatorMethodsMap;
        typedef std::map<std::string, std::string> OperatorMethodNameMap;
        
        class Constants
        {
        public:
            OperatorMethodsMap operator_map;
            OperatorMethodNameMap operator_name_map;
            
            static Constants &Get()
            {
                static Constants *constants_ptr = new Constants();
                return *constants_ptr;
            }
            
            Constants()
            {
                operator_map = map_list_of
                    (semantics::ADD, "__op_plus__")
                    (semantics::SUBTRACT, "__op_minus__")
                    (semantics::MULTIPLY, "__op_multiply__")
                    (semantics::DIVIDE, "__op_divide__")
                    (semantics::MODULO, "__op_mod__")
                    (semantics::LEFT_SHIFT, "__op_lshift__")
                    (semantics::RIGHT_SHIFT, "__op_rshift__")
                    (semantics::LESS_THAN, "__op_lt__")
                    (semantics::GREATER_THAN, "__op_gt__")
                    (semantics::LESS_OR_EQUALS, "__op_leq__")
                    (semantics::GREATER_OR_EQUALS, "__op_geq__")
                    (semantics::EQUALS, "__op_equals__")
                    (semantics::NOT_EQUALS, "__op_nequals__")
                    (semantics::AND, "__op_and__")
                    (semantics::OR, "__op_or__")
                    (semantics::NOT, "__op_not__")
                    (semantics::UNARY_PLUS, "__op_unplus__")
                    (semantics::UNARY_MINUS, "__op_unminus__")
                    (semantics::XOR, "__op_xor__")
                    (semantics::CONSTRUCTOR, "__init__")
                    (semantics::DESTRUCTOR, "__destruct__")
                    (semantics::DEREF_ARRAY, "__op_deref_array__")
                    (semantics::MAP, "__op_map__")
                    (semantics::REDUCE, "__op_reduce__");

                operator_name_map = map_list_of
                    ("plus", "__op_plus__")
                    ("minus", "__op_minus__")
                    ("multiply", "__op_multiply__")
                    ("divide", "__op_divide__")
                    ("mod", "__op_mod__")
                    ("lshift", "__op_lshift__")
                    ("rshift", "__op_rshift__")
                    ("lt", "__op_lt__")
                    ("gt", "__op_gt__")
                    ("leq", "__op_leq__")
                    ("geq", "__op_geq__")
                    ("equals", "__op_equals__")
                    ("nequals", "__op_nequals__")
                    ("and", "__op_and__")
                    ("or", "__op_or__")
                    ("not", "__op_not__")
                    ("unplus", "__op_unplus__")
                    ("unminus", "__op_unminus__")
                    ("xor", "__op_xor__")
                    ("array", "__op_deref_array__")
                    ("map", "__op_map__")
                    ("reduce", "__op_reduce__");
            }
        };
    }
}

#endif
