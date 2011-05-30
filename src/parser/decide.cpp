#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T>
        void kite_grammar<T>::initialize_decide_rules()
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
            
            decide_statement = 
                   lit("decide") [ at_c<0>(_val) = kite::semantics::DECIDE ]
                >> '['
                >>    ((
                        (
                          ('(' >> or_statement [ push_back(at_c<1>(_val), _1) ] >> ')') | 
                          lit("true") [ push_back(at_c<1>(_val), true) ]
                        )
                >>     '[' >> start [ push_back(at_c<1>(_val), _1) ] >> ']') % ',')
                >> ']';
        }
        
        kite_grammar<std::string::const_iterator> decide_grammar;
    }
}