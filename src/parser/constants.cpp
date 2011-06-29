#include "grammar.h"

namespace kite
{
    namespace parser
    {
        template<typename T, typename U>
        void kite_grammar<T, U>::initialize_const_rules()
        {
            using qi::lit;
            using qi::lexeme;
            using qi::raw;
            using ascii::char_;
            using qi::int_;
            using qi::double_;
            using qi::real_parser;
            using qi::strict_real_policies;
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
                (real_parser<double, strict_real_policies<double> >() | int_ | unesc_str | anon_method_statement) [ _val = _1 ]
                | lit("true") [ _val = true ]
                | lit("false") [ _val = false ];
            
            const_statement = 
                  numeric_value [ push_back(at_c<1>(_val), _1) ] [ at_c<0>(_val) = kite::semantics::CONST ]
                | identifier [ push_back(at_c<1>(_val), _1) ] [ at_c<0>(_val) = kite::semantics::VARIABLE ]
                | make_statement [ _val = _1 ];
            
            identifier =
                (qi::lexeme[ (qi::alpha | '_') >> *(qi::alnum | '_') ] 
                    - lit("decide") 
                    - lit("while") 
                    - lit("until")
                    - lit("true")
                    - lit("make")
                    - lit("run")
                    - lit("catch")
                    - lit("method")
                    - lit("operator")
                    - lit("construct")
                    - lit("false")); // [ _val = _1 ];
            
            operator_identifier = 
                lit("plus") |
                lit("minus") |
                lit("multiply") |
                lit("divide") |
                lit("mod") |
                lit("unplus") |
                lit("unminus") |
                lit("map") |
                lit("reduce") |
                lit("array") |
                lit("array_set") |
                lit("equals") |
                lit("nequals") |
                lit("lt") |
                lit("gt") |
                lit("leq") |
                lit("geq") |
                lit("and") |
                lit("or") |
                lit("not") |
                lit("xor") |
                lit("lshift") |
                lit("rshift") |
                lit("call") |
                lit("property");
        }
        
        kite_grammar<pos_iterator_type, BOOST_TYPEOF(KITE_SKIP_RULE)> constants_grammar;
    }
}
