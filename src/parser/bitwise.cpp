#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T, typename U>
        void kite_grammar<T, U>::initialize_bitwise_rules()
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
            
            or_statement = 
                   xor_statement [ _val = _1 ]
                >> *(    lit("or")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::OR ]
                      > xor_statement [ push_back(at_c<1>(_val), _1) ]);
                    
            xor_statement = 
                   and_statement [ _val = _1 ]
                >> *(    lit("xor")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::XOR ]
                      > and_statement [ push_back(at_c<1>(_val), _1) ]);
            
            and_statement = 
                   comparison_equals_statement [ _val = _1 ]
                >> *(    lit("and")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::AND ]
                      > comparison_equals_statement [ push_back(at_c<1>(_val), _1) ]);
        }
        
        //kite_grammar<std::string::const_iterator> bitwise_grammar;
    }
}
