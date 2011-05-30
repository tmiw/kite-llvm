#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T>
        void kite_grammar<T>::initialize_statement_rules()
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
            
            math_statement = assign_statement [ _val = _1 ];
            statement = 
                ( loop_statement [ _val = _1 ]
                | decide_statement [ _val = _1 ]
                | method_statement [ _val = _1 ]
                | math_statement [ _val = _1 ]) >> ';';
            start = (*statement [ push_back(at_c<1>(_val), _1) ]) [ at_c<0>(_val) = kite::semantics::ITERATE ];
        }
        
        kite_grammar<std::string::const_iterator> statement_grammar;
    }
}