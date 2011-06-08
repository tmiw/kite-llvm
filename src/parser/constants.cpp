#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T>
        void kite_grammar<T>::initialize_const_rules()
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
            
            unesc_str = '"' >> *(unesc_char | (char_ - "\\" - "\"") | "\\x" >> qi::hex) >> '"';
            unesc_char.add("\\a", '\a')("\\b", '\b')("\\f", '\f')("\\n", '\n')
                          ("\\r", '\r')("\\t", '\t')("\\v", '\v')("\\\\", '\\')
                          ("\\\'", '\'')("\\\"", '\"');
            numeric_value = 
                (int_ | double_ | unesc_str) [ _val = _1 ]
                | lit("true") [ _val = true ]
                | lit("false") [ _val = false ];
            
            const_statement = 
                  numeric_value [ push_back(at_c<1>(_val), _1) ] [ at_c<0>(_val) = kite::semantics::CONST ]
                | identifier [ push_back(at_c<1>(_val), _1) ] [ at_c<0>(_val) = kite::semantics::VARIABLE ];
                  
            identifier =
                (qi::lexeme[ (qi::alpha | '_') >> *(qi::alnum | '_') ] 
                    - lit("decide") 
                    - lit("while") 
                    - lit("until")
                    - lit("true")
                    - lit("false")); // [ _val = _1 ];
        }
        
        //kite_grammar<std::string::const_iterator> constants_grammar;
    }
}