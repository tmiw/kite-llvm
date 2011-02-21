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

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/assign.hpp>
#include "llvm_node_codegen.h"
using namespace boost::assign;

namespace kite
{
    namespace codegen
    {
        // Map of arithmetic operations with two parameters.
        typedef Value *(IRBuilder<>::*IRBuilderFPtr)(Value*,Value*,const Twine&);
        typedef std::pair<semantics::code_operation, semantics::builtin_types> CodeOperationKey;
        typedef std::map<CodeOperationKey, IRBuilderFPtr> CodeOperationMap;
        static CodeOperationMap codegen_map = map_list_of
            (CodeOperationKey(semantics::ADD, semantics::INTEGER), &IRBuilder<>::CreateAdd)
            (CodeOperationKey(semantics::ADD, semantics::FLOAT), &IRBuilder<>::CreateFAdd)
            (CodeOperationKey(semantics::SUBTRACT, semantics::INTEGER), &IRBuilder<>::CreateSub)
            (CodeOperationKey(semantics::SUBTRACT, semantics::FLOAT), &IRBuilder<>::CreateFSub)
            (CodeOperationKey(semantics::MULTIPLY, semantics::INTEGER), &IRBuilder<>::CreateMul)
            (CodeOperationKey(semantics::MULTIPLY, semantics::FLOAT), &IRBuilder<>::CreateFMul)
            (CodeOperationKey(semantics::DIVIDE, semantics::INTEGER), &IRBuilder<>::CreateSDiv)
            (CodeOperationKey(semantics::DIVIDE, semantics::FLOAT), &IRBuilder<>::CreateFDiv)
            (CodeOperationKey(semantics::MODULO, semantics::INTEGER), &IRBuilder<>::CreateSRem)
            (CodeOperationKey(semantics::MODULO, semantics::FLOAT), &IRBuilder<>::CreateFRem)
            (CodeOperationKey(semantics::LEFT_SHIFT, semantics::INTEGER), IRBuilderFPtr(&IRBuilder<>::CreateShl))
            (CodeOperationKey(semantics::RIGHT_SHIFT, semantics::INTEGER), IRBuilderFPtr(&IRBuilder<>::CreateLShr))
            (CodeOperationKey(semantics::LESS_THAN, semantics::INTEGER), &IRBuilder<>::CreateICmpSLT)
            (CodeOperationKey(semantics::LESS_THAN, semantics::FLOAT), &IRBuilder<>::CreateFCmpULT)
            (CodeOperationKey(semantics::LESS_OR_EQUALS, semantics::INTEGER), &IRBuilder<>::CreateICmpSLE)
            (CodeOperationKey(semantics::LESS_OR_EQUALS, semantics::FLOAT), &IRBuilder<>::CreateFCmpULE)
            (CodeOperationKey(semantics::GREATER_THAN, semantics::INTEGER), &IRBuilder<>::CreateICmpSGT)
            (CodeOperationKey(semantics::GREATER_THAN, semantics::FLOAT), &IRBuilder<>::CreateFCmpUGT)
            (CodeOperationKey(semantics::GREATER_OR_EQUALS, semantics::INTEGER), &IRBuilder<>::CreateICmpSGE)
            (CodeOperationKey(semantics::GREATER_OR_EQUALS, semantics::FLOAT), &IRBuilder<>::CreateFCmpUGE)
            (CodeOperationKey(semantics::EQUALS, semantics::INTEGER), &IRBuilder<>::CreateICmpEQ)
            (CodeOperationKey(semantics::EQUALS, semantics::BOOLEAN), &IRBuilder<>::CreateICmpEQ)
            (CodeOperationKey(semantics::EQUALS, semantics::FLOAT), &IRBuilder<>::CreateFCmpUEQ)
            (CodeOperationKey(semantics::NOT_EQUALS, semantics::INTEGER), &IRBuilder<>::CreateICmpNE)
            (CodeOperationKey(semantics::NOT_EQUALS, semantics::BOOLEAN), &IRBuilder<>::CreateICmpNE)
            (CodeOperationKey(semantics::NOT_EQUALS, semantics::FLOAT), &IRBuilder<>::CreateFCmpUNE)
            (CodeOperationKey(semantics::AND, semantics::INTEGER), IRBuilderFPtr(&IRBuilder<>::CreateAnd))
            (CodeOperationKey(semantics::AND, semantics::BOOLEAN), IRBuilderFPtr(&IRBuilder<>::CreateAnd))
            (CodeOperationKey(semantics::OR, semantics::INTEGER), IRBuilderFPtr(&IRBuilder<>::CreateOr))
            (CodeOperationKey(semantics::OR, semantics::BOOLEAN), IRBuilderFPtr(&IRBuilder<>::CreateOr))
            (CodeOperationKey(semantics::XOR, semantics::INTEGER), IRBuilderFPtr(&IRBuilder<>::CreateXor))
            (CodeOperationKey(semantics::XOR, semantics::BOOLEAN), IRBuilderFPtr(&IRBuilder<>::CreateXor));
        
        llvm_node_codegen::llvm_node_codegen(llvm_compile_state &state)
        : state(state)
        {
            // empty
        }
        
        Value *llvm_node_codegen::operator()(semantics::syntax_tree const &tree) const
        {
            Value *ret = NULL;
            
            switch(tree.op)
            {
                case semantics::ITERATE:
                    ret = codegen_iterate_op(tree);
                    break;
                case semantics::CONST:
                    ret = codegen_const_op(tree);
                    break;
                case semantics::ADD:
                case semantics::SUBTRACT:
                case semantics::MULTIPLY:
                case semantics::DIVIDE:
                case semantics::MODULO:
                case semantics::LEFT_SHIFT:
                case semantics::RIGHT_SHIFT:
                case semantics::LESS_THAN:
                case semantics::GREATER_THAN:
                case semantics::LESS_OR_EQUALS:
                case semantics::GREATER_OR_EQUALS:
                case semantics::EQUALS:
                case semantics::NOT_EQUALS:
                case semantics::AND:
                case semantics::OR:
                case semantics::XOR:
                    ret = codegen_binary_op(tree);
                    break;
                case semantics::UNARY_PLUS:
                    ret = codegen_unary_plus_op(tree);
                    break;
                case semantics::UNARY_MINUS:
                    ret = codegen_unary_minus_op(tree);
                    break;
                case semantics::NOT:
                    ret = codegen_not_op(tree);
                    break;
                case semantics::MAP:
                    ret = codegen_map_op(tree);
                    break;
                case semantics::REDUCE:
                    ret = codegen_reduce_op(tree);
                    break;
                case semantics::DEREF_FILTER:
                    ret = codegen_deref_filter_op(tree);
                    break;
                case semantics::VARIABLE:
                    ret = codegen_variable_op(tree);
                    break;
                case semantics::ASSIGN:
                    ret = codegen_assign_op(tree);
                    break;
                case semantics::WHILE:
                case semantics::UNTIL:
                    ret = codegen_loop_op(tree);
                    break;
                case semantics::DECIDE:
                    ret = codegen_decide_op(tree);
                    break;
            }
            
            return ret;
        }
        
        Value *llvm_node_codegen::operator()(int const &val) const
        {
            return ConstantInt::get(getGlobalContext(), APInt(32, val, true));
        }
        
        Value *llvm_node_codegen::operator()(double const &val) const
        {
            return ConstantFP::get(getGlobalContext(), APFloat(val));
        }
        
        Value *llvm_node_codegen::operator()(bool const &val) const
        {
            return ConstantInt::get(getGlobalContext(), APInt(1, val, true));
        }
        
        Value *llvm_node_codegen::operator()(std::string const &val) const
        {
            return state.module_builder().CreateGlobalStringPtr(val.c_str());
        }
        
        Value *llvm_node_codegen::codegen_iterate_op(semantics::syntax_tree const &tree) const
        {
            Value *ret = NULL;
            
            BOOST_FOREACH(semantics::syntax_tree_node const &node, tree.children)
            {
                ret = boost::apply_visitor(llvm_node_codegen(state), node);
            }
            
            return ret;
        }
        
        Value *llvm_node_codegen::codegen_const_op(semantics::syntax_tree const &tree) const
        {
            return boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
        }
        
        Value *llvm_node_codegen::codegen_binary_op(semantics::syntax_tree const &tree) const
        {
            Value *lhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            Value *rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
            
            // TODO
            assert(lhs->getType() == rhs->getType());
            semantics::builtin_types op_type = get_type(lhs);
            IRBuilderFPtr ptr = codegen_map[CodeOperationKey(tree.op, op_type)];
            assert(ptr != NULL);
            return (state.module_builder().*ptr)(lhs, rhs, "");
        }
        
        Value *llvm_node_codegen::codegen_unary_plus_op(semantics::syntax_tree const &tree) const
        {
            return boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
        }
        
        Value *llvm_node_codegen::codegen_unary_minus_op(semantics::syntax_tree const &tree) const
        {
            Value *lhs;
            Value *rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            semantics::builtin_types op_type = get_type(rhs);
        
            if (op_type == semantics::INTEGER)
            {
                lhs = ConstantInt::get(getGlobalContext(), APInt(32, 0, true));
                lhs = state.module_builder().CreateSub(lhs, rhs);
            }
            else
            {
                lhs = ConstantFP::get(getGlobalContext(), APFloat(0.0));
                lhs = state.module_builder().CreateFSub(lhs, rhs);
            }
            
            return lhs;
        }
        
        Value *llvm_node_codegen::codegen_not_op(semantics::syntax_tree const &tree) const
        {
            Value *rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            return state.module_builder().CreateNot(rhs);
        }
        
        Value *llvm_node_codegen::codegen_map_op(semantics::syntax_tree const &tree) const
        {
            // TODO
            return NULL;
        }
        
        Value *llvm_node_codegen::codegen_reduce_op(semantics::syntax_tree const &tree) const
        {
            // TODO
            return NULL;
        }
        
        Value *llvm_node_codegen::codegen_deref_filter_op(semantics::syntax_tree const &tree) const
        {
            Value *lhs = NULL;
            
            for(int i = 0; i < tree.children.size(); i++)
            {
                if (lhs == NULL)
                {
                    lhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                }
                else
                {
                    llvm_node_codegen_params param(boost::get<semantics::syntax_tree>(tree.children[i]), lhs);
                    boost::variant<llvm_node_codegen_params> variant_param(param);
                    lhs = boost::apply_visitor(
                        llvm_node_codegen(state),
                        variant_param);
                }
            }
            
            return lhs;
        }
        
        Value *llvm_node_codegen::operator()(llvm_node_codegen_params &param) const
        {
            Value *ret = NULL;
            
            switch(param.tree.op)
            {
                case semantics::DEREF_PROPERTY:
                    ret = codegen_deref_property_op(param.tree, param.prev);
                    break;
                case semantics::DEREF_METHOD:
                    ret = codegen_deref_method_op(param.tree, param.prev);
                    break;
                case semantics::DEREF_ARRAY:
                    ret = codegen_deref_array_op(param.tree, param.prev);
                    break;
            }
            
            return ret;
        }
        
        Value *llvm_node_codegen::codegen_deref_property_op(semantics::syntax_tree const &tree, Value *prev) const
        {
            // TODO
            return prev;
        }
        
        Value *llvm_node_codegen::codegen_deref_method_op(semantics::syntax_tree const &tree, Value *prev) const
        {
            semantics::builtin_types type = get_type(prev);
            stdlib::object_method_map &method_map = get_method_map(type);
            std::string method_name = boost::get<std::string>(tree.children[0]);
            std::vector<Value*> parameters;
            std::vector<const Type*> parameterTypes;
            
            method_name += std::string("__") + type_to_code(type);
            parameters.push_back(prev);
            parameterTypes.push_back(kite_type_to_llvm_type(type));
            for (int i = 1; i < tree.children.size(); i++)
            {
                Value *param_val = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                method_name += type_to_code(get_type(param_val));
                parameters.push_back(param_val);
                parameterTypes.push_back(kite_type_to_llvm_type(get_type(param_val)));
            }
            function_semantics &semantics = method_map[method_name];
                
            const FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics.first), parameterTypes, false);
            Value *fptrval = ConstantInt::get(getGlobalContext(), APInt(sizeof(void*) << 4, (uint64_t)semantics.second, true));
            Value *fptr = state.module_builder().CreateIntToPtr(fptrval, PointerType::getUnqual(ft));
            prev = state.module_builder().CreateCall(
                fptr,
                parameters.begin(),
                parameters.end()
            );
            return prev;
        }
        
        Value *llvm_node_codegen::codegen_deref_array_op(semantics::syntax_tree const &tree, Value *prev) const
        {
            // TODO
            return prev;
        }
        
        Value *llvm_node_codegen::codegen_variable_op(semantics::syntax_tree const &tree) const
        {
            std::string var_name = boost::get<std::string>(tree.children[0]);
            std::map<std::string, Value*> &sym_stack = state.current_symbol_stack();
            if (sym_stack.count(var_name) == 0)
            {
                // TODO: default value.
                //sym_stack[var_name] = ConstantInt::get(getGlobalContext(), APInt(32, 0, true));
                sym_stack[var_name] = state.module_builder().CreateAlloca(kite_type_to_llvm_type(semantics::INTEGER));
            }
            return state.module_builder().CreateLoad(sym_stack[var_name]);
        }
        
        Value *llvm_node_codegen::codegen_assign_op(semantics::syntax_tree const &tree) const
        {
            Value *lhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            Value *rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
            std::map<std::string, Value*> &sym_stack = state.current_symbol_stack();
            
            Value *ptr = ((LoadInst*)lhs)->getPointerOperand();
            for (std::map<std::string, Value*>::iterator i = sym_stack.begin(); i != sym_stack.end(); i++)
            {
                if (i->second == ptr)
                {
                    if (lhs->getType() != rhs->getType())
                    {
                        sym_stack[i->first] = state.module_builder().CreateAlloca(rhs->getType());
                        ptr = sym_stack[i->first];
                    }
                    state.module_builder().CreateStore(rhs, ptr);
                    return rhs;
                }
            }
            
            // TODO
            assert(0);
        }
        
        Value *llvm_node_codegen::codegen_loop_op(semantics::syntax_tree const &tree) const
        {
            BasicBlock *currentBB = state.module_builder().GetInsertBlock();
            Function *currentFunc = currentBB->getParent();
            BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "loop", currentFunc);
            
            state.module_builder().CreateBr(BB);
            state.module_builder().SetInsertPoint(BB);
            Value *condition = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            
            BasicBlock *bodyBB = BasicBlock::Create(getGlobalContext(), "loopbody", currentFunc);
            BasicBlock *afterLoopBB = BasicBlock::Create(getGlobalContext(), "loopend", currentFunc);
            
            switch(tree.op)
            {
                case semantics::WHILE:
                    state.module_builder().CreateCondBr(condition, bodyBB, afterLoopBB);
                    break;
                case semantics::UNTIL:
                    state.module_builder().CreateCondBr(condition, afterLoopBB, bodyBB);
                    break;
            }
            
            state.module_builder().SetInsertPoint(bodyBB);
            Value *inner = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
            state.module_builder().CreateBr(BB);
            
            state.module_builder().SetInsertPoint(afterLoopBB);
            return ConstantInt::get(getGlobalContext(), APInt(32, 0, true)); // TODO
        }
        
        Value *llvm_node_codegen::codegen_decide_op(semantics::syntax_tree const &tree) const
        {
            BasicBlock *currentBB = state.module_builder().GetInsertBlock();
            Function *currentFunc = currentBB->getParent();
            BasicBlock *endBB = BasicBlock::Create(getGlobalContext(), "decide_end", currentFunc);
            Value *condition = NULL;
            BasicBlock *condBB = BasicBlock::Create(getGlobalContext(), "decide_cond", currentFunc, endBB);
            BasicBlock *actionBB = NULL;
            
            state.module_builder().CreateBr(condBB);
            state.module_builder().SetInsertPoint(condBB);
            for(int i = 0; i < tree.children.size(); i++)
            {
                condition = boost::apply_visitor(llvm_node_codegen(state), tree.children[i++]);
                actionBB = BasicBlock::Create(getGlobalContext(), "decide_true", currentFunc, endBB);
                if (i < tree.children.size() - 1)
                {
                    condBB = BasicBlock::Create(getGlobalContext(), "decide_cond", currentFunc, endBB);
                }
                else
                {
                    condBB = endBB;
                }
                state.module_builder().CreateCondBr(condition, actionBB, condBB);
                state.module_builder().SetInsertPoint(actionBB);
                boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                state.module_builder().CreateBr(endBB);
                state.module_builder().SetInsertPoint(condBB);
            }
            
            state.module_builder().SetInsertPoint(endBB);
            return ConstantInt::get(getGlobalContext(), APInt(32, 0, true)); // TODO
        }
        
        semantics::builtin_types llvm_node_codegen::get_type(Value *val) const
        {
            semantics::builtin_types op_type;
            const Type *type = val->getType();
            
            if (type->isIntegerTy(32)) 
            {
                op_type = semantics::INTEGER;
            }
            else if (type->isIntegerTy(1))
            {
                op_type = semantics::BOOLEAN;
            }
            else if (
                type->isPointerTy() && 
                ((PointerType*)type)->isValidElementType(Type::getInt8Ty(getGlobalContext())) )
            {
                op_type = semantics::STRING;
            }
            else if (type->isDoubleTy())
            {
                op_type = semantics::FLOAT;
            }
            else
            {
                op_type == semantics::OBJECT;
            }
            
            return op_type;
        }
        
        stdlib::object_method_map &llvm_node_codegen::get_method_map(semantics::builtin_types type) const
        {
            switch(type)
            {
                case semantics::INTEGER:
                {
                    return System::integer::method_map;
                }
                case semantics::STRING:
                {
                    return System::string::method_map;
                }
                case semantics::BOOLEAN:
                {
                    return System::boolean::method_map;
                }
                case semantics::FLOAT:
                {
                    return System::fpnum::method_map;
                }
                default:
                {
                    // TODO
                }
            }
        }
        
        std::string llvm_node_codegen::type_to_code(semantics::builtin_types type) const
        {
            switch(type)
            {
                case semantics::INTEGER:
                {
                    return "i";
                }
                case semantics::FLOAT:
                {
                    return "f";
                }
                case semantics::BOOLEAN:
                {
                    return "b";
                }
                case semantics::OBJECT:
                {
                    return "o";
                }
                case semantics::STRING:
                {
                    return "s";
                }
                default:
                {
                    // TODO
                    return "";
                }
            }
        }
        
        const Type *llvm_node_codegen::kite_type_to_llvm_type(semantics::builtin_types type) const
        {
            switch(type)
            {
                case semantics::INTEGER:
                {
                    return Type::getInt32Ty(getGlobalContext());
                }
                case semantics::FLOAT:
                {
                    return Type::getDoubleTy(getGlobalContext());
                }
                case semantics::BOOLEAN:
                {
                    return Type::getInt1Ty(getGlobalContext());
                }
                case semantics::STRING:
                {
                    return PointerType::getUnqual(Type::getInt8Ty(getGlobalContext()));
                }
/*                case semantics::OBJECT:
                {
                    return "o";
                }*/
                default:
                {
                    // TODO
                    return NULL;
                }
            }
        }
        
        std::string llvm_node_codegen::type_to_method_prefix(semantics::builtin_types type) const
        {
            switch(type)
            {
                case semantics::INTEGER:
                {
                    return "System__integer__";
                }
                case semantics::FLOAT:
                {
                    return "System__float__";
                }
                case semantics::BOOLEAN:
                {
                    return "System__boolean__";
                }
                case semantics::OBJECT:
                {
                    return "System__object__";
                }
                case semantics::STRING:
                {
                    return "System__string__";
                }
                default:
                {
                    // TODO
                    return "";
                }
            }
        }
    }
}
