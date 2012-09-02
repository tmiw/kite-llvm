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
                (real_parser<double, strict_real_policies<double> >() | int_ | unesc_str | method_ref_statement | anon_method_statement) [ _val = _1 ]
                | lit("true") [ _val = true ]
                | lit("false") [ _val = false ]
                | lit("null") [ _val = (void*)NULL ];
            
            list_statement = 
                lit("[") [ at_c<0>(_val) = kite::semantics::LIST_VAL ] >>
                -(or_statement [ push_back(at_c<1>(_val), _1) ] % ',') >>
                lit("]");
            
            const_statement = 
                  numeric_value [ push_back(at_c<1>(_val), _1) ] [ at_c<0>(_val) = kite::semantics::CONST ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                | list_statement [ _val = _1 ]
                | identifier [ push_back(at_c<1>(_val), _1) ] [ at_c<0>(_val) = kite::semantics::VARIABLE ] >> iter_pos [ at_c<2>(_val) = phoenix::construct<semantics::syntax_tree_position>(_1) ]
                | make_statement [ _val = _1 ];
            
            identifier =
                (qi::lexeme[ (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_')) ] 
                    - lit("decide") 
                    - lit("while") 
                    - lit("until")
                    - lit("return")
                    - lit("true")
                    - lit("make")
                    - lit("run")
                    - lit("catch")
                    - lit("method")
                    - lit("operator")
                    - lit("construct")
                    - lit("false")); // [ _val = _1 ];
            
            operator_identifier = 
                qi::string("plus") |
                qi::string("minus") |
                qi::string("multiply") |
                qi::string("divide") |
                qi::string("mod") |
                qi::string("unplus") |
                qi::string("unminus") |
                qi::string("map") |
                qi::string("reduce") |
                qi::string("array") |
                qi::string("array_set") |
                qi::string("equals") |
                qi::string("nequals") |
                qi::string("lt") |
                qi::string("gt") |
                qi::string("leq") |
                qi::string("geq") |
                qi::string("and") |
                qi::string("or") |
                qi::string("not") |
                qi::string("xor") |
                qi::string("lshift") |
                qi::string("rshift") |
                qi::string("call") |
                qi::string("property");
        }
        
        kite_grammar<pos_iterator_type, BOOST_TYPEOF(KITE_SKIP_RULE)> constants_grammar;
    }
}
