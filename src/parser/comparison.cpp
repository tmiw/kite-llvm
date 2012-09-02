#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T, typename U>
        void kite_grammar<T, U>::initialize_comparison_rules()
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
            
            comparison_equals_statement = 
                   comparison_less_greater_statement [ _val = _1 ]
                >> *(    (lit("==")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::EQUALS ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ] |
                          lit("!=")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::NOT_EQUALS ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ] |
                          lit("isof")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::ISOF_CLASS ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ] |
                          lit("is")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::IS_CLASS ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ])
                      > comparison_less_greater_statement [ push_back(at_c<1>(_val), _1) ]);
                
            comparison_less_greater_statement =
                   bit_shift_statement [ _val = _1 ]
                >> *((
                          lit("<=")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::LESS_OR_EQUALS ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ] |
                          lit(">=")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::GREATER_OR_EQUALS ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ] |
                          lit("<")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::LESS_THAN ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ] |
                          lit(">")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::GREATER_THAN ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ])
                      > bit_shift_statement [ push_back(at_c<1>(_val), _1) ]);
        }
        
        kite_grammar<pos_iterator_type, BOOST_TYPEOF(KITE_SKIP_RULE)> comparison_grammar;
    }
}
