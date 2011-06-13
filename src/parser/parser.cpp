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

// for extended parse error messages
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <iomanip>
namespace classic = boost::spirit::classic;

using namespace std;
 
namespace kite
{
    namespace parser
    {
        bool kite_parser::parse(std::istream &stream, semantics::syntax_tree &ast)
        {
            using boost::spirit::ascii::space;
            using boost::spirit::multi_pass;

            std::istreambuf_iterator<char> stream_iter(stream);

            typedef multi_pass<std::istreambuf_iterator<char> > forward_iterator_type;
            forward_iterator_type fwd_begin =
                boost::spirit::make_default_multi_pass(stream_iter);
            forward_iterator_type fwd_end;

            // wrap forward iterator with position iterator, to record the position
            typedef classic::position_iterator2<forward_iterator_type> pos_iterator_type;
            pos_iterator_type position_begin(fwd_begin, fwd_end, "(stdin)"); // TODO
            pos_iterator_type position_end;

#define KITE_SKIP_RULE space | ('#' >> *(ascii::char_ - qi::eol) >> qi::eol)
            kite_grammar<pos_iterator_type, BOOST_TYPEOF(KITE_SKIP_RULE)> grammar;
            try
            {
                bool r = phrase_parse(position_begin, position_end, grammar, KITE_SKIP_RULE, ast);
                return r && position_begin == position_end;
            }
            catch (const qi::expectation_failure<pos_iterator_type> &e)
            {
                const classic::file_position_base<std::string>& pos =
    e.first.get_position();
                cerr << "parse error at file " << pos.file
                     << "line " << pos.line << " column " << pos.column << std::endl
                     << "'" << e.first.get_currentline() << "'" << std::endl
                     << std::setw(pos.column) << " " << "^- here";
                return false;
            }
        }
    }
}
