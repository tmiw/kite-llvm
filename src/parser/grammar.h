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
                using qi::lit;
                using qi::lexeme;
                using qi::raw;
                using ascii::char_;
                using qi::int_;
                using qi::double_;
                using ascii::string;
                using namespace qi::labels;

                using phoenix::at_c;
                using phoenix::push_back;
                
                // Constants.
                numeric_value = 
                    (int_ | double_) [ _val = _1 ]
                    | lit("true") [ _val = true ]
                    | lit("false") [ _val = false ];
                
                const_statement = 
                      numeric_value [ push_back(at_c<1>(_val), _1) ] [ at_c<0>(_val) = kite::semantics::CONST ]
                    | identifier [ push_back(at_c<1>(_val), _1) ] [ at_c<0>(_val) = kite::semantics::VARIABLE ];
                      
                identifier =
                    (qi::lexeme[ (qi::alpha | '_') >> *(qi::alnum | '_') ]); // [ _val = _1 ];
                
                // Math operations.
                assign_statement =
                    (    deref_filter_only_statement [ push_back(at_c<1>(_val), _1) ]
                      >> lit('=') [ at_c<0>(_val) = kite::semantics::ASSIGN ]
                      >> or_statement [ push_back(at_c<1>(_val), _1) ] )
                    | or_statement [ _val = _1 ];
                    
                or_statement = 
                    (   xor_statement [ push_back(at_c<1>(_val), _1) ]
                     >> lit("or") [ at_c<0>(_val) = kite::semantics::OR ]
                     >> or_statement [ push_back(at_c<1>(_val), _1) ])
                    | xor_statement [ _val = _1 ];
                
                xor_statement = 
                    (   and_statement [ push_back(at_c<1>(_val), _1) ]
                     >> lit("xor") [ at_c<0>(_val) = kite::semantics::OR ]
                     >> xor_statement [ push_back(at_c<1>(_val), _1) ])
                    | and_statement [ _val = _1 ];
                
                and_statement = 
                    (   comparison_equals_statement [ push_back(at_c<1>(_val), _1) ]
                     >> lit("and") [ at_c<0>(_val) = kite::semantics::OR ]
                     >> and_statement [ push_back(at_c<1>(_val), _1) ])
                    | comparison_equals_statement [ _val = _1 ];
                     
                comparison_equals_statement = 
                    (  comparison_less_greater_statement [ push_back(at_c<1>(_val), _1) ]
                    >> ((lit("==") [ at_c<0>(_val) = kite::semantics::EQUALS ]) |
                        (lit("!=") [ at_c<0>(_val) = kite::semantics::NOT_EQUALS ]))
                    >> comparison_equals_statement [ push_back(at_c<1>(_val), _1) ])
                    | comparison_less_greater_statement [ _val = _1 ];
                    
                comparison_less_greater_statement =
                    (  bit_shift_statement [ push_back(at_c<1>(_val), _1) ]
                    >> ((lit("<") [ at_c<0>(_val) = kite::semantics::LESS_THAN ]) |
                        (lit("<=") [ at_c<0>(_val) = kite::semantics::LESS_OR_EQUALS ]) |
                        (lit(">") [ at_c<0>(_val) = kite::semantics::GREATER_THAN ]) |
                        (lit(">=") [ at_c<0>(_val) = kite::semantics::GREATER_OR_EQUALS ]))
                    >> comparison_less_greater_statement [ push_back(at_c<1>(_val), _1) ])
                    | bit_shift_statement [ _val = _1 ];
                    
                bit_shift_statement = 
                    (  add_subtract_statement [ push_back(at_c<1>(_val), _1) ]
                    >> ((lit(">>") [ at_c<0>(_val) = kite::semantics::RIGHT_SHIFT ]) |
                        (lit("<<") [ at_c<0>(_val) = kite::semantics::LEFT_SHIFT ]))
                    >> bit_shift_statement [ push_back(at_c<1>(_val), _1) ])
                    | add_subtract_statement [ _val = _1 ];
                    
                add_subtract_statement = 
                    (  multiply_divide_statement [ push_back(at_c<1>(_val), _1) ]
                    >> ((lit('+') [ at_c<0>(_val) = kite::semantics::ADD ]) |
                        (lit('-') [ at_c<0>(_val) = kite::semantics::SUBTRACT ]))
                    >> add_subtract_statement [ push_back(at_c<1>(_val), _1) ])
                    | multiply_divide_statement [ _val = _1 ];
    
                multiply_divide_statement = 
                    (    unary_statement [ push_back(at_c<1>(_val), _1) ]
                      >> ((lit('*') [ at_c<0>(_val) = kite::semantics::MULTIPLY ]) |
                          (lit('/') [ at_c<0>(_val) = kite::semantics::DIVIDE ]) |
                          (lit('%') [ at_c<0>(_val) = kite::semantics::MODULO ]))
                       >> multiply_divide_statement [ push_back(at_c<1>(_val), _1) ])
                     | unary_statement [ _val = _1 ];
                
                unary_statement = 
                    ((lit('+') [ at_c<0>(_val) = kite::semantics::UNARY_PLUS ]) |
                     (lit('-') [ at_c<0>(_val) = kite::semantics::UNARY_MINUS ]) |
                     (lit("not") [ at_c<0>(_val) = kite::semantics::NOT ]))
                     >> unary_statement [ push_back(at_c<1>(_val), _1) ]
                    | map_reduce_statement [ _val = _1 ];
                
                map_reduce_statement =
                    (    deref_filter_statement [ push_back(at_c<1>(_val), _1) ]
                      >> ((lit("<|") [ at_c<0>(_val) = kite::semantics::REDUCE ]) |
                          (lit("<-") [ at_c<0>(_val) = kite::semantics::MAP ]))
                      >> deref_filter_statement [ push_back(at_c<1>(_val), _1) ])
                    | deref_filter_statement [ _val = _1 ];
                
                deref_property_statement =
                    lit('.') [ at_c<0>(_val) = kite::semantics::DEREF_PROPERTY ] >>
                    identifier [ push_back(at_c<1>(_val), _1) ];
                
                deref_method_statement =
                    lit('|') [ at_c<0>(_val) = kite::semantics::DEREF_METHOD ] >>
                    identifier [ push_back(at_c<1>(_val), _1) ] >>
                    (!lit('(') | (lit('(') >> *(math_statement [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')')));
                
                deref_method_mandatory_params =
                    (identifier
                        [ push_back(at_c<1>(_val), _1) ] 
                        [ at_c<0>(_val) = kite::semantics::DEREF_METHOD_RELATIVE_SELF ])
                    >> (lit('(') >> *(math_statement [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')'));
                    
                deref_array_statement =
                    lit('[') [ at_c<0>(_val) = kite::semantics::DEREF_ARRAY ] >>
                    math_statement [ push_back(at_c<1>(_val), _1) ] >>
                    lit(']');
                    
                deref_types = 
                    (deref_property_statement | deref_method_statement | deref_array_statement ) [ _val = _1 ];
                
                deref_filter_only_statement =
                    (    grouping_statement 
                            [ push_back(at_c<1>(_val), _1) ]
                            [ at_c<0>(_val) = kite::semantics::DEREF_FILTER ])
                    >> *deref_types [ push_back(at_c<1>(_val), _1) ];
                            
                deref_filter_statement =
                      deref_filter_only_statement [ _val = _1 ]
                    | (deref_method_mandatory_params
                        [ push_back(at_c<1>(_val), _1) ]
                        [ at_c<0>(_val) = kite::semantics::DEREF_FILTER ])
                      >> *deref_types [ push_back(at_c<1>(_val), _1) ]
                    | grouping_statement [ _val = _1 ];
                      
                grouping_statement = 
                      ( '(' >> add_subtract_statement [ _val = _1 ] >> ')' )
                      | const_statement [ _val = _1 ];

                // Statements.
                math_statement = assign_statement [ _val = _1 ];
                loop_statement = 
                    (  lit("while") [ at_c<0>(_val) = kite::semantics::WHILE ]
                     | lit("until") [ at_c<0>(_val) = kite::semantics::UNTIL ])
                    >> '(' >> or_statement [ push_back(at_c<1>(_val), _1) ] >> ')'
                    >> '['
                    >> start [ push_back(at_c<1>(_val), _1) ]
                    >> ']';
                decide_statement = 
                       lit("decide") [ at_c<0>(_val) = kite::semantics::DECIDE ]
                    >> '['
                    >> +(  '(' >> or_statement [ push_back(at_c<1>(_val), _1) ]
                         >> ')' >> '[' >> start [ push_back(at_c<1>(_val), _1) ] >> ']')
                    >> ']';
                
                statement = 
                      (math_statement [ _val = _1 ] >> ';')
                    | loop_statement [ _val = _1 ]
                    | decide_statement [ _val = _1 ];
                start = (*statement [ push_back(at_c<1>(_val), _1) ]) [ at_c<0>(_val) = kite::semantics::ITERATE ];
            }
            
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> start;
            qi::rule<Iterator, semantics::syntax_tree_node(), ascii::space_type> statement;
            qi::rule<Iterator, semantics::syntax_tree(), ascii::space_type> math_statement;
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
        };
    }
}

BOOST_FUSION_ADAPT_STRUCT(
    kite::semantics::syntax_tree,
    (kite::semantics::code_operation, op)
    (std::vector<kite::semantics::syntax_tree_node>, children)
)

#endif