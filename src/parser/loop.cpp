#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T, typename U>
        void kite_grammar<T, U>::initialize_loop_rules()
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
            
            loop_statement = 
                (  lit("while") [ at_c<0>(_val) = kite::semantics::WHILE ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                 | lit("until") [ at_c<0>(_val) = kite::semantics::UNTIL ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ])
                >> '(' >> or_statement [ push_back(at_c<1>(_val), _1) ] >> ')'
                >> '['
                >> start [ push_back(at_c<1>(_val), _1) ]
                >> ']';
                
            break_continue_statement =
                  lit("break") [ at_c<0>(_val) = kite::semantics::BREAK ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                | lit("continue") [ at_c<0>(_val) = kite::semantics::CONTINUE ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                | lit("return") >> const_statement [ push_back(at_c<1>(_val), _1) ] [ at_c<0>(_val) = kite::semantics::RETURN_VAL ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ];
        }
        
        kite_grammar<pos_iterator_type, BOOST_TYPEOF(KITE_SKIP_RULE)> loop_grammar;
    }
}
