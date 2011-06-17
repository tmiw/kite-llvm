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
            using phoenix::push_back;
            using phoenix::push_front;
            using phoenix::begin;
            using phoenix::end;
            using phoenix::erase;
            using phoenix::front;
            
            method_statement =
                   lit("method") [ at_c<0>(_val) = kite::semantics::METHOD ]
                > identifier [ push_back(at_c<1>(_val), _1) ]
                >> (lit('(') >> -(identifier [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')'))
                > '[' >> start [ push_back(at_c<1>(_val), _1) ] >> ']';
            
            anon_method_statement =
                   lit("method") [ at_c<0>(_val) = kite::semantics::METHOD ] [ push_back(at_c<1>(_val), std::string("__AnonMethod")) ]
                >> (lit('(') >> -(identifier [ push_back(at_c<1>(_val), _1) ] % ',') >> lit(')'))
                > '[' >> start [ push_back(at_c<1>(_val), _1) ] >> ']';
        }
        
        //kite_grammar<std::string::const_iterator> method_grammar;
    }
}
