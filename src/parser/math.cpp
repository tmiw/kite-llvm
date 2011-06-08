#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T>
        void kite_grammar<T>::initialize_math_rules()
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
            
            bit_shift_statement = 
                   add_subtract_statement [ _val = _1 ]
                >> *(    (lit("<<")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::LEFT_SHIFT ] |
                          lit(">>")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::RIGHT_SHIFT ])
                      >> add_subtract_statement [ push_back(at_c<1>(_val), _1) ]);
                
            add_subtract_statement = 
                   multiply_divide_statement [ _val = _1 ]
                >> *(    (lit("+")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::ADD ] |
                          lit("-")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::SUBTRACT ])
                      >> multiply_divide_statement [ push_back(at_c<1>(_val), _1) ]);

            multiply_divide_statement = 
                   unary_statement [ _val = _1 ]
                >> *(    (lit("*")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::MULTIPLY ] |
                          lit("/")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::DIVIDE ] |
                          lit("%")  [ push_front(at_c<1>(_val), _val) ] [ erase(at_c<1>(_val), begin(at_c<1>(_val)) + 1, end(at_c<1>(_val))) ] [ at_c<0>(_val) = kite::semantics::MODULO ])
                      >> unary_statement [ push_back(at_c<1>(_val), _1) ]);
            
            unary_statement = 
                ((lit('+') [ at_c<0>(_val) = kite::semantics::UNARY_PLUS ]) |
                 (lit('-') [ at_c<0>(_val) = kite::semantics::UNARY_MINUS ]) |
                 (lit("not") [ at_c<0>(_val) = kite::semantics::NOT ]))
                 >> unary_statement [ push_back(at_c<1>(_val), _1) ]
                | map_reduce_statement [ _val = _1 ];
        }
        
        //kite_grammar<std::string::const_iterator> math_grammar;
    }
}