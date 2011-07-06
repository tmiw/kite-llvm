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
            import_statement = 
                lit("import") [ at_c<0>(_val) = kite::semantics::IMPORT ] > 
                unesc_str [ push_back(at_c<1>(_val), _1) ];
            
            statement = 
                ( loop_statement 
                | decide_statement 
                | method_statement 
                | operator_statement
                | class_statement
                | constructor_statement
                | destructor_statement
                | exception_statement
                | import_statement
                | math_statement ) [ _val = _1 ] > ';';
            start = (*statement [ push_back(at_c<1>(_val), _1) ]) [ at_c<0>(_val) = kite::semantics::ITERATE ];
        }
        
        kite_grammar<pos_iterator_type, BOOST_TYPEOF(KITE_SKIP_RULE)> statement_grammar;
    }
}
