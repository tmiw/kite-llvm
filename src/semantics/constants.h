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
            ITERATE,
            CONST,
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
            UNARY_PLUS,
            UNARY_MINUS,
            NOT,
            OR,
            XOR,
            AND,
            MAP,
            REDUCE,
            DEREF_FILTER,
            DEREF_PROPERTY,
            DEREF_METHOD,
            DEREF_ARRAY,
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
        };
        
        typedef std::map<semantics::code_operation, std::string> OperatorMethodsMap;
        typedef std::map<std::string, std::string> OperatorMethodNameMap;
        
        extern OperatorMethodsMap operator_map;
        extern OperatorMethodNameMap operator_name_map;
    }
}

#endif
