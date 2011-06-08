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

#ifndef KITE_PARSER__GRAMMAR_H
#define KITE_PARSER__GRAMMAR_H

#include <semantics/syntax_tree.h>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace kite
{
    namespace parser
    {
        namespace fusion = boost::fusion;
        namespace phoenix = boost::phoenix;
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;
        namespace semantics = kite::semantics;
        
        template<typename Iterator>
        struct kite_grammar
            : qi::grammar<Iterator, semantics::syntax_tree(), ascii::space_type>
        {
            kite_grammar() : kite_grammar::base_type(start)
            {
                // Constants.
                initialize_const_rules();
                
                // Math operations.
                initialize_assign_rules();
                initialize_bitwise_rules();
                initialize_comparison_rules();
                initialize_math_rules();    
                initialize_map_reduce_rules();
                initialize_deref_rules();
                initialize_grouping_rules();

                // Statements.
                initialize_loop_rules();
                initialize_decide_rules();
                initialize_method_rules();
                initialize_class_rules();
                initialize_statement_rules();
            }
            
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> start;
            qi::rule<Iterator, semantics::syntax_tree_node(), ascii::space_type> statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> math_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> method_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> class_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> loop_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> decide_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> const_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> bit_shift_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> multiply_divide_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> add_subtract_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> comparison_equals_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> map_reduce_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> assign_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> or_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> xor_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> and_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> comparison_less_greater_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> unary_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> grouping_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> deref_filter_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> deref_filter_only_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> deref_types;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> deref_property_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> deref_method_statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> deref_method_mandatory_params;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> deref_array_statement;
            qi::rule<Iterator, std::string(), ascii::space_type> identifier;
            qi::rule<Iterator, semantics::syntax_tree_node(), ascii::space_type> numeric_value;
            qi::rule<Iterator, std::string()> unesc_str;
            qi::symbols<char const, char const> unesc_char;
        
        private:
            void initialize_const_rules();
            void initialize_assign_rules();
            void initialize_bitwise_rules();
            void initialize_comparison_rules();
            void initialize_math_rules();
            void initialize_map_reduce_rules();
            void initialize_deref_rules();
            void initialize_grouping_rules();
            void initialize_loop_rules();
            void initialize_decide_rules();
            void initialize_method_rules();
            void initialize_class_rules();
            void initialize_statement_rules();
        };
    }
}

BOOST_FUSION_ADAPT_STRUCT(
    kite::semantics::syntax_tree,
    (kite::semantics::code_operation, op)
    (std::deque<kite::semantics::syntax_tree_node>, children)
)

#endif