#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T>
        void kite_grammar<T>::initialize_deref_rules()
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
            using phoenix::push_front;
            using phoenix::begin;
            using phoenix::end;
            using phoenix::erase;
            using phoenix::front;
            
            deref_property_statement =
                lit('.') [ at_c<0>(_val) = kite::semantics::DEREF_PROPERTY ] >>
                identifier [ push_back(at_c<1>(_val), _1) ];
            
            deref_method_statement =
                lit('|') [ at_c<0>(_val) = kite::semantics::DEREF_METHOD ] >>
                identifier [ push_back(at_c<1>(_val), _1) ] >>
                -(lit('(') >> -(or_statement [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')'));
            
            deref_method_mandatory_params =
                   identifier [ push_back(at_c<1>(_val), _1) ]
                >> lit('(') [ at_c<0>(_val) = kite::semantics::DEREF_METHOD_RELATIVE_SELF ]
                >> -(or_statement [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')');
                
            deref_array_statement =
                lit('[') [ at_c<0>(_val) = kite::semantics::DEREF_ARRAY ] >>
                or_statement [ push_back(at_c<1>(_val), _1) ] >>
                lit(']');
                
            deref_types = 
                (deref_property_statement | deref_method_statement | deref_array_statement) [ _val = _1 ];
            
            deref_filter_only_statement =
                (    grouping_statement 
                        [ push_back(at_c<1>(_val), _1) ]
                        [ at_c<0>(_val) = kite::semantics::DEREF_FILTER ])
                >> *deref_types [ push_back(at_c<1>(_val), _1) ];
                        
            deref_filter_statement =
                    (deref_method_mandatory_params 
                         [ push_back(at_c<1>(_val), _1) ]
                         [ at_c<0>(_val) = kite::semantics::DEREF_FILTER ]
                  >> *deref_types [ push_back(at_c<1>(_val), _1) ])
                  |
                  ((    grouping_statement 
                          [ push_back(at_c<1>(_val), _1) ]
                          [ at_c<0>(_val) = kite::semantics::DEREF_FILTER ])
                          >> *deref_types [ push_back(at_c<1>(_val), _1) ]);
        }
        
        kite_grammar<std::string::const_iterator> deref_grammar;
    }
}