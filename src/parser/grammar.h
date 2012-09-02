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
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

using boost::spirit::ascii::space;

// for extended parse error messages
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <iomanip>
namespace classic = boost::spirit::classic;
using boost::spirit::multi_pass;
typedef multi_pass<std::istreambuf_iterator<char> > forward_iterator_type;
typedef classic::position_iterator2<forward_iterator_type> pos_iterator_type;

#define KITE_SKIP_RULE space | ('#' >> *(ascii::char_ - qi::eol) >> qi::eol)

// Necessary for line number collection.
// =====================================

namespace kite 
{ 
    namespace parser
    {
        // iter_pos contains the location in the file.
        BOOST_SPIRIT_TERMINAL(iter_pos);
    }
}

namespace boost { namespace spirit
{
    // We want custom_parser::iter_pos to be usable as a terminal only,
    // and only for parser expressions (qi::domain).
    template <>
    struct use_terminal<qi::domain, kite::parser::tag::iter_pos>
      : mpl::true_
    {};
}}

namespace kite
{
    namespace parser
    {
        struct iter_pos_parser
          : boost::spirit::qi::primitive_parser<iter_pos_parser>
        {
            // Define the attribute type exposed by this parser component
            template <typename Context, typename Iterator>
            struct attribute
            {
                typedef Iterator type;
            };
 
            // This function is called during the actual parsing process
            template <typename Iterator, typename Context
              , typename Skipper, typename Attribute>
            bool parse(Iterator& first, Iterator const& last
              , Context&, Skipper const& skipper, Attribute& attr) const
            {
                boost::spirit::qi::skip_over(first, last, skipper);
                boost::spirit::traits::assign_to(first, attr);
                return true;
            }
 
            // This function is called during error handling to create
            // a human readable string for the error context.
            template <typename Context>
            boost::spirit::info what(Context&) const
            {
                return boost::spirit::info("iter_pos");
            }
        };
    }
}

namespace boost { namespace spirit { namespace qi
{
    // This is the factory function object invoked in order to create
    // an instance of our iter_pos_parser.
    template <typename Modifiers>
    struct make_primitive<kite::parser::tag::iter_pos, Modifiers>
    {
        typedef kite::parser::iter_pos_parser result_type;
 
        result_type operator()(unused_type, unused_type) const
        {
            return result_type();
        }
    };
}}}

namespace kite
{
    namespace parser
    {        
        namespace fusion = boost::fusion;
        namespace phoenix = boost::phoenix;
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;
        namespace semantics = kite::semantics;
        
        template<typename Iterator, typename SkipType>
        struct kite_grammar
            : qi::grammar<Iterator, semantics::syntax_tree(), SkipType>
        {
            kite_grammar() : kite_grammar::base_type(start)
            {
                // Constants.
                initialize_const_rules();
                initialize_make_rules();
                
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
                initialize_exception_rules();
                initialize_constructor_rules();
                initialize_destructor_rules();
                initialize_statement_rules();
            }
            
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> start;
            qi::rule<Iterator, semantics::syntax_tree_node(), SkipType> statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> math_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> method_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> operator_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> anon_method_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> class_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> loop_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> decide_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> const_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> bit_shift_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> multiply_divide_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> add_subtract_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> comparison_equals_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> map_reduce_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> assign_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> or_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> xor_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> and_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> comparison_less_greater_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> unary_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> grouping_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> deref_filter_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> deref_filter_only_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> deref_types;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> deref_property_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> deref_method_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> deref_method_mandatory_params;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> deref_array_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> make_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> exception_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> constructor_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> destructor_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> list_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> method_ref_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> import_statement;
            qi::rule<Iterator, semantics::syntax_tree(), SkipType> break_continue_statement;
            qi::rule<Iterator, std::string(), SkipType> identifier;
            qi::rule<Iterator, std::string(), SkipType> operator_identifier;
            qi::rule<Iterator, semantics::syntax_tree_node(), SkipType> numeric_value;
            qi::rule<Iterator, std::string()> unesc_str;
            qi::symbols<char const, char const> unesc_char;
        
        private:
            void initialize_const_rules();
            void initialize_make_rules();
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
            void initialize_exception_rules();
            void initialize_constructor_rules();
            void initialize_destructor_rules();
            void initialize_statement_rules();
        };
    }
}

BOOST_FUSION_ADAPT_STRUCT(
    kite::semantics::syntax_tree,
    (kite::semantics::code_operation, op)
    (std::deque<kite::semantics::syntax_tree_node>, children)
    (kite::semantics::syntax_tree_position, position)
)

#endif
