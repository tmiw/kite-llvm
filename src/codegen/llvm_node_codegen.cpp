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

#include <llvm/Support/Dwarf.h>
#include <llvm/Analysis/DebugInfo.h>
#include <setjmp.h>
#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include "llvm_node_codegen.h"
#include "stdlib/language/kite.h"

using namespace boost::assign;
using namespace kite::stdlib;
using namespace llvm::dwarf;

namespace kite
{
    namespace codegen
    {
        // Map of arithmetic operations with two parameters.
        typedef Value *(IRBuilder<>::*IRBuilderFPtr)(Value*,Value*,const Twine&);
        typedef std::pair<semantics::code_operation, semantics::builtin_types> CodeOperationKey;
        typedef std::map<CodeOperationKey, IRBuilderFPtr> CodeOperationMap;
        typedef Value *(IRBuilder<>::*IRBuilderNUWFPtr)(Value*,Value*,const Twine&,bool,bool);
        typedef Value *(IRBuilder<>::*IRBuilderRShiftFPtr)(Value*,Value*,const Twine&,bool);

        static CodeOperationMap codegen_map = map_list_of
            (CodeOperationKey(semantics::ADD, semantics::INTEGER), (IRBuilderFPtr)(IRBuilderNUWFPtr)&IRBuilder<>::CreateAdd)
            (CodeOperationKey(semantics::ADD, semantics::FLOAT), (IRBuilderFPtr)&IRBuilder<>::CreateFAdd)
            (CodeOperationKey(semantics::SUBTRACT, semantics::INTEGER), (IRBuilderFPtr)(IRBuilderNUWFPtr)&IRBuilder<>::CreateSub)
            (CodeOperationKey(semantics::SUBTRACT, semantics::FLOAT), (IRBuilderFPtr)&IRBuilder<>::CreateFSub)
            (CodeOperationKey(semantics::MULTIPLY, semantics::INTEGER), (IRBuilderFPtr)(IRBuilderNUWFPtr)&IRBuilder<>::CreateMul)
            (CodeOperationKey(semantics::MULTIPLY, semantics::FLOAT), (IRBuilderFPtr)&IRBuilder<>::CreateFMul)
            (CodeOperationKey(semantics::DIVIDE, semantics::INTEGER), (IRBuilderFPtr)(IRBuilderRShiftFPtr)&IRBuilder<>::CreateSDiv)
            (CodeOperationKey(semantics::DIVIDE, semantics::FLOAT), (IRBuilderFPtr)&IRBuilder<>::CreateFDiv)
            (CodeOperationKey(semantics::MODULO, semantics::INTEGER), &IRBuilder<>::CreateSRem)
            (CodeOperationKey(semantics::MODULO, semantics::FLOAT), (IRBuilderFPtr)&IRBuilder<>::CreateFRem)
            (CodeOperationKey(semantics::LEFT_SHIFT, semantics::INTEGER), (IRBuilderFPtr)(IRBuilderNUWFPtr)&IRBuilder<>::CreateShl)
            (CodeOperationKey(semantics::RIGHT_SHIFT, semantics::INTEGER), (IRBuilderFPtr)(IRBuilderRShiftFPtr)&IRBuilder<>::CreateLShr)
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
                case semantics::PROP_ASSIGN:
                    ret = codegen_assign_op(tree);
                    break;
                case semantics::WHILE:
                case semantics::UNTIL:
                    ret = codegen_loop_op(tree);
                    break;
                case semantics::DECIDE:
                    ret = codegen_decide_op(tree);
                    break;
                case semantics::METHOD:
                case semantics::OPERATOR:
                    ret = codegen_method_op(tree);
                    break;
                case semantics::EVAL:
                    ret = codegen_eval_op(tree);
                    break;
                case semantics::DEREF_METHOD_RELATIVE_SELF:
                    ret = codegen_deref_method_relative_self_op(tree);
                    break;
                case semantics::CLASS:
                    ret = codegen_class_op(tree);
                    break;
                case semantics::MAKE:
                    ret = codegen_make_op(tree);
                    break;
                case semantics::RUN_CATCH:
                    ret = codegen_run_catch_op(tree);
                    break;
                case semantics::CONSTRUCTOR:
                    ret = codegen_constructor_op(tree);
                    break;
                case semantics::DESTRUCTOR:
                    ret = codegen_destructor_op(tree);
                    break;
                case semantics::IS_CLASS:
                case semantics::ISOF_CLASS:
                    ret = codegen_isof_op(tree);
                    break;
                case semantics::LIST_VAL:
                    ret = codegen_list_op(tree);
                    break;
                case semantics::METHOD_REF:
                    ret = codegen_method_ref_op(tree);
                    break;
                case semantics::IMPORT:
                    ret = codegen_import_op(tree);
                    break;
                case semantics::BREAK:
                case semantics::CONTINUE:
                    ret = codegen_break_continue_op(tree);
                    break;
                case semantics::RETURN_VAL:
                    ret = codegen_return_op(tree);
                    break;
                case semantics::FROM_TOP:
                    ret = codegen_from_op(tree);
                    break;
            }
            
            return ret;
        }
        
        // for 'null' value
        Value *llvm_node_codegen::operator()(void* const &val) const
        {
            IRBuilder<> &builder = state.module_builder();
            
            return builder.CreateIntToPtr(
                ConstantInt::get(getGlobalContext(), APInt(32, 0, true)),
                kite_type_to_llvm_type(semantics::OBJECT));
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
        
        Value *llvm_node_codegen::codegen_from_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            Value *this_obj = generate_debug_data(
                builder.CreateLoad(state.current_symbol_stack()["this"]),
                tree.position);
            Value *parent = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            generate_llvm_dynamic_object_set_parent(this_obj, parent, tree);
            
            state.push_class_from(const_cast<semantics::syntax_tree*>(
                &boost::get<semantics::syntax_tree>(tree.children[0])));
            return this_obj;
        }
        
        Value *llvm_node_codegen::codegen_iterate_op(semantics::syntax_tree const &tree) const
        {
            Value *ret = NULL;
            
            if (tree.doc_string.size() > 0)
            {
                // Doc string at the beginning of the file. Set doc for "this".
                IRBuilder<> &builder = state.module_builder();
                Value *obj = builder.CreateLoad(state.current_symbol_stack()["this"]);
                generate_llvm_dynamic_object_set_doc_string(obj, tree.doc_string, tree);
            }
            
            BOOST_FOREACH(semantics::syntax_tree_node const &node, tree.children)
            {
                ret = boost::apply_visitor(llvm_node_codegen(state), node);
                if (state.get_skip_remaining()) break;
            }
            
            return ret;
        }
        
        void llvm_node_codegen::find_used_variable_names(const semantics::syntax_tree &tree, std::map<std::string, semantics::syntax_tree> &vars) const
        {
            // Variables should all be defined at the top of the method. This function
            // extracts all VARIABLE nodes from the AST.
            if (tree.op == semantics::VARIABLE)
            {
                std::string var_name = boost::get<std::string>(tree.children[0]);
                if (vars.find(var_name) == vars.end() && var_name != "base" && var_name != "__root" && var_name != "__exc")
                {
                    vars[var_name] = tree;
                }
            }
            else if (tree.op == semantics::PROP_ASSIGN)
            {
                // property assignments should also be near the top of the method.
                // This also results in 'x' automatically pointing to this.x.
                if (vars.find(tree.prop_name) == vars.end())
                {
                    vars[tree.prop_name] = tree;
                }
            }
            
            int index = 0;
            BOOST_FOREACH(semantics::syntax_tree_node const &node, tree.children)
            {
                index++;
                if (tree.op == semantics::CLASS && (index - 1) > 0)
                {
                    // Class inheritance/contents should not appear at the beginning of the method.
                    break;
                }
                else if (tree.op == semantics::MAKE && (index - 1) == 0)
                {
                    // neither should classes referenced by make.
                    break;
                }
                else if (
                    tree.op == semantics::METHOD_REF || tree.op == semantics::CONSTRUCTOR ||
                    tree.op == semantics::DESTRUCTOR || tree.op == semantics::OPERATOR ||
                    tree.op == semantics::METHOD)
                {
                    // or methods.
                    break;
                }
                
                if ( const semantics::syntax_tree *childTree = boost::get<semantics::syntax_tree>(&node) )
                {
                    find_used_variable_names(*childTree, vars);
                }
            }
        }
        
        Value *llvm_node_codegen::codegen_const_op(semantics::syntax_tree const &tree) const
        {
            return boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
        }
        
        Value *llvm_node_codegen::codegen_import_op(semantics::syntax_tree const &tree) const
        {
            std::string module_name = boost::get<std::string>(tree.children[0]);
            
            language::kite::kite::ImportModule(module_name);
            
            return ConstantInt::get(getGlobalContext(), APInt(32, 0, true));
        }
        
        Value *llvm_node_codegen::codegen_binary_op(semantics::syntax_tree const &tree) const
        {
            Value *lhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            Value *rhs;
            Value *ret = NULL;
            Type *lhs_type = lhs->getType();
            Type *rhs_type;
            
            IRBuilder<> &builder = state.module_builder();
            BasicBlock *currentBB = builder.GetInsertBlock();
            Function *currentFunc = currentBB->getParent();
            Module *module = state.current_module();
            
            if 
                ((tree.op == semantics::AND || tree.op == semantics::OR) &&
                 get_type(lhs) == semantics::BOOLEAN)
            {
                // Short circuiting special handling.
                BasicBlock *do_other = BasicBlock::Create(getGlobalContext(), "do_other", currentFunc);
                BasicBlock *collect_result = BasicBlock::Create(getGlobalContext(), "collect_result", currentFunc);
                if (tree.op == semantics::AND)
                {
                    generate_debug_data(builder.CreateCondBr(lhs, do_other, collect_result), tree.position);
                }
                else
                {
                    generate_debug_data(builder.CreateCondBr(lhs, collect_result, do_other), tree.position);
                }
                
                builder.SetInsertPoint(do_other);
                semantics::builtin_types op_type = get_type(lhs);
                IRBuilderFPtr ptr = codegen_map[CodeOperationKey(tree.op, op_type)];
                rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
                if (get_type(rhs) != semantics::BOOLEAN)
                {
                    BasicBlock *type_error_block = BasicBlock::Create(getGlobalContext(), "raise_type_error", currentFunc);
                    BasicBlock *type_error_block_end = BasicBlock::Create(getGlobalContext(), "raise_type_error_end", currentFunc);
                    if (get_type(rhs) == semantics::OBJECT)
                    {
                        std::vector<Type*> parameterTypes;
                        parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
                        FunctionType *ftPtrLookup = FunctionType::get(kite_type_to_llvm_type(semantics::BOOLEAN), ArrayRef<Type*>(parameterTypes), false);
                        Function *funPtrLookup = Function::Create(ftPtrLookup, Function::ExternalLinkage, "kite_object_is_boolean", module);
                        if (funPtrLookup->getName() != "kite_object_is_boolean")
                        {  
                            funPtrLookup->eraseFromParent();
                            funPtrLookup = module->getFunction("kite_object_is_boolean");
                        }
                        Value *ret = generate_debug_data(builder.CreateCall(funPtrLookup, rhs), tree.position);
                        generate_debug_data(builder.CreateCondBr(ret, type_error_block_end, type_error_block), tree.position);
                    }
                    else
                    {
                        generate_debug_data(builder.CreateBr(type_error_block), tree.position);
                    }
                    
                    builder.SetInsertPoint(type_error_block);
                    std::vector<Type*> parameterTypes;
                    parameterTypes.push_back(kite_type_to_llvm_type(semantics::STRING));
                    FunctionType *ftPtrLookup = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
                    Function *funPtrLookup = Function::Create(ftPtrLookup, Function::ExternalLinkage, "kite_exception_raise_type_mismatch", module);
                    if (funPtrLookup->getName() != "kite_exception_raise_type_mismatch")
                    {  
                        funPtrLookup->eraseFromParent();
                        funPtrLookup = module->getFunction("kite_exception_raise_type_mismatch");
                    }
                    generate_debug_data(builder.CreateCall(funPtrLookup, builder.CreateGlobalStringPtr("boolean expected.")), tree.position);
                    generate_debug_data(builder.CreateBr(type_error_block_end), tree.position);
                    builder.SetInsertPoint(type_error_block_end);

                    std::vector<Value*> params;
                    params.push_back(rhs);
                    rhs = generate_llvm_method_call(rhs, "bool", params, tree);
                }
                Value *other_val = (state.module_builder().*ptr)(lhs, rhs, "");
                generate_debug_data(builder.CreateBr(collect_result), tree.position);
                do_other = builder.GetInsertBlock();
                
                builder.SetInsertPoint(collect_result);
                PHINode *phi = builder.CreatePHI(lhs_type, 2);
                phi->addIncoming(other_val, do_other);
                phi->addIncoming(lhs, currentBB);
                generate_debug_data(phi, tree.position);
                ret = phi;
            }
            else 
            {
                rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
                rhs_type = rhs->getType();
                if (lhs_type == rhs_type && lhs_type != kite_type_to_llvm_type(semantics::OBJECT))
                {
                    if ((get_type(lhs) == semantics::INTEGER || get_type(lhs) == semantics::FLOAT) &&
                        (tree.op == semantics::DIVIDE || tree.op == semantics::MODULO))
                    {
                        // Divide by zero special handling.
                        Value *valAsInt = rhs, *result_eqzero, *result_neqzero;
                        if (get_type(lhs) == semantics::FLOAT) valAsInt = builder.CreateFPToSI(rhs, kite_type_to_llvm_type(semantics::INTEGER));
                    
                        BasicBlock *neq_zero = BasicBlock::Create(getGlobalContext(), "neq_zero", currentFunc);
                        BasicBlock *eq_zero = BasicBlock::Create(getGlobalContext(), "eq_zero", currentFunc);
                        BasicBlock *div_result = BasicBlock::Create(getGlobalContext(), "div_result", currentFunc);
                        Value *cond = builder.CreateICmpEQ(valAsInt, ConstantInt::get(getGlobalContext(), APInt(32, 0, true)));
                        generate_debug_data(builder.CreateCondBr(cond, eq_zero, neq_zero), tree.position);
                    
                        builder.SetInsertPoint(eq_zero);
                        std::vector<Type*> parameterTypes;
                        FunctionType *ftPtrLookup = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
                        Function *funPtrLookup = Function::Create(ftPtrLookup, Function::ExternalLinkage, "kite_exception_raise_div_by_zero", module);
                        if (funPtrLookup->getName() != "kite_exception_raise_div_by_zero")
                        {  
                            funPtrLookup->eraseFromParent();
                            funPtrLookup = module->getFunction("kite_exception_raise_div_by_zero");
                        }
                        generate_debug_data(builder.CreateCall(funPtrLookup), tree.position);
                        if (get_type(lhs) == semantics::FLOAT) result_eqzero = ConstantFP::get(getGlobalContext(), APFloat(0.0));
                        else result_eqzero = ConstantInt::get(getGlobalContext(), APInt(32, 0, true));
                        generate_debug_data(builder.CreateBr(div_result), tree.position);
                    
                        builder.SetInsertPoint(neq_zero);
                        semantics::builtin_types op_type = get_type(lhs);
                        IRBuilderFPtr ptr = codegen_map[CodeOperationKey(tree.op, op_type)];
                        if (ptr != NULL)
                        {
                            result_neqzero = (state.module_builder().*ptr)(lhs, rhs, "");
                            if (isa<UndefValue>(result_neqzero))
                            {
                                result_neqzero = NULL;
                            }
                        }
                        if (result_neqzero == NULL)
                        {
                            if (get_type(lhs) == semantics::FLOAT) result_neqzero = ConstantFP::get(getGlobalContext(), APFloat(0.0));
                            else result_neqzero = ConstantInt::get(getGlobalContext(), APInt(32, 0, true));
                        }
                        generate_debug_data(builder.CreateBr(div_result), tree.position);
                    
                        builder.SetInsertPoint(div_result);
                        ret = builder.CreatePHI(lhs_type, 2);
                        ((PHINode*)ret)->addIncoming(result_neqzero, neq_zero);
                        ((PHINode*)ret)->addIncoming(result_eqzero, eq_zero);
                        generate_debug_data((PHINode*)ret, tree.position);
                    }
                    else
                    {
                        semantics::builtin_types op_type = get_type(lhs);
                        IRBuilderFPtr ptr = codegen_map[CodeOperationKey(tree.op, op_type)];
                        if (ptr != NULL)
                        {
                            ret = (state.module_builder().*ptr)(lhs, rhs, "");
                        }
                    }
                }
            }
            
            if (ret == NULL)
            {
                // Use method call to perform operation.
                std::vector<Value*> params;
                if (lhs_type != kite_type_to_llvm_type(semantics::OBJECT))
                {
                    params.push_back(lhs);
                    lhs = generate_llvm_method_call(lhs, "obj", params, tree);
                    params.clear();
                }
                if (rhs_type != kite_type_to_llvm_type(semantics::OBJECT))
                {
                    params.push_back(rhs);
                    rhs = generate_llvm_method_call(rhs, "obj", params, tree);
                    params.clear();
                }
                params.push_back(lhs);
                params.push_back(rhs);
                std::string funcName = semantics::Constants::Get().operator_map[tree.op];
                ret = generate_llvm_method_call(lhs, funcName, params, tree);
            }

            return ret;
        }
        
        Value *llvm_node_codegen::codegen_unary_plus_op(semantics::syntax_tree const &tree) const
        {
            Value *rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            switch(get_type(rhs))
            {
                case semantics::INTEGER:
                case semantics::FLOAT:
                    return rhs;
                default:
                    std::vector<Value*> params;
                    params.push_back(rhs);
                    return generate_llvm_method_call(rhs, semantics::Constants::Get().operator_map[tree.op], params, tree);
            }
        }
        
        Value *llvm_node_codegen::codegen_method_ref_op(semantics::syntax_tree const &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            Value *this_ptr = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            std::string name = boost::get<std::string>(tree.children[1]);
            int args = boost::get<int>(tree.children[2]);
            
            if (get_type(this_ptr) != semantics::OBJECT)
            {
                std::vector<Value*> params;
                params.push_back(this_ptr);
                this_ptr = generate_llvm_method_call(this_ptr, "obj", params, tree);
            }
            
            // TODO: refactor
            std::vector<Type*> parameterTypesLookup;
            std::vector<Value*> paramValuesLookup;
            parameterTypesLookup.push_back(PointerType::getUnqual(Type::getInt32Ty(getGlobalContext())));
            parameterTypesLookup.push_back(kite_type_to_llvm_type(semantics::STRING));
            parameterTypesLookup.push_back(kite_type_to_llvm_type(semantics::INTEGER));
            FunctionType *ftPtrLookup = FunctionType::get(parameterTypesLookup[0], ArrayRef<Type*>(parameterTypesLookup), false);
            Function *funPtrLookup = Function::Create(ftPtrLookup, Function::ExternalLinkage, "kite_find_funccall", module);
            if (funPtrLookup->getName() != "kite_find_funccall")
            {
                funPtrLookup->eraseFromParent();
                funPtrLookup = module->getFunction("kite_find_funccall");
            }
            paramValuesLookup.push_back(builder.CreateBitCast(this_ptr, PointerType::getUnqual(Type::getInt32Ty(getGlobalContext()))));
            paramValuesLookup.push_back(builder.CreateGlobalStringPtr(name.c_str()));
            paramValuesLookup.push_back(ConstantInt::get(getGlobalContext(), APInt(32, args + 1, true)));
            Value *fptr = generate_debug_data(builder.CreateCall(
                funPtrLookup,
                ArrayRef<Value*>(paramValuesLookup)
            ), tree.position);
            
            std::vector<Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::INTEGER));
            
            FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_method_alloc", module);
            if (funPtr->getName() != "kite_method_alloc")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_method_alloc");
            }
            Value *method = generate_debug_data(
                builder.CreateCall2(funPtr, builder.CreateBitCast(fptr, kite_type_to_llvm_type(semantics::OBJECT)), ConstantInt::get(kite_type_to_llvm_type(semantics::INTEGER), args + 0)),
                tree.position);
            generate_debug_data(
                builder.CreateStore(this_ptr, builder.CreateStructGEP(builder.CreateBitCast(method, get_method_type()), 5)),
                tree.position);
            
            return method;
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
            else if (op_type == semantics::FLOAT)
            {
                lhs = ConstantFP::get(getGlobalContext(), APFloat(0.0));
                lhs = state.module_builder().CreateFSub(lhs, rhs);
            }
            else
            {
                std::vector<Value*> params;
                params.push_back(rhs);
                return generate_llvm_method_call(rhs, semantics::Constants::Get().operator_map[tree.op], params, tree);
            }
            
            return lhs;
        }
        
        Value *llvm_node_codegen::codegen_not_op(semantics::syntax_tree const &tree) const
        {
            Value *rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            switch(get_type(rhs))
            {
                case semantics::INTEGER:
                case semantics::BOOLEAN:
                    return state.module_builder().CreateNot(rhs);
                default:
                    std::vector<Value*> params;
                    params.push_back(rhs);
                    return generate_llvm_method_call(rhs, semantics::Constants::Get().operator_map[tree.op], params, tree);
            }
        }
        
        Value *llvm_node_codegen::codegen_isof_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            Module *module = state.current_module();
            Value *lhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            Value *rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
            Value *type = NULL;
            
            const Type *lhsType = lhs->getType();
            if (lhsType == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)) ||
                (lhsType != kite_type_to_llvm_type(semantics::OBJECT) && isa<PointerType>(lhsType)))
            {
                lhs = generate_debug_data(builder.CreateLoad(lhs), tree.position);
            }
            const Type *rhsType = rhs->getType();
            if (rhsType == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)) ||
                (rhsType != kite_type_to_llvm_type(semantics::OBJECT) && isa<PointerType>(rhsType)))
            {
                rhs = generate_debug_data(builder.CreateLoad(rhs), tree.position);
            }
            
            if (tree.op == semantics::IS_CLASS)
            {
                type = ConstantInt::get(getGlobalContext(), APInt(1, 0, true));
            }
            else
            {
                type = ConstantInt::get(getGlobalContext(), APInt(1, 1, true));
            }
            
            std::vector<Type*> parameterTypes;
            std::vector<Value*> paramValues;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::BOOLEAN));
            FunctionType *ftPtrLookup = FunctionType::get(parameterTypes[2], ArrayRef<Type*>(parameterTypes), false);
            Function *funPtrLookup = Function::Create(ftPtrLookup, Function::ExternalLinkage, "kite_object_isof", module);
            if (funPtrLookup->getName() != "kite_object_isof")
            {  
                funPtrLookup->eraseFromParent();
                funPtrLookup = module->getFunction("kite_object_isof");
            }
            
            std::vector<Value*> params;
            if (lhs->getType() != kite_type_to_llvm_type(semantics::OBJECT))
            {
                params.push_back(lhs);
                lhs = generate_llvm_method_call(lhs, "obj", params, tree);
            }
            paramValues.push_back(lhs);
            if (rhs->getType() != kite_type_to_llvm_type(semantics::OBJECT))
            {
                params.clear();
                params.push_back(rhs);
                rhs = generate_llvm_method_call(rhs, "obj", params, tree);
            }
            paramValues.push_back(rhs);
            paramValues.push_back(type);
            return generate_debug_data(
                builder.CreateCall(
                    funPtrLookup,
                    ArrayRef<Value*>(paramValues)
                ), tree.position);
        }

        Value *llvm_node_codegen::codegen_map_op(semantics::syntax_tree const &tree) const
        {
            return codegen_mapreduce_op(tree);
        }
        
        Value *llvm_node_codegen::codegen_reduce_op(semantics::syntax_tree const &tree) const
        {
            return codegen_mapreduce_op(tree);
        }
        
        Value *llvm_node_codegen::codegen_mapreduce_op(semantics::syntax_tree const &tree) const
        {
            std::vector<Value*> parameters;
            
            Value *list_val = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            Value *method_val = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
            parameters.push_back(list_val);
            parameters.push_back(method_val);
            
            return generate_llvm_method_call(list_val, semantics::Constants::Get().operator_map[tree.op], parameters, tree);
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
            
            state.overrideOverloadedProperties(false);
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
            std::string property_name = boost::get<std::string>(tree.children[0]);
            return generate_llvm_dynamic_object_get_property(prev, property_name, tree);
        }
        
        Value *llvm_node_codegen::codegen_deref_method_op(semantics::syntax_tree const &tree, Value *prev) const
        {
            std::string method_name = boost::get<std::string>(tree.children[0]);
            std::vector<Value*> parameters;
            std::map<std::string, Value*> &sym_stack = state.current_symbol_stack();
            IRBuilder<> &builder = state.module_builder();
            
            parameters.push_back(prev);
            bool overrideOverloaded = state.overrideOverloadedProperties();
            for (int i = 1; i < tree.children.size(); i++)
            {
                Value *param_val = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                if (param_val->getType() == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
                {
                    param_val = generate_debug_data(builder.CreateLoad(param_val), tree.position);
                }
                parameters.push_back(param_val);
            }
            
            if (overrideOverloaded)
            {
                state.overrideOverloadedProperties(false);
                parameters[0] = generate_debug_data(builder.CreateLoad(sym_stack["this"]), tree.position);
            }
            return generate_llvm_method_call(prev, method_name, parameters, tree);
        }
        
        Value *llvm_node_codegen::codegen_deref_method_relative_self_op(semantics::syntax_tree const &tree) const
        {
            std::string method_name = boost::get<std::string>(tree.children[0]);
            std::vector<Value*> parameters;
            std::vector<Type*> parameterTypes;
            std::map<std::string, Value*> &sym_stack = state.current_symbol_stack();
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();

            Value *fptr = NULL;

            Value *self = generate_debug_data(state.module_builder().CreateLoad(sym_stack["this"]), tree.position);
            parameters.push_back(self);
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            for (int i = 1; i < tree.children.size(); i++)
            {
                Value *param_val = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                parameters.push_back(param_val);
                parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            }
            FunctionType *ft = FunctionType::get(parameterTypes[0], ArrayRef<Type*>(parameterTypes), false);

            if (sym_stack.find(method_name) != sym_stack.end())
            {
                // local variable
                // TODO: verify that variable is System::object* first.
                Value *method_obj = builder.CreateBitCast(
                    generate_debug_data(
                        builder.CreateLoad(sym_stack[method_name]), tree.position),
                    get_method_type());
                parameters[0] = generate_debug_data(
                    builder.CreateLoad(builder.CreateStructGEP(method_obj, 5)),
                    tree.position);
                
                std::vector<Type*> parameterTypesLookup;
                std::vector<Value*> paramValuesLookup;
                parameterTypesLookup.push_back(PointerType::getUnqual(Type::getInt32Ty(getGlobalContext())));
                parameterTypesLookup.push_back(kite_type_to_llvm_type(semantics::INTEGER));
                FunctionType *ftPtrLookup = FunctionType::get(parameterTypesLookup[0], ArrayRef<Type*>(parameterTypesLookup), false);
                Function *funPtrLookup = Function::Create(ftPtrLookup, Function::ExternalLinkage, "kite_method_verify_semantics", module);
                if (funPtrLookup->getName() != "kite_method_verify_semantics")
                {  
                    funPtrLookup->eraseFromParent();
                    funPtrLookup = module->getFunction("kite_method_verify_semantics");
                }
                paramValuesLookup.push_back(
                    generate_debug_data(
                        builder.CreateLoad(sym_stack[method_name]),
                        tree.position));
                paramValuesLookup.push_back(ConstantInt::get(getGlobalContext(), APInt(32, parameters.size(), true)));
                fptr = builder.CreateBitCast(
                    generate_debug_data(
                        builder.CreateCall(
                            funPtrLookup,
                            ArrayRef<Value*>(paramValuesLookup)
                        ),
                        tree.position),
                    PointerType::getUnqual(ft));
            }

            if (fptr != NULL)
            {
                for (int i = 0; i < parameters.size(); i++)
                {
                    if (get_type(parameters[i]) != semantics::OBJECT)
                    {
                        std::vector<Value*> params;
                        params.push_back(parameters[i]);
                        parameters[i] = generate_llvm_method_call(parameters[i], "obj", params, tree);
                    }
                }
                return generate_debug_data(
                    builder.CreateCall(
                        fptr,
                        ArrayRef<Value*>(parameters)
                    ), tree.position);
            }
            else
            {
                return generate_llvm_method_call(self, method_name, parameters, tree);
            }
        }

        Value *llvm_node_codegen::codegen_deref_array_op(semantics::syntax_tree const &tree, Value *prev) const
        {
            std::vector<Value*> parameters;
            
            parameters.push_back(prev);
            Value *index_val = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            parameters.push_back(index_val);
            
            return generate_llvm_method_call(prev, semantics::Constants::Get().operator_map[tree.op], parameters, tree);
        }
        
        Value *llvm_node_codegen::codegen_variable_op(semantics::syntax_tree const &tree) const
        {
            std::string var_name = boost::get<std::string>(tree.children[0]);
            std::map<std::string, Value*> &sym_stack = state.current_symbol_stack();
            IRBuilder<> &builder = state.module_builder();
            BasicBlock *currentBB = builder.GetInsertBlock();
            Function *currentFunc = currentBB->getParent();
            
            if (var_name == "__root")
            {
                return generate_llvm_dynamic_object_get_root(tree);
            }
            else if (var_name == "__exc" && sym_stack.find("__exc") != sym_stack.end())
            {
                return sym_stack["__exc"];
            }
            else if (var_name == "base")
            {
                semantics::syntax_tree *baseTree = state.class_from();
                Value *this_obj = NULL;
                if (baseTree != NULL)
                {
                    this_obj = (*this)(*baseTree);
                }
                else
                {
                    this_obj = (*this)((void*)NULL);
                }
                state.overrideOverloadedProperties(true);
                return this_obj;
            }
            else
            {
                if (sym_stack.count(var_name) == 0)
                {
                    BasicBlock *sym_block = BasicBlock::Create(getGlobalContext(), "varcreate", currentFunc);
                    generate_debug_data(builder.CreateBr(sym_block), tree.position);
                    
                    builder.SetInsertPoint(sym_block);
                    // TODO: do not autocreate var in this/__root if it doesn't exist.
                    // The method level version will be used until the end of this method,
                    // and then other methods in the class may end up using the one in
                    // the instance instead of another method-level var.
                    Value *getProp = generate_llvm_dynamic_object_get_property(sym_stack["this"], var_name, tree);
                    Value *propValue = generate_debug_data(builder.CreateLoad(getProp), tree.position);
                    
                    BasicBlock *has_var = BasicBlock::Create(getGlobalContext(), "varnotexists", currentFunc);
                    BasicBlock *end_var = BasicBlock::Create(getGlobalContext(), "varend", currentFunc);
                    Value *zeroInt = ConstantInt::get(getGlobalContext(), APInt(sizeof(void*) * 8, 0, true));
                    Value *zeroPtr = builder.CreateIntToPtr(zeroInt, propValue->getType());
                    Value *cmpValue = builder.CreateICmpEQ(propValue, zeroPtr);
                    generate_debug_data(builder.CreateCondBr(cmpValue, has_var, end_var), tree.position);
                    
                    builder.SetInsertPoint(has_var);
                    // TODO: check __root too.
                    Value *createVar = builder.CreateAlloca(kite_type_to_llvm_type(semantics::OBJECT));
                    generate_debug_data(builder.CreateStore(zeroPtr, createVar), tree.position);
                    generate_debug_data(builder.CreateBr(end_var), tree.position);
                    
                    builder.SetInsertPoint(end_var);
                    PHINode *phi = builder.CreatePHI(getProp->getType(), 2);
                    assert(getProp->getType() == createVar->getType());
                    phi->addIncoming(getProp, sym_block);
                    phi->addIncoming(createVar, has_var);
                    sym_stack[var_name] = phi;
                    generate_debug_data(phi, tree.position);
                }
                return generate_debug_data(builder.CreateLoad(sym_stack[var_name]), tree.position);
            }
        }
        
        Value *llvm_node_codegen::codegen_assign_op(semantics::syntax_tree const &tree) const
        {
            Value *rhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
            
            if (tree.is_global || tree.op != semantics::PROP_ASSIGN)
            {
                Value *lhs = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
                std::map<std::string, Value*> &sym_stack = state.current_symbol_stack();
            
                Value *ptr = ((LoadInst*)lhs)->getPointerOperand();
                for (std::map<std::string, Value*>::iterator i = sym_stack.begin(); i != sym_stack.end(); i++)
                {
                    if (i->second == ptr)
                    {
                        if (lhs->getType() != rhs->getType() && 
                            lhs->getType() != kite_type_to_llvm_type(semantics::OBJECT))
                        {
                            sym_stack[i->first] = 
                                state.module_builder().CreateAlloca(rhs->getType());
                            ptr = sym_stack[i->first];
                        }
                        else if (
                            lhs->getType() != rhs->getType() && 
                            lhs->getType() == kite_type_to_llvm_type(semantics::OBJECT))
                        {
                            std::vector<Value*> params;
                            params.push_back(rhs);
                            rhs = generate_llvm_method_call(rhs, "obj", params, tree);
                        }
                        generate_debug_data(state.module_builder().CreateStore(rhs, ptr), tree.position);
                        return rhs;
                    }
                }
            
                // Else, just a simple store into a property.
                // TODO
                if (rhs->getType() != kite_type_to_llvm_type(semantics::OBJECT))
                {
                    std::vector<Value*> params;
                    params.push_back(rhs);
                    rhs = generate_llvm_method_call(rhs, "obj", params, tree);
                }
                generate_debug_data(state.module_builder().CreateStore(rhs, lhs), tree.position);
                
                if (tree.op == semantics::PROP_ASSIGN && tree.is_global)
                {
                    std::map<std::string, Value*> &symStack = state.current_symbol_stack();
                    if (symStack.find(tree.prop_name) == symStack.end())
                    {
                        symStack[tree.prop_name] = lhs;
                    }
                }
            }
            
            // Set documentation string for property if needed.
            if (tree.doc_string.size() > 0)
            {
                Value *this_obj = generate_debug_data(
                    state.module_builder().CreateLoad(state.current_symbol_stack()["this"]),
                    tree.position);
                generate_llvm_dynamic_object_set_doc_string_prop(this_obj, tree.prop_name, tree.doc_string, tree);
            }
            
            return rhs;
        }
        
        Value *llvm_node_codegen::codegen_loop_op(semantics::syntax_tree const &tree) const
        {
            BasicBlock *currentBB = state.module_builder().GetInsertBlock();
            Function *currentFunc = currentBB->getParent();
            BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "loop", currentFunc);
            
            generate_debug_data(state.module_builder().CreateBr(BB), tree.position);
            state.module_builder().SetInsertPoint(BB);
            Value *condition = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            if (get_type(condition) != semantics::BOOLEAN)
            {
                std::vector<Value*> params;
                params.push_back(condition);
                condition = generate_llvm_method_call(condition, "bool", params, tree);
            }
            
            BasicBlock *bodyBB = BasicBlock::Create(getGlobalContext(), "loopbody", currentFunc);
            BasicBlock *afterLoopBB = BasicBlock::Create(getGlobalContext(), "loopend", currentFunc);
            
            state.push_loop(BB);
            state.push_loop_end(afterLoopBB);
            
            switch(tree.op)
            {
                case semantics::WHILE:
                    generate_debug_data(state.module_builder().CreateCondBr(condition, bodyBB, afterLoopBB), tree.position);
                    break;
                case semantics::UNTIL:
                    generate_debug_data(state.module_builder().CreateCondBr(condition, afterLoopBB, bodyBB), tree.position);
                    break;
            }
            
            state.module_builder().SetInsertPoint(bodyBB);
            Value *inner = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
            if (!state.get_skip_remaining()) generate_debug_data(state.module_builder().CreateBr(BB), tree.position);
            state.skip_remaining(false);
            
            state.module_builder().SetInsertPoint(afterLoopBB);
            
            state.pop_loop();
            state.pop_loop_end();
            
            return ConstantInt::get(getGlobalContext(), APInt(32, 0, true)); // TODO
        }
        
        Value *llvm_node_codegen::codegen_break_continue_op(semantics::syntax_tree const &tree) const
        {
            BasicBlock *currentLoop = NULL;
            IRBuilder<> &builder = state.module_builder();
            
            // TODO: verify that we're inside of a loop.
            switch(tree.op)
            {
                case semantics::CONTINUE:
                    currentLoop = state.current_loop();
                    break;
                case semantics::BREAK:
                    currentLoop = state.current_loop_end();
                    break;
            }
            
            generate_debug_data(builder.CreateBr(currentLoop), tree.position);
            state.skip_remaining(true);
            return ConstantInt::get(getGlobalContext(), APInt(32, 0, true)); // TODO
        }
        
        Value *llvm_node_codegen::codegen_decide_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            BasicBlock *currentBB = builder.GetInsertBlock();
            Function *currentFunc = currentBB->getParent();
            BasicBlock *endBB = BasicBlock::Create(getGlobalContext(), "decide_end", currentFunc);
            Value *condition = NULL;
            BasicBlock *condBB = BasicBlock::Create(getGlobalContext(), "decide_cond", currentFunc, endBB);
            BasicBlock *actionBB = NULL;
            PHINode *PN = NULL;
            std::vector<Value*> decideResults;
            std::vector<BasicBlock*> decideBlocks;
            
            generate_debug_data(builder.CreateBr(condBB), tree.position);
            builder.SetInsertPoint(condBB);
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
                    Value *zero = ConstantInt::get(getGlobalContext(), APInt(sizeof(void*) << 3, (uint64_t)0, true));
                    zero = builder.CreateIntToPtr(zero, kite_type_to_llvm_type(semantics::OBJECT));
                    decideResults.push_back(zero);
                    decideBlocks.push_back(builder.GetInsertBlock());
                    condBB = endBB;
                }
                
                if (get_type(condition) != semantics::BOOLEAN)
                {
                    std::vector<Value*> emptyList;
                    emptyList.push_back(condition);
                    condition = generate_llvm_method_call(condition, "bool", emptyList, tree);
                }
                
                generate_debug_data(builder.CreateCondBr(condition, actionBB, condBB), tree.position);
                builder.SetInsertPoint(actionBB);
                Value *result = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                if (!state.get_skip_remaining())
                {
                    if (get_type(result) != semantics::OBJECT)
                    {
                        std::vector<Value*> emptyList;
                        emptyList.push_back(result);
                        result = generate_llvm_method_call(result, "obj", emptyList, tree);
                    }
                    decideResults.push_back(result);
                    decideBlocks.push_back(builder.GetInsertBlock());
                }
                actionBB = builder.GetInsertBlock();
                if (!state.get_skip_remaining()) generate_debug_data(builder.CreateBr(endBB), tree.position);
                state.skip_remaining(false);
                builder.SetInsertPoint(condBB);
            }
            
            builder.SetInsertPoint(endBB);
            PN = builder.CreatePHI(kite_type_to_llvm_type(semantics::OBJECT), decideResults.size());
            for (int index = 0; index < decideResults.size(); index++)
            {
                Value *val = decideResults[index];
                BasicBlock *bb = decideBlocks[index];
                PN->addIncoming(val, bb);
            }
            generate_debug_data(PN, tree.position);
            return PN;
            //return ConstantInt::get(getGlobalContext(), APInt(32, 0, true)); // TODO
        }
        
        Value *llvm_node_codegen::codegen_run_catch_op(semantics::syntax_tree const &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            BasicBlock *currentBB = builder.GetInsertBlock();
            Function *currentFunc = currentBB->getParent();
            BasicBlock *run_block = BasicBlock::Create(getGlobalContext(), "run_block", currentFunc);
            BasicBlock *catch_block = BasicBlock::Create(getGlobalContext(), "catch_block", currentFunc);
            BasicBlock *end_block = BasicBlock::Create(getGlobalContext(), "end_block", currentFunc);
            
            // Create jump point
            //Value *default_val = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0); // TODO
            
            Value *jmpbuf = 
                builder.CreateAlloca(
                    Type::getInt8Ty(getGlobalContext()), 
                    ConstantInt::get(Type::getInt32Ty(getGlobalContext()), sizeof(jmp_buf)));
            std::vector<Type*> setjmp_params;
            setjmp_params.push_back(jmpbuf->getType());
            FunctionType *setjmp_type = FunctionType::get(Type::getInt32Ty(getGlobalContext()), ArrayRef<Type*>(setjmp_params), false);
            Function *setjmp_fun = Function::Create(setjmp_type, Function::ExternalLinkage, "setjmp", module);
            if (setjmp_fun->getName() != "setjmp")
            {
                setjmp_fun->eraseFromParent();
                setjmp_fun = module->getFunction("setjmp");
            }
            Value *setjmp_ret = generate_debug_data(builder.CreateCall(setjmp_fun, jmpbuf), tree.position);
            Value *setjmp_jumped = builder.CreateICmpEQ(setjmp_ret, ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 1));
            generate_debug_data(
                builder.CreateCondBr(setjmp_jumped, catch_block, run_block),
                tree.position);
            
            // Create run block
            builder.SetInsertPoint(run_block);
            FunctionType *pushjmp_type = FunctionType::get(Type::getVoidTy(getGlobalContext()), ArrayRef<Type*>(setjmp_params), false);
            Function *pushjmp_fun = Function::Create(pushjmp_type, Function::ExternalLinkage, "kite_exception_stack_push", module);
            if (pushjmp_fun->getName() != "kite_exception_stack_push")
            {
                pushjmp_fun->eraseFromParent();
                pushjmp_fun = module->getFunction("kite_exception_stack_push");
            }
            generate_debug_data(builder.CreateCall(pushjmp_fun, jmpbuf), tree.position);
            Value *run_ret = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            std::vector<Value*> runRetList;
            runRetList.push_back(run_ret);
            run_ret = generate_llvm_method_call(run_ret, "obj", runRetList, tree);
            
            // Clear exception stack entry after successful completion of block.
            std::vector<Type*> pop_exc_params;
            FunctionType *pop_exc_type = FunctionType::get(Type::getVoidTy(getGlobalContext()), ArrayRef<Type*>(pop_exc_params), false);
            Function *pop_exc_fun = Function::Create(pop_exc_type, Function::ExternalLinkage, "kite_exception_stack_pop", module);
            if (pop_exc_fun->getName() != "kite_exception_stack_pop")
            {
                pop_exc_fun->eraseFromParent();
                pop_exc_fun = module->getFunction("kite_exception_stack_pop");
            }
            generate_debug_data(builder.CreateCall(pop_exc_fun), tree.position);
            
            // Jump to end of run/catch statement.
            generate_debug_data(builder.CreateBr(end_block), tree.position);
            run_block = builder.GetInsertBlock();
            
            // Create catch block
            builder.SetInsertPoint(catch_block);

            // Retrieve exception object and execute catch block.
            std::map<std::string, Value*> &sym_stack = state.current_symbol_stack();
            std::vector<Type*> get_exc_params;
            FunctionType *get_exc_type = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(get_exc_params), false);
            Function *get_exc_fun = Function::Create(get_exc_type, Function::ExternalLinkage, "kite_exception_get", module);
            if (get_exc_fun->getName() != "kite_exception_get")
            {
                get_exc_fun->eraseFromParent();
                get_exc_fun = module->getFunction("kite_exception_get");
            }
            sym_stack["__exc"] =
                builder.CreateAlloca(kite_type_to_llvm_type(semantics::OBJECT));
            generate_debug_data(
                builder.CreateStore(builder.CreateCall(get_exc_fun), sym_stack["__exc"]),
                tree.position);
            
            // Clear exception stack here to handle nested exceptions.
            generate_debug_data(
                builder.CreateCall(pop_exc_fun),
                tree.position);
            
            Value *catch_ret = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
            std::vector<Value*> catchRetList;
            catchRetList.push_back(catch_ret);
            catch_ret = generate_llvm_method_call(catch_ret, "obj", catchRetList, tree);
            generate_debug_data(
                builder.CreateBr(end_block),
                tree.position);
            sym_stack.erase("__exc");
            catch_block = builder.GetInsertBlock();
            
            // Create cleanup block
            builder.SetInsertPoint(end_block);
            PHINode *phi = builder.CreatePHI(run_ret->getType(), 2);
            phi->addIncoming(run_ret, run_block);
            phi->addIncoming(catch_ret, catch_block);
            //phi->addIncoming(default_val, currentBB);
            builder.SetInsertPoint(end_block);
            generate_debug_data(phi, tree.position);
            
            // Return result.
            return phi;
        }

        Value *llvm_node_codegen::codegen_constructor_op(semantics::syntax_tree const &tree) const
        {
            // TODO: refactor
            IRBuilder<> &builder = state.module_builder();
            int numargs = 1;
            std::vector<std::string> argnames;
            std::vector<std::string> argdocs;
            std::string functionName = semantics::Constants::Get().operator_map[tree.op];
            for (int i = 0; i < tree.children.size() - 1; i += 2)
            {
                argnames.push_back(boost::get<std::string>(tree.children[i]));
                argdocs.push_back(boost::get<std::string>(tree.children[i + 1]));
                numargs++;
            }

            semantics::syntax_tree &body = const_cast<semantics::syntax_tree&>(boost::get<semantics::syntax_tree>(tree.children[tree.children.size() - 1]));
            Value *method = generate_llvm_method(functionName, argnames, body, tree);
            
            Value *method_obj = generate_llvm_method_alloc(method, tree);
            Value *property = generate_debug_data(builder.CreateLoad(state.current_symbol_stack()["this"]), tree.position);
            functionName += "__";
            for (int i = 0; i < numargs; i++)
            {
               functionName += "o";
            }
            Value *prop_entry = generate_llvm_dynamic_object_get_property(property, functionName, tree, true);
            generate_debug_data(builder.CreateStore(method_obj, prop_entry), tree.position);
            
            state.current_symbol_stack()[functionName] = prop_entry;
            generate_llvm_dynamic_object_set_doc_string(method_obj, tree.doc_string, tree);
            return method_obj;
        }

        Value *llvm_node_codegen::codegen_list_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            Module *module = state.current_module();
            
            std::vector<Type*> parameterTypesNewList;
            std::vector<Value*> paramValuesNewList;
            FunctionType *ftPtrNewList = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypesNewList), false);
            Function *funPtrNewList = Function::Create(ftPtrNewList, Function::ExternalLinkage, "kite_list_new", module);
            if (funPtrNewList->getName() != "kite_list_new")
            {
                funPtrNewList->eraseFromParent();
                funPtrNewList = module->getFunction("kite_list_new");
            }
            Value *listObject = generate_debug_data(builder.CreateCall(funPtrNewList), tree.position);
            
            parameterTypesNewList.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypesNewList.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            FunctionType *ftPtrAppendList = FunctionType::get(parameterTypesNewList[0], ArrayRef<Type*>(parameterTypesNewList), false);
            Function *funPtrAppendList = Function::Create(ftPtrAppendList, Function::ExternalLinkage, "kite_list_append", module);
            if (funPtrAppendList->getName() != "kite_list_append")
            {
                funPtrAppendList->eraseFromParent();
                funPtrAppendList = module->getFunction("kite_list_append");
            }
            
            paramValuesNewList.push_back(listObject);
            paramValuesNewList.push_back(NULL);
            for (int i = 0; i < tree.children.size(); i++)
            {
                Value *listItem = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                const Type *itemType = listItem->getType();
                if (itemType == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
                {
                    listItem = generate_debug_data(builder.CreateLoad(listItem), tree.position);
                }
                else if (itemType != kite_type_to_llvm_type(semantics::OBJECT))
                {
                    std::vector<Value*> params;
                    params.push_back(listItem);
                    listItem = generate_llvm_method_call(listItem, "obj", params, tree);
                }
                paramValuesNewList[1] = listItem;
                generate_debug_data(
                    builder.CreateCall(funPtrAppendList, ArrayRef<Value*>(paramValuesNewList)),
                    tree.position);
            }

            return listObject;
        }

        Value *llvm_node_codegen::codegen_destructor_op(semantics::syntax_tree const &tree) const
        {
            // TODO: refactor
            IRBuilder<> &builder = state.module_builder();
            std::vector<std::string> argnames;
            std::string functionName = semantics::Constants::Get().operator_map[tree.op];
        
            semantics::syntax_tree &body = const_cast<semantics::syntax_tree&>(boost::get<semantics::syntax_tree>(tree.children[0]));
            Value *method = generate_llvm_method(functionName, argnames, body, tree);
            
            Value *method_obj = generate_llvm_method_alloc(method, tree);
            Value *property = generate_debug_data(
                builder.CreateLoad(state.current_symbol_stack()["this"]),
                tree.position);
            functionName += "__o";
            Value *prop_entry = generate_llvm_dynamic_object_get_property(property, functionName, tree);
            generate_debug_data(
                builder.CreateStore(method_obj, prop_entry),
                tree.position);
            
            state.current_symbol_stack()[functionName] = prop_entry;
            generate_llvm_dynamic_object_set_doc_string(method_obj, tree.doc_string, tree);
            generate_llvm_dynamic_object_enable_finalizer(method, tree);
            return method_obj;
        }
        
        Value *llvm_node_codegen::codegen_eval_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            std::map<std::string, Value*> &symbolTable = state.current_symbol_stack();
            
            // Cast all items on symbol table into System.object instances.
            // This is necessary in order to be able to modify variables
            // while inside the anonymous method. We can also begin specifying
            // the argument list for both the code generator and the actual
            // function call.
            std::vector<Type*> argNamesTypes;
            std::vector<Value*> argNames;
            std::vector<Value*> argValues;
            std::vector<Type*> argValuesTypes;
            
            Value *numArgs = ConstantInt::get(getGlobalContext(), APInt(32, symbolTable.size(), true));
            argNames.push_back(numArgs);
            argNamesTypes.push_back(numArgs->getType());
            
            for (
                std::map<std::string, Value*>::iterator i = symbolTable.begin();
                i != symbolTable.end();
                i++
            )
            {
                Value *val = i->second;
                argNames.push_back(builder.CreateGlobalStringPtr(i->first.c_str()));
                
                if (val->getType() != PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
                {
                    std::vector<Value*> params;
                    params.push_back(val);
                    val = generate_llvm_method_call(val, "obj", params, tree);
                    
                    symbolTable[i->first] = 
                            state.module_builder().CreateAlloca(val->getType());
                    generate_debug_data(state.module_builder().CreateStore(val, symbolTable[i->first]), tree.position);
                }
                
                argValues.push_back(i->second);
                argValuesTypes.push_back(i->second->getType());
            }
            
            // Retrieve string representation of code to execute, as an object.
            Value *ret = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            if (get_type(ret) != semantics::STRING)
            {
                std::vector<Value*> params;
                params.push_back(ret);
                ret = generate_llvm_method_call(ret, "str", params, tree);
            }
            if (get_type(ret) != semantics::OBJECT)
            {
                std::vector<Value*> params;
                params.push_back(ret);
                ret = generate_llvm_method_call(ret, "obj", params, tree);
            }
            argNames.insert(argNames.begin(), ret);
            argNamesTypes.insert(argNamesTypes.begin(), ret->getType());
            
            // Call standard library helper to generate method.
            Module *module = state.current_module();
            std::vector<Value*> paramValuesNewList;
            FunctionType *ftEvalType = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(argNamesTypes), true);
            Function *funEval = Function::Create(ftEvalType, Function::ExternalLinkage, "kite_eval_code", module);
            if (funEval->getName() != "kite_eval_code")
            {
                funEval->eraseFromParent();
                funEval = module->getFunction("kite_eval_code");
            }
            Value *functionPointer = generate_debug_data(builder.CreateCall(funEval, argNames), tree.position);
            
            // Call method using the values from the symbol table.
            FunctionType *ftPtrType = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(argValuesTypes), false);
            Function *funPtr = (Function*)builder.CreateBitCast(functionPointer, PointerType::getUnqual(ftPtrType));
            return builder.CreateCall(funPtr, argValues);
        }
        
        Value *llvm_node_codegen::codegen_method_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            int numargs = 0;
            std::string functionName = boost::get<std::string>(tree.children[0]);
            std::vector<std::string> argnames;
            std::vector<std::string> argdocs;
            
            if (tree.op == semantics::OPERATOR)
            {
                functionName = semantics::Constants::Get().operator_name_map[functionName];
            }
            
            for (int i = 1; i < tree.children.size() - 1; i += 2)
            {
                argnames.push_back(boost::get<std::string>(tree.children[i]));
                argdocs.push_back(boost::get<std::string>(tree.children[i + 1]));
                numargs++;
            }
        
            semantics::syntax_tree &body = const_cast<semantics::syntax_tree&>(boost::get<semantics::syntax_tree>(tree.children[tree.children.size() - 1]));
            Value *method = generate_llvm_method(functionName, argnames, body, tree);
            
            Value *method_obj = generate_llvm_method_alloc(method, tree);
            Value *property = generate_debug_data(
                builder.CreateLoad(state.current_symbol_stack()["this"]),
                tree.position);
            
            if (functionName != "__AnonMethod")
            {
                functionName += "__";
                for (int i = 0; i <= numargs; i++)
                    functionName += "o";
                Value *prop_entry = generate_llvm_dynamic_object_get_property(property, functionName, tree, true);
                generate_debug_data(
                    builder.CreateStore(method_obj, prop_entry),
                    tree.position);
                state.current_symbol_stack()[functionName] = prop_entry;
            }
            else
            {
                Value *method_casted = builder.CreateBitCast(method_obj, get_method_type());
                Value *this_loc = builder.CreateStructGEP(method_casted, 5);
                generate_debug_data(
                    builder.CreateStore(property, this_loc),
                    tree.position);
            }
            
            generate_llvm_dynamic_object_set_doc_string(method_obj, tree.doc_string, tree);
            for (int i = 0; i < argdocs.size(); i++)
            {
                if (argdocs[i].size() > 0)
                {
                    generate_llvm_dynamic_object_set_doc_string_arg(method_obj, argnames[i], argdocs[i], tree);
                }
            }
            return method_obj;
        }

        Value *llvm_node_codegen::codegen_return_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            Value *ret = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            if (ret->getType() == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
            {
                ret = generate_debug_data(
                    builder.CreateLoad(ret),
                    tree.position);
            }
            else if (get_type(ret) != semantics::OBJECT)
            {
                std::vector<Value*> params;
                params.push_back(ret);
                ret = generate_llvm_method_call(ret, "obj", params, tree);
            }
            generate_debug_data(builder.CreateRet(ret), tree.position);
            state.skip_remaining(true);
            return ret;
        }
        
        Value *llvm_node_codegen::generate_llvm_eval_method(std::vector<std::string> &argnames, semantics::syntax_tree &body, const semantics::syntax_tree &parent) const
        {
            BasicBlock *currentBB = state.module_builder().GetInsertBlock();
            std::vector<Type*> argTypes;
            std::string functionName = state.identifier_prefix() + "__EvalBlock";
            int numargs = argnames.size();

            state.push_friendly_method_name("__EvalBlock");
            
            if (numargs > 0)
            {
                functionName += std::string(numargs, 'o');
                for (int i = 0; i < numargs; i++)
                {
                    argTypes.push_back(PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)));
                }
            }
            
            state.push_c_method_name(functionName);
            
            Module *currentModule = state.current_module();
            FunctionType *FT = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(argTypes), false);
            Function *F = Function::Create(FT, Function::ExternalLinkage, functionName.c_str(), currentModule);
            BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
            IRBuilder<> &builder = state.module_builder();
            builder.SetInsertPoint(BB);
            
            state.push_symbol_stack();
            int i = 0;
            std::map<std::string, Value*> &symStack = state.current_symbol_stack();
            for (Function::arg_iterator AI = F->arg_begin(); i < numargs; i++, ++AI)
            {
                std::string name = argnames[i];
                AI->setName(name.c_str());
                symStack[name] = AI;
            }
            
            Value *ret = llvm_node_codegen(state)(body);
            state.pop_symbol_stack();
            
            /*if (state.get_skip_remaining() == false)
            {*/
            
            if (ret != NULL)
            {
                if (ret->getType() == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
                {
                    ret = generate_debug_data(builder.CreateLoad(ret), body.position);
                }
                else if (get_type(ret) != semantics::OBJECT)
                {
                    std::vector<Value*> params;
                    params.push_back(ret);
                    ret = generate_llvm_method_call(ret, "obj", params, body);
                }
            }
            else
            {
                if (symStack.find("this") != symStack.end())
                {
                    ret = generate_debug_data(builder.CreateLoad(symStack["this"]), parent.position);
                }
                else
                {
                    ret = ConstantInt::get(getGlobalContext(), APInt(sizeof(void*) << 3, (uint64_t)0, true));
                    ret = builder.CreateIntToPtr(ret, PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)));
                }
            }
            generate_debug_data(builder.CreateRet(ret), body.position);
            //}
            state.skip_remaining(false);
            if (currentBB) state.module_builder().SetInsertPoint(currentBB);
            
            state.pop_c_method_name();
            state.pop_friendly_method_name();
            return F;
        }
        
        Value *llvm_node_codegen::generate_llvm_method(const std::string &name, std::vector<std::string> &argnames, semantics::syntax_tree &body, const semantics::syntax_tree &parent) const
        {
            BasicBlock *currentBB = state.module_builder().GetInsertBlock();
            std::vector<Type*> argTypes;
            std::string functionName = state.identifier_prefix() + name;
            int numargs = argnames.size();

            state.push_friendly_method_name(name);
            
            functionName += "__o";
            argTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            
            if (numargs > 0)
            {
                functionName += std::string(numargs, 'o');
                for (int i = 0; i < numargs; i++)
                {
                    argTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
                }
            }
            
            state.push_c_method_name(functionName);
            
            Module *currentModule = state.current_module();
            FunctionType *FT = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(argTypes), false);
            Function *F = Function::Create(FT, Function::ExternalLinkage, functionName.c_str(), currentModule);
            BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
            IRBuilder<> &builder = state.module_builder();
            builder.SetInsertPoint(BB);
            
            state.push_symbol_stack();
            int i = 0;
            std::map<std::string, Value*> &symStack = state.current_symbol_stack();
            for (Function::arg_iterator AI = F->arg_begin(); i < numargs + 1; i++, ++AI)
            {
                std::string name;
                if (i == 0)
                {
                    name = "this";
                }
                else
                {
                    name = argnames[i - 1];
                }
                AI->setName(name.c_str());
                symStack[name] =
                    builder.CreateAlloca(kite_type_to_llvm_type(semantics::OBJECT));
                generate_debug_data(
                    builder.CreateStore(AI, symStack[name]),
                    parent.position);
            }
            
            // Define all variables at the top of the method.
            std::map<std::string, semantics::syntax_tree> vars;
            find_used_variable_names(body, vars);
            for (
                std::map<std::string, semantics::syntax_tree>::iterator i = vars.begin();
                i != vars.end();
                i++)
            {
                if (symStack.find(i->first) == symStack.end())
                {
                    (*this)(i->second);
                }
            }
            
            // Generate body.
            Value *ret = llvm_node_codegen(state)(body);
            
            /*if (state.get_skip_remaining() == false)
            {*/
            
            if (ret != NULL)
            {
                if (ret->getType() == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
                {
                    ret = generate_debug_data(builder.CreateLoad(ret), body.position);
                }
                else if (get_type(ret) != semantics::OBJECT)
                {
                    std::vector<Value*> params;
                    params.push_back(ret);
                    ret = generate_llvm_method_call(ret, "obj", params, body);
                }
            }
            else
            {
                if (symStack.find("this") != symStack.end())
                {
                    ret = generate_debug_data(builder.CreateLoad(symStack["this"]), parent.position);
                }
                else
                {
                    ret = ConstantInt::get(getGlobalContext(), APInt(sizeof(void*) << 3, (uint64_t)0, true));
                    ret = builder.CreateIntToPtr(ret, PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)));
                }
            }
            state.pop_symbol_stack();
            generate_debug_data(builder.CreateRet(ret), body.position);
            //}
            state.skip_remaining(false);
            if (currentBB) state.module_builder().SetInsertPoint(currentBB);
            
            state.pop_c_method_name();
            state.pop_friendly_method_name();
            return F;
        }
        
        Value *llvm_node_codegen::generate_llvm_method_call(Value *self, const std::string &name, std::vector<Value*> &params, const semantics::syntax_tree &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            Module *module = state.current_module();
            semantics::builtin_types type = get_type(self);
            stdlib::object_method_map &method_map = get_method_map(type);
            std::string method_name = name;
            std::vector<Type*> parameterTypes;
            std::vector<Value*> paramsCopy(params);
            
            if (self->getType() == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
            {
                bool replaceParam = false;
                if (params[0] == self) replaceParam = true;
                self = generate_debug_data(
                    builder.CreateLoad(self),
                    tree.position);
                if (replaceParam) paramsCopy[0] = self;
            }
            
            method_name += std::string("__");
            for (int i = 0; i < params.size(); i++)
            {
                method_name += type_to_code(get_type(paramsCopy[i]));
                parameterTypes.push_back(kite_type_to_llvm_type(get_type(paramsCopy[i])));
            }

            function_semantics semantics;
            Type *tmpType;
            if (method_map.find(method_name.c_str()) == method_map.end())
            {
                tmpType = const_cast<Type*>(kite_type_to_llvm_type(semantics::OBJECT));
            }
            else
            {
                semantics = method_map[method_name.c_str()];
                tmpType = const_cast<Type*>(kite_type_to_llvm_type(semantics.first));
            }
            FunctionType *ft = FunctionType::get(tmpType, ArrayRef<Type*>(parameterTypes), false);
            Value *fptr;
            if ((uint64_t)semantics.second != 0)
            {
                std::string prefixedMethod = kite_type_to_function_prefix(type) + method_name;
                Function *funPtrLookup = Function::Create(ft, Function::ExternalLinkage, prefixedMethod.c_str(), module);
                if (funPtrLookup->getName() != prefixedMethod.c_str())
                {
                    funPtrLookup->eraseFromParent();
                    funPtrLookup = module->getFunction(prefixedMethod.c_str());
                }
                fptr = funPtrLookup;
                /*fptr = module->getFunction(prefixedMethod.c_str());
                if (fptr == NULL)
                {
                    Value *fptrval = ConstantInt::get(getGlobalContext(), APInt(sizeof(void*) << 3, (uint64_t)semantics.second, true));
                    fptr = builder.CreateIntToPtr(fptrval, PointerType::getUnqual(ft));
                }*/
            }
            else
            {
                int count = 0;
                do 
                {
                    fptr = module->getFunction(method_name.c_str());
                    if (fptr == NULL)
                    {
                        // Convert all parameters to object type.
                        for (int i = 0; i < paramsCopy.size(); i++)
                        {
                            Value *v = paramsCopy[i];
                            if (get_type(v) != semantics::OBJECT)
                            {
                                std::vector<Value*> castParams;
                                castParams.push_back(v);
                                paramsCopy[i] = generate_llvm_method_call(v, std::string("obj"), castParams, tree);
                                parameterTypes[i] = kite_type_to_llvm_type(semantics::OBJECT);
                            }
                        }
                        ft = FunctionType::get(tmpType, parameterTypes, false);
                        
                        method_name = name + std::string("__");
                        for (int i = 0; i < paramsCopy.size(); i++)
                        {
                            method_name += type_to_code(get_type(paramsCopy[i]));
                        }
                    }
                } while (count++ <= 1);
                
                if (fptr == NULL)
                {
                    std::vector<Type*> parameterTypesLookup;
                    std::vector<Value*> paramValuesLookup;
                    parameterTypesLookup.push_back(PointerType::getUnqual(Type::getInt32Ty(getGlobalContext())));
                    parameterTypesLookup.push_back(kite_type_to_llvm_type(semantics::STRING));
                    parameterTypesLookup.push_back(kite_type_to_llvm_type(semantics::INTEGER));
                    FunctionType *ftPtrLookup = FunctionType::get(parameterTypesLookup[0], ArrayRef<Type*>(parameterTypesLookup), false);
                    Function *funPtrLookup = Function::Create(ftPtrLookup, Function::ExternalLinkage, "kite_find_funccall", module);
                    if (funPtrLookup->getName() != "kite_find_funccall")
                    {
                        funPtrLookup->eraseFromParent();
                        funPtrLookup = module->getFunction("kite_find_funccall");
                    }
                    paramValuesLookup.push_back(builder.CreateBitCast(self, PointerType::getUnqual(Type::getInt32Ty(getGlobalContext()))));
                    paramValuesLookup.push_back(builder.CreateGlobalStringPtr(name.c_str()));
                    paramValuesLookup.push_back(ConstantInt::get(getGlobalContext(), APInt(32, paramsCopy.size(), true)));
                    fptr = builder.CreateBitCast(
                        generate_debug_data(
                            builder.CreateCall(
                                funPtrLookup,
                                ArrayRef<Value*>(paramValuesLookup)
                            ),
                            tree.position),
                        PointerType::getUnqual(ft)
                    );
                    
                    if (name == "__init__" && paramsCopy.size() == 1)
                    {
                        Value *validatePtr = builder.CreateIsNotNull(fptr);
                        
                        BasicBlock *currentBB = builder.GetInsertBlock();
                        Function *currentFunc = currentBB->getParent();
                        BasicBlock *not_null = BasicBlock::Create(getGlobalContext(), "not_null", currentFunc);
                        BasicBlock *end_block = BasicBlock::Create(getGlobalContext(), "end_block", currentFunc);
                        generate_debug_data(
                            builder.CreateCondBr(validatePtr, not_null, end_block),
                            tree.position);
                        
                        builder.SetInsertPoint(not_null);
                        generate_debug_data(
                            builder.CreateCall(
                                fptr,
                                ArrayRef<Value*>(paramsCopy)
                            ),
                            tree.position);
                        generate_debug_data(
                            builder.CreateBr(end_block),
                            tree.position);
                        
                        builder.SetInsertPoint(end_block);
                        return self;
                    }
                }
            
            }
            self = generate_debug_data(
                builder.CreateCall(
                    fptr,
                    ArrayRef<Value*>(paramsCopy)
                ),
                tree.position);
            return self;
        }
        
        Value *llvm_node_codegen::codegen_class_op(semantics::syntax_tree const &tree) const
        {
            Value *ret;
            IRBuilder<> &builder = state.module_builder();
            BasicBlock *currentBlock = builder.GetInsertBlock();
            std::string className = boost::get<std::string>(tree.children[0]);
            
            semantics::syntax_tree &body = const_cast<semantics::syntax_tree&>(boost::get<semantics::syntax_tree>(tree.children[tree.children.size() - 1]));
            std::vector<std::string> args;
            
            // Initialize dynamic_object that will store the class and insert
            // LLVM code to call __static_init__ on this object.
            Value *obj = generate_llvm_dynamic_object_alloc(NULL, tree);
            if (tree.children.size() > 2)
            {
                // Set child class parent.
                Value *parent = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
                generate_llvm_dynamic_object_set_parent(obj, parent, tree);

                semantics::syntax_tree &base_tree = const_cast<semantics::syntax_tree&>(
                    boost::get<semantics::syntax_tree>(tree.children[1]));
                state.push_class_from(&base_tree);
            }
            else
            {
                state.push_class_from(NULL);
            }
            
            // Store object in parent so it's accessible during child(ren)'s static init.
            Value *property = generate_debug_data(
                builder.CreateLoad(state.current_symbol_stack()["this"]),
                tree.position);
            Value *prop_entry = generate_llvm_dynamic_object_get_property(property, className, tree);
            generate_debug_data(
                builder.CreateStore(obj, prop_entry),
                tree.position);
            
            state.push_namespace_stack(className);
            ret = generate_llvm_method(std::string("__static_init__"), args, body, tree);
            
            generate_debug_data(
                builder.CreateCall(ret, obj),
                tree.position);
            
            generate_llvm_dynamic_object_set_name(obj, tree);
            state.pop_namespace_stack();
            state.pop_class_from();
            
            state.current_symbol_stack()[className] = prop_entry;
            
            generate_llvm_dynamic_object_set_doc_string(obj, tree.doc_string, tree);
            return obj;
        }

        Value *llvm_node_codegen::codegen_make_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            std::vector<Value*> params;
            Value *parent = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            
            Value *obj = generate_llvm_dynamic_object_alloc(parent, tree);
            generate_llvm_dynamic_object_set_parent(obj, parent, tree);
            
            params.push_back(obj);
            for(int i = 1; i < tree.children.size(); i++)
            {
                Value *v = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                
                if (v->getType() == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
                {
                    v = generate_debug_data(
                        builder.CreateLoad(v),
                        tree.position);
                }
                params.push_back(v);
            }
            
            generate_llvm_method_call(obj, semantics::Constants::Get().operator_map[semantics::CONSTRUCTOR], params, tree);
            return obj;
        }
        
        Value *llvm_node_codegen::generate_llvm_dynamic_object_alloc(Value *orig, const semantics::syntax_tree &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            Value *alloc_method;
            
            std::vector<Type*> parameterTypes;
            FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
            
            if (orig == NULL)
            {
                Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_dynamic_object_alloc", module);
                if (funPtr->getName() != "kite_dynamic_object_alloc")
                {
                    funPtr->eraseFromParent();
                    funPtr = module->getFunction("kite_dynamic_object_alloc");
                }
                alloc_method = funPtr;
            }
            else
            {
                if (orig->getType() == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
                {
                    orig = generate_debug_data(
                        builder.CreateLoad(orig),
                        tree.position);
                }
                orig = builder.CreateBitCast(orig, get_object_type()); // TODO
                alloc_method = builder.CreateStructGEP(orig, 2);
                alloc_method = builder.CreateBitCast(
                    generate_debug_data(
                        builder.CreateLoad(alloc_method),
                        tree.position),
                    PointerType::getUnqual(ft));
            }
            
            Value *obj = generate_debug_data(
                builder.CreateCall(alloc_method),
                tree.position);
            return obj;
        }
        
        void llvm_node_codegen::generate_llvm_dynamic_object_set_parent(Value *obj, Value *parent, const semantics::syntax_tree &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            
            std::vector<Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            
            FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_dynamic_object_set_parent", module);
            if (funPtr->getName() != "kite_dynamic_object_set_parent")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_dynamic_object_set_parent");
            }
            
            if (parent->getType() == PointerType::getUnqual(parameterTypes[0]))
            {
                parent = generate_debug_data(
                    builder.CreateLoad(parent),
                    tree.position);
            }
            
            generate_debug_data(
                builder.CreateCall2(funPtr, obj, parent),
                tree.position);
        }
        
        void llvm_node_codegen::generate_llvm_dynamic_object_set_name(Value *obj, const semantics::syntax_tree &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            
            std::vector<Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::STRING));
            
            FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_dynamic_object_set_name", module);
            if (funPtr->getName() != "kite_dynamic_object_set_name")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_dynamic_object_set_name");
            }
            
            // Form full class name.
            std::string fullName = state.full_class_name();
            generate_debug_data(
                builder.CreateCall2(funPtr, obj, builder.CreateGlobalStringPtr(fullName.c_str())),
                tree.position);
        }
        
        void llvm_node_codegen::generate_llvm_dynamic_object_set_doc_string(Value *obj, const std::string &doc, const semantics::syntax_tree &tree) const
        {
            if (doc.size() > 0)
            {
                Module *module = state.current_module();
                IRBuilder<> &builder = state.module_builder();
            
                std::vector<Type*> parameterTypes;
                parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
                parameterTypes.push_back(kite_type_to_llvm_type(semantics::STRING));
            
                FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
                Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_set_docstring", module);
                if (funPtr->getName() != "kite_set_docstring")
                {
                    funPtr->eraseFromParent();
                    funPtr = module->getFunction("kite_set_docstring");
                }
            
                generate_debug_data(
                    builder.CreateCall2(funPtr, obj, builder.CreateGlobalStringPtr(doc.c_str())),
                    tree.position);
            }
        }
        
        void llvm_node_codegen::generate_llvm_dynamic_object_set_doc_string_arg(Value *obj, const std::string &name, const std::string &doc, const semantics::syntax_tree &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
        
            std::vector<Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::STRING));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::STRING));
        
            FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_set_docstring_arg", module);
            if (funPtr->getName() != "kite_set_docstring_arg")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_set_docstring_arg");
            }
        
            generate_debug_data(
                builder.CreateCall3(
                    funPtr,
                    obj, 
                    builder.CreateGlobalStringPtr(name.c_str()),
                    builder.CreateGlobalStringPtr(doc.c_str())),
                tree.position);
        }
        
        void llvm_node_codegen::generate_llvm_dynamic_object_set_doc_string_prop(Value *obj, const std::string &name, const std::string &doc, const semantics::syntax_tree &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
        
            std::vector<Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::STRING));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::STRING));
        
            FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_set_docstring_prop", module);
            if (funPtr->getName() != "kite_set_docstring_prop")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_set_docstring_prop");
            }
        
            generate_debug_data(
                builder.CreateCall3(
                    funPtr,
                    obj, 
                    builder.CreateGlobalStringPtr(name.c_str()),
                    builder.CreateGlobalStringPtr(doc.c_str())),
                tree.position);
        }
        
        Value *llvm_node_codegen::generate_llvm_method_alloc(Value *method, const semantics::syntax_tree &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            Function *function = (Function*)method;
            int num_args = function->arg_size();

            std::vector<Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::INTEGER));
            
            FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_method_alloc", module);
            if (funPtr->getName() != "kite_method_alloc")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_method_alloc");
            }
            return generate_debug_data(
                builder.CreateCall2(
                    funPtr, 
                    builder.CreateBitCast(
                        method, 
                        kite_type_to_llvm_type(semantics::OBJECT)), 
                    ConstantInt::get(kite_type_to_llvm_type(semantics::INTEGER), num_args)),
                tree.position);
        }
        
        Value *llvm_node_codegen::generate_llvm_dynamic_object_get_property(Value *obj, const std::string &name, const semantics::syntax_tree &tree, bool set) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            
            std::vector<Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::STRING));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::BOOLEAN));
            
            FunctionType *ft = FunctionType::get(PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)), ArrayRef<Type*>(parameterTypes), false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_dynamic_object_get_property", module);
            if (funPtr->getName() != "kite_dynamic_object_get_property")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_dynamic_object_get_property");
            }
            
            if (obj->getType() == PointerType::getUnqual(parameterTypes[0]))
            {
                obj = generate_debug_data(builder.CreateLoad(obj), tree.position);
            }
            return generate_debug_data(
                builder.CreateCall3(
                    funPtr, 
                    obj, 
                    builder.CreateGlobalStringPtr(name.c_str()), 
                    ConstantInt::get(kite_type_to_llvm_type(semantics::BOOLEAN), set)),
                tree.position);
        }
        
        Value *llvm_node_codegen::generate_llvm_dynamic_object_enable_finalizer(Value *method, const semantics::syntax_tree &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            
            std::vector<Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            
            FunctionType *ft = FunctionType::get(PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)), ArrayRef<Type*>(parameterTypes), false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_dynamic_object_enable_finalizer", module);
            if (funPtr->getName() != "kite_dynamic_object_enable_finalizer")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_dynamic_object_enable_finalizer");
            }
            
            Value *obj = generate_debug_data(
                builder.CreateLoad(state.current_symbol_stack()["this"]),
                tree.position);
                
            return generate_debug_data(
                builder.CreateCall2(
                    funPtr, 
                    obj, 
                    method),
                tree.position);
        }
        
        Value *llvm_node_codegen::generate_llvm_dynamic_object_get_root(const semantics::syntax_tree &tree) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            
            std::vector<Type*> parameterTypes;
            FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), ArrayRef<Type*>(parameterTypes), false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_dynamic_object_get_root", module);
            if (funPtr->getName() != "kite_dynamic_object_get_root")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_dynamic_object_get_root");
            }
            
            return generate_debug_data(builder.CreateCall(funPtr), tree.position);
        }
        
        semantics::builtin_types llvm_node_codegen::get_type(Value *val)
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
            else if (type == Type::getInt8PtrTy(getGlobalContext()))
            {
                op_type = semantics::STRING;
            }
            else if (type->isDoubleTy())
            {
                op_type = semantics::FLOAT;
            }
            else
            {
                op_type = semantics::OBJECT;
            }
            
            return op_type;
        }
        
        Type *llvm_node_codegen::get_object_type() const
        {
            std::vector<Type*> struct_types;
            struct_types.push_back(Type::getIntNTy(getGlobalContext(), sizeof(semantics::builtin_types) * 8)); // type
            struct_types.push_back(kite_type_to_llvm_type(semantics::OBJECT)); // parent
            struct_types.push_back(kite_type_to_llvm_type(semantics::OBJECT)); // alloc method
            struct_types.push_back(
                ArrayType::get(
                    Type::getIntNTy(
                        getGlobalContext(), 
                        8),
                    sizeof(kite::stdlib::System::property_map) +
                    sizeof(kite::stdlib::System::property_doc_map) +
                    sizeof(std::string))); // placeholder
                        
            return PointerType::getUnqual(StructType::get(getGlobalContext(), ArrayRef<Type*>(struct_types)));
        }

        Type *llvm_node_codegen::get_method_type() const
        {
            std::vector<Type*> struct_types;
            // dynamic_object stuff
            struct_types.push_back(Type::getIntNTy(getGlobalContext(), sizeof(semantics::builtin_types) * 8)); // type
            struct_types.push_back(kite_type_to_llvm_type(semantics::OBJECT)); // parent
            struct_types.push_back(kite_type_to_llvm_type(semantics::OBJECT)); // alloc method
            struct_types.push_back(
                ArrayType::get(
                    Type::getIntNTy(
                        getGlobalContext(), 
                        8),
                    sizeof(kite::stdlib::System::property_map) +
                    sizeof(kite::stdlib::System::property_doc_map) +
                    sizeof(std::string))); // placeholder
            
            // method specific stuff
            struct_types.push_back(kite_type_to_llvm_type(semantics::OBJECT)); // method pointer
            struct_types.push_back(kite_type_to_llvm_type(semantics::OBJECT)); // this pointer
            struct_types.push_back(kite_type_to_llvm_type(semantics::INTEGER)); // number of arguments
            return PointerType::getUnqual(StructType::get(getGlobalContext(), ArrayRef<Type*>(struct_types)));
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
                    return System::object::method_map;
                }
            }
        }
        
        std::string llvm_node_codegen::type_to_code(semantics::builtin_types type)
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
                    assert(0);
                }
            }
        }
        
        Type *llvm_node_codegen::kite_type_to_llvm_type(semantics::builtin_types type)
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
                case semantics::OBJECT:
                {
                    /*PATypeHolder StructTy = OpaqueType::get(getGlobalContext());
                    std::vector<const Type*> Elts;
                    Elts.push_back(Type::getInt32Ty(getGlobalContext()));
                    Elts.push_back(PointerType::getUnqual(StructTy));
                    StructType *newStructType = StructType::get(getGlobalContext(), Elts);
                    cast<OpaqueType>(StructTy.get())->refineAbstractTypeTo(newStructType);
                    newStructType = cast<StructType>(StructTy.get());
                    return PointerType::getUnqual(newStructType);*/
                    return PointerType::getUnqual(Type::getInt32Ty(getGlobalContext()));
                }
                default:
                {
                    assert(0);
                }
            }
        }
        
        std::string llvm_node_codegen::kite_type_to_function_prefix(semantics::builtin_types type)
        {
            std::string returnValue;
            
            switch(type)
            {
                case semantics::INTEGER:
                {
                    returnValue = INTEGER_METHOD_PREFIX_AS_STRING;
                    break;
                }
                case semantics::FLOAT:
                {
                    returnValue = FLOAT_METHOD_PREFIX_AS_STRING;
                    break;
                }
                case semantics::BOOLEAN:
                {
                    returnValue = BOOLEAN_METHOD_PREFIX_AS_STRING;
                    break;
                }
                case semantics::STRING:
                {
                    returnValue = STRING_METHOD_PREFIX_AS_STRING;
                    break;
                }
                case semantics::OBJECT:
                {
                    break;
                }
                default:
                {
                    assert(0);
                }
            }
        
            return returnValue;
        }
        
        Instruction *llvm_node_codegen::generate_debug_data(Instruction *instruction, const semantics::syntax_tree_position &pos) const
        {
            const DebugLoc &debugLoc = instruction->getDebugLoc();
            
            if (debugLoc.getLine() == 0)
            {
                boost::filesystem::path p(pos.file);
                
                // Create compile unit
                if (state.compileUnitCache.find(pos.file) == state.compileUnitCache.end())
                {
                    // Note: createCompileUnit seems to create one compile unit per module.
                    // Maybe we should look into this.
                    state.current_debug_builder()->createCompileUnit (0xAA35, p.filename().string(), p.parent_path().string(), "Kite version 2.0", false, "", 0);
                    state.compileUnitCache[pos.file] = const_cast<MDNode*>(state.current_debug_builder()->getCU());
                }

                DIFile file = state.current_debug_builder()->createFile(p.filename().string(), p.parent_path().string());

                std::map<std::string, DISubprogram> &subroutineCache = state.subroutineCache;
                DISubprogram subprogram;
                if (subroutineCache.find(state.current_c_method_name()) == subroutineCache.end())
                {
                    DIType basicType = state.current_debug_builder()->createBasicType("System::object", sizeof(void*)*8 /* TODO */, 0, DW_ATE_address);
                    DIType pointerType = state.current_debug_builder()->createPointerType(basicType, sizeof(void*)*8);
                    
                    std::vector<Value*> fxnTypes;
                    fxnTypes.push_back(pointerType);
                    fxnTypes.push_back(pointerType);
                    
                    DIType type = state.current_debug_builder()->createSubroutineType(
                        file,
                        DIArray(MDNode::get(getGlobalContext(), fxnTypes))
                        );
                    
                    subprogram = state.current_debug_builder()->createFunction(
                        file,
                        state.full_class_name() + "|" + state.current_friendly_method_name(), 
                        state.current_c_method_name(),
                        file,
                        1 /* TODO: line# */,
                        type,
                        false,
                        true,
                        1 /* TODO: line# */
                        );
                    
                    assert(subprogram.Verify());
                    
                    subroutineCache[state.current_c_method_name()] = subprogram;
                }
                else
                {
                    subprogram = subroutineCache[state.current_c_method_name()];
                }

                // Create lexical block.
                DILexicalBlock lexicalBlock;
                if (state.lexicalBlockCache.find(state.current_c_method_name()) == state.lexicalBlockCache.end())
                {
                    lexicalBlock = state.current_debug_builder()->createLexicalBlock(subprogram, file, 1 /* TODO: line */, 1 /* TODO: col */);
                    assert(lexicalBlock.Verify());
                    state.lexicalBlockCache[state.current_c_method_name()] = lexicalBlock;
                }
                else
                {
                    lexicalBlock = state.lexicalBlockCache[state.current_c_method_name()];
                }
                               
                // Create new debug location
                DebugLoc newLoc = DebugLoc::get(pos.line, pos.column, lexicalBlock);
                instruction->setDebugLoc(newLoc);
            }
            return instruction;
        }
    }
}
