#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T, typename U>
        void kite_grammar<T, U>::initialize_method_rules()
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
            using phoenix::at;
            using phoenix::push_back;
            using phoenix::push_front;
            using phoenix::begin;
            using phoenix::end;
            using phoenix::erase;
            using phoenix::front;
            
            method_statement =
                   lit("method") [ at_c<0>(_val) = kite::semantics::METHOD ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                > identifier [ push_back(at_c<1>(_val), _1) ]
                >> (lit('(') >> -(identifier [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')'))
                > '[' >> start [ push_back(at_c<1>(_val), _1) ] >> ']';
            
            operator_statement =
                   lit("operator") [ at_c<0>(_val) = kite::semantics::OPERATOR ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                > operator_identifier [ push_back(at_c<1>(_val), _1) ]
                >> (lit('(') >> -(identifier [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')'))
                > '[' >> start [ push_back(at_c<1>(_val), _1) ] >> ']';
            
            anon_method_statement =
                   lit("method") [ at_c<0>(_val) = kite::semantics::METHOD ] [ push_back(at_c<1>(_val), std::string("__AnonMethod")) ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                >> (lit('(') >> -(identifier [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')'))
                > '[' >> start [ push_back(at_c<1>(_val), _1) ] >> ']';
                
            method_ref_statement =
                   lit("method_ref") [ at_c<0>(_val) = kite::semantics::METHOD_REF ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                >> (lit('(') > 
                      or_statement [ push_back(at_c<1>(_val), _1) ] > ',' >
                      unesc_str [ push_back(at_c<1>(_val), _1) ] > ',' >
                      int_ [ push_back(at_c<1>(_val), _1) ] >> lit(')'));
        }
        
        kite_grammar<pos_iterator_type, BOOST_TYPEOF(KITE_SKIP_RULE)> method_grammar;
    }
}
