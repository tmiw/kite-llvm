#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T, typename U>
        void kite_grammar<T, U>::initialize_statement_rules()
        {
            using namespace qi::labels;
            using phoenix::at_c;
            using phoenix::push_back;
            using ascii::char_;
            using boost::spirit::eol;
            using qi::no_skip;
            using qi::lit;

            math_statement = assign_statement;
            statement = 
                ( loop_statement 
                | decide_statement 
                | method_statement 
                | class_statement
                | constructor_statement
                | exception_statement
                | math_statement ) [ _val = _1 ] > ';';
            start = (*statement [ push_back(at_c<1>(_val), _1) ]) [ at_c<0>(_val) = kite::semantics::ITERATE ];
        }
        
        //kite_grammar<std::string::const_iterator> statement_grammar;
    }
}
