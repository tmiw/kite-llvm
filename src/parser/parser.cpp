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

#include "grammar.h"
#include "parser.h"

// HACK: random files are somehow being ignored by g++/ld, so there goes the
// use of multiple .cpp files to try to reduce compile time due to Boost.
#include "assignment.cpp"
#include "bitwise.cpp"
#include "classes.cpp"
#include "comparison.cpp"
#include "constants.cpp"
#include "decide.cpp"
#include "deref.cpp"
#include "grouping.cpp"
#include "loop.cpp"
#include "map_reduce.cpp"
#include "math.cpp"
#include "method.cpp"
#include "statement.cpp"
#include "make.cpp"
#include "exceptions.cpp"

using namespace std;
 
namespace kite
{
    namespace parser
    {
        bool kite_parser::parse(const std::string &code, semantics::syntax_tree &ast)
        {
            kite_grammar<std::string::const_iterator> grammar;

            using boost::spirit::ascii::space;
            string::const_iterator iter = code.begin();
            string::const_iterator end = code.end();
            return phrase_parse(iter, end, grammar, space, ast) && iter == end;
        }
    }
}