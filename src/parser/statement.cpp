#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T>
        void kite_grammar<T>::initialize_statement_rules()
        {
            using namespace qi::labels;
            using phoenix::at_c;
            using phoenix::push_back;
            
            math_statement = assign_statement;
            statement = 
                ( loop_statement 
                | decide_statement 
                | method_statement 
                | math_statement ) [ _val = _1 ] >> ';';
            start = (*statement [ push_back(at_c<1>(_val), _1) ]) [ at_c<0>(_val) = kite::semantics::ITERATE ];
        }
        
        kite_grammar<std::string::const_iterator> statement_grammar;
    }
}