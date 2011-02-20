/*****************************************************************************
 * Copyright (c) 2011, Mooneer Salem
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Kite Language organization nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MOONEER SALEM ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MOONEER SALEM BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/

#ifndef KITE_CODEGEN__LLVM_NODE_CODEGEN_H
#define KITE_CODEGEN__LLVM_NODE_CODEGEN_H

#include <semantics/syntax_tree.h>
#include <stdlib/System/integer.h>
#include <stdlib/System/string.h>
#include <stdlib/System/boolean.h>
#include <stdlib/System/float.h>
#include "llvm_compile_state.h"
using namespace kite::stdlib;

namespace kite
{
    namespace codegen
    {
        struct llvm_node_codegen_params
        {
            semantics::syntax_tree const &tree;
            Value *prev;
            
            llvm_node_codegen_params(semantics::syntax_tree const &tree, Value *prev)
            : tree(tree), prev(prev)
            {
                // empty
            }
        };
        
        struct llvm_node_codegen : boost::static_visitor<Value*>
        {
        public:   
            llvm_node_codegen(llvm_compile_state &state);
            Value *operator()(semantics::syntax_tree const &tree) const;
            
            // Type-specific codegen methods (for use in CONST tree type only)
            Value *operator()(int const &val) const;
            Value *operator()(double const &val) const;
            Value *operator()(bool const &val) const;
            Value *operator()(std::string const &val) const;
            
            // For deref_filter
            Value *operator()(llvm_node_codegen_params &param) const;
        private:
            llvm_compile_state &state;
            
            Value *codegen_iterate_op(semantics::syntax_tree const &tree) const;
            Value *codegen_const_op(semantics::syntax_tree const &tree) const;
            Value *codegen_binary_op(semantics::syntax_tree const &tree) const;
            Value *codegen_unary_plus_op(semantics::syntax_tree const &tree) const;
            Value *codegen_unary_minus_op(semantics::syntax_tree const &tree) const;
            Value *codegen_not_op(semantics::syntax_tree const &tree) const;
            Value *codegen_map_op(semantics::syntax_tree const &tree) const;
            Value *codegen_reduce_op(semantics::syntax_tree const &tree) const;
            Value *codegen_deref_filter_op(semantics::syntax_tree const &tree) const;
            Value *codegen_variable_op(semantics::syntax_tree const &tree) const;
            Value *codegen_assign_op(semantics::syntax_tree const &tree) const;
            Value *codegen_loop_op(semantics::syntax_tree const &tree) const;
            Value *codegen_decide_op(semantics::syntax_tree const &tree) const;
            
            Value *codegen_deref_property_op(semantics::syntax_tree const &tree, Value *prev) const;
            Value *codegen_deref_method_op(semantics::syntax_tree const &tree, Value *prev) const;
            Value *codegen_deref_array_op(semantics::syntax_tree const &tree, Value *prev) const;
            
            semantics::builtin_types get_type(Value *val) const;
            stdlib::object_method_map &get_method_map(semantics::builtin_types type) const;
            std::string type_to_code(semantics::builtin_types type) const;
            const Type *kite_type_to_llvm_type(semantics::builtin_types type) const;
            std::string type_to_method_prefix(semantics::builtin_types type) const;
        };
    }
}

#endif