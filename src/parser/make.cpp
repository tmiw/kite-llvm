#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T, typename U>
        void kite_grammar<T, U>::initialize_make_rules()
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
            
            make_statement =
                   lit("make") [ at_c<0>(_val) = kite::semantics::MAKE ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                >> deref_filter_only_statement [ push_back(at_c<1>(_val), _1) ] 
                >> (lit('(') >> -(or_statement [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')'));
        }
        
        kite_grammar<pos_iterator_type, BOOST_TYPEOF(KITE_SKIP_RULE)> make_grammar;
    }
}
