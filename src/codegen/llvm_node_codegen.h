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
            
            Value *generate_llvm_method(std::string name, std::vector<std::string> &argnames, semantics::syntax_tree &body) const;

            static std::string type_to_code(semantics::builtin_types type);
            static semantics::builtin_types get_type(Value *val);
            static const Type *kite_type_to_llvm_type(semantics::builtin_types type);
            static std::string kite_type_to_function_prefix(semantics::builtin_types type);
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
            Value *codegen_mapreduce_op(semantics::syntax_tree const &tree) const;
            Value *codegen_deref_filter_op(semantics::syntax_tree const &tree) const;
            Value *codegen_variable_op(semantics::syntax_tree const &tree) const;
            Value *codegen_assign_op(semantics::syntax_tree const &tree) const;
            Value *codegen_loop_op(semantics::syntax_tree const &tree) const;
            Value *codegen_decide_op(semantics::syntax_tree const &tree) const;
            Value *codegen_method_op(semantics::syntax_tree const &tree) const;
            Value *codegen_constructor_op(semantics::syntax_tree const &tree) const;
            Value *codegen_destructor_op(semantics::syntax_tree const &tree) const;
            Value *codegen_run_catch_op(semantics::syntax_tree const &tree) const;
            Value *codegen_isof_op(semantics::syntax_tree const &tree) const;
            Value *codegen_list_op(semantics::syntax_tree const &tree) const;
            
            Value *codegen_deref_property_op(semantics::syntax_tree const &tree, Value *prev) const;
            Value *codegen_deref_method_op(semantics::syntax_tree const &tree, Value *prev) const;
            Value *codegen_deref_method_relative_self_op(semantics::syntax_tree const &tree) const;
            Value *codegen_deref_array_op(semantics::syntax_tree const &tree, Value *prev) const;
            
            Value *codegen_class_op(semantics::syntax_tree const &tree) const;
            Value *codegen_make_op(semantics::syntax_tree const &tree) const;
            
            stdlib::object_method_map &get_method_map(semantics::builtin_types type) const;
            Value *generate_llvm_method_call(Value *self, std::string name, std::vector<Value*> &params) const;
            Value *generate_llvm_dynamic_object_alloc(Value *orig) const;
            void generate_llvm_dynamic_object_set_parent(Value *obj, Value *parent) const;
            void generate_llvm_dynamic_object_set_name(Value *obj) const;
            Value *generate_llvm_dynamic_object_get_property(Value *obj, std::string name, bool set = false) const;
            Value *generate_llvm_method_alloc(Value *method) const;
            Value *generate_llvm_dynamic_object_get_root() const;

            const Type *get_method_type() const;
            const Type *get_object_type() const;
        };
    }
}

#endif
