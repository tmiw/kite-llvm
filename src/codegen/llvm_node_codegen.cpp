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
                case semantics::METHOD:
                    ret = codegen_method_op(tree);
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
            std::string property_name = boost::get<std::string>(tree.children[0]);
            return generate_llvm_dynamic_object_get_property(prev, property_name);
        }
        
        Value *llvm_node_codegen::codegen_deref_method_op(semantics::syntax_tree const &tree, Value *prev) const
        {
            std::string method_name = boost::get<std::string>(tree.children[0]);
            std::vector<Value*> parameters;
            
            parameters.push_back(prev);
            for (int i = 1; i < tree.children.size(); i++)
            {
                Value *param_val = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                parameters.push_back(param_val);
            }
            
            return generate_llvm_method_call(prev, method_name, parameters);
        }
        
        Value *llvm_node_codegen::codegen_deref_method_relative_self_op(semantics::syntax_tree const &tree) const
        {
            std::string method_name = boost::get<std::string>(tree.children[0]);
            std::vector<Value*> parameters;
            std::map<std::string, Value*> &sym_stack = state.current_symbol_stack();
            Value *self = state.module_builder().CreateLoad(sym_stack["this"]);
            
            parameters.push_back(self);
            for (int i = 1; i < tree.children.size(); i++)
            {
                Value *param_val = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                parameters.push_back(param_val);
            }
            
            return generate_llvm_method_call(self, method_name, parameters);
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
            IRBuilder<> &builder = state.module_builder();
            
            if (sym_stack.count(var_name) == 0)
            {
                sym_stack[var_name] = builder.CreateAlloca(kite_type_to_llvm_type(semantics::INTEGER));
                builder.CreateStore(ConstantInt::get(getGlobalContext(), APInt(32, 0, true)), sym_stack[var_name]);
            }
            return builder.CreateLoad(sym_stack[var_name]);
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
                        //if (lhs->getType() != kite_type_to_llvm_type(semantics::OBJECT))
                        //{
                            sym_stack[i->first] = state.module_builder().CreateAlloca(rhs->getType());
                            ptr = sym_stack[i->first];
                        /*}
                        else
                        {
                            std::vector<Value*> params;
                            rhs = generate_llvm_method_call(rhs, "obj", params);
                        }*/
                    }
                    state.module_builder().CreateStore(rhs, ptr);
                    return rhs;
                }
            }
            
            // Else, just a simple store into a property.
            // TODO
            state.module_builder().CreateStore(rhs, lhs);
            return rhs;
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
            
            builder.CreateBr(condBB);
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
                    decideResults.push_back(ConstantInt::get(getGlobalContext(), APInt(sizeof(void*) << 3, (uint64_t)0, true)));
                    decideBlocks.push_back(condBB);
                    condBB = endBB;
                }
                builder.CreateCondBr(condition, actionBB, condBB);
                builder.SetInsertPoint(actionBB);
                Value *result = boost::apply_visitor(llvm_node_codegen(state), tree.children[i]);
                if (get_type(result) != semantics::OBJECT) 
                {
                    std::vector<Value*> emptyList;
                    emptyList.push_back(result);
                    result = generate_llvm_method_call(result, "obj", emptyList);
                }
                decideResults.push_back(result);
                decideBlocks.push_back(actionBB);
                builder.CreateBr(endBB);
                builder.SetInsertPoint(condBB);
            }
            
            builder.SetInsertPoint(endBB);
            PN = builder.CreatePHI(kite_type_to_llvm_type(semantics::OBJECT));
            for (int index = 0; index < decideResults.size(); index++)
            {
                Value *val = decideResults[index];
                BasicBlock *bb = decideBlocks[index];
                PN->addIncoming(val, bb);
            }
            return PN;
            //return ConstantInt::get(getGlobalContext(), APInt(32, 0, true)); // TODO
        }
        
        Value *llvm_node_codegen::codegen_method_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            int numargs = tree.children.size() - 2;
            std::string functionName = boost::get<std::string>(tree.children[0]);
            std::vector<std::string> argnames;

            if (numargs < 0) numargs = 0;
            for (int i = 1; i < tree.children.size() - 1; i++)
            {
                argnames.push_back(boost::get<std::string>(tree.children[i]));
            }
        
            semantics::syntax_tree &body = const_cast<semantics::syntax_tree&>(boost::get<semantics::syntax_tree>(tree.children[tree.children.size() - 1]));
            Value *method = generate_llvm_method(functionName, argnames, body);
            
            Value *method_obj = generate_llvm_method_alloc(method);
            Value *property = builder.CreateLoad(state.current_symbol_stack()["this"]);
            functionName += "__";
            functionName += std::string("o", tree.children.size() + 1);
            Value *prop_entry = generate_llvm_dynamic_object_get_property(property, functionName);
            builder.CreateStore(method_obj, prop_entry);
            
            state.current_symbol_stack()[functionName] = prop_entry;
            return method;
        }

        Value *llvm_node_codegen::generate_llvm_method(std::string name, std::vector<std::string> &argnames, semantics::syntax_tree &body) const
        {
            BasicBlock *currentBB = state.module_builder().GetInsertBlock();
            std::vector<const Type*> argTypes;
            std::string functionName = state.identifier_prefix() + name;
            int numargs = argnames.size();

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
            
            Module *currentModule = state.current_module();
            FunctionType *FT = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), argTypes, false);
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
                symStack[name] = builder.CreateAlloca(kite_type_to_llvm_type(semantics::OBJECT));
                builder.CreateStore(AI, symStack[name]);
            }
            Value *ret = llvm_node_codegen(state)(body);
            state.pop_symbol_stack();
            
            if (get_type(ret) != semantics::OBJECT)
            {
                std::vector<Value*> params;
                params.push_back(ret);
                ret = generate_llvm_method_call(ret, "obj", params);
            }
            builder.CreateRet(ret);
            state.module_builder().SetInsertPoint(currentBB);
            return F;
        }
        
        Value *llvm_node_codegen::generate_llvm_method_call(Value *self, std::string name, std::vector<Value*> &params) const
        {
            IRBuilder<> &builder = state.module_builder();
            Module *module = state.current_module();
            semantics::builtin_types type = get_type(self);
            stdlib::object_method_map &method_map = get_method_map(type);
            std::string method_name = name;
            std::vector<const Type*> parameterTypes;
            std::vector<Value*> paramsCopy(params);
            
            if (self->getType() == PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)))
            {
                bool replaceParam = false;
                if (params[0] == self) replaceParam = true;
                self = builder.CreateLoad(self);
                if (replaceParam) paramsCopy[0] = self;
            }
            
            method_name += std::string("__");
            for (int i = 0; i < params.size(); i++)
            {
                method_name += type_to_code(get_type(paramsCopy[i]));
                parameterTypes.push_back(kite_type_to_llvm_type(get_type(paramsCopy[i])));
            }
            function_semantics &semantics = method_map[method_name];
                
            const FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics.first), parameterTypes, false);
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
                                paramsCopy[i] = generate_llvm_method_call(v, std::string("obj"), castParams);
                            }
                        }
                        
                        method_name = name + std::string("__");
                        for (int i = 0; i < paramsCopy.size(); i++)
                        {
                            method_name += type_to_code(get_type(paramsCopy[i]));
                        }
                    }
                } while (count++ <= 1);
                
                if (fptr == NULL)
                {
                    std::vector<const Type*> parameterTypesLookup;
                    std::vector<Value*> paramValuesLookup;
                    parameterTypesLookup.push_back(PointerType::getUnqual(Type::getInt32Ty(getGlobalContext())));
                    parameterTypesLookup.push_back(kite_type_to_llvm_type(semantics::STRING));
                    parameterTypesLookup.push_back(kite_type_to_llvm_type(semantics::INTEGER));
                    const FunctionType *ftPtrLookup = FunctionType::get(parameterTypesLookup[0], parameterTypesLookup, false);
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
                        builder.CreateCall(
                            funPtrLookup,
                            paramValuesLookup.begin(),
                            paramValuesLookup.end()
                        ),
                        PointerType::getUnqual(ft)
                    );
                }
            
            }
            self = builder.CreateCall(
                fptr,
                paramsCopy.begin(),
                paramsCopy.end()
            );
            return self;
        }
        
        Value *llvm_node_codegen::codegen_class_op(semantics::syntax_tree const &tree) const
        {
            Value *ret;
            IRBuilder<> &builder = state.module_builder();
            BasicBlock *currentBlock = builder.GetInsertBlock();
            std::string className = boost::get<std::string>(tree.children[0]);
            
            state.push_namespace_stack(className);
            semantics::syntax_tree &body = const_cast<semantics::syntax_tree&>(boost::get<semantics::syntax_tree>(tree.children[tree.children.size() - 1]));
            std::vector<std::string> args;
            ret = generate_llvm_method(std::string("__static_init__"), args, body);
            state.pop_namespace_stack();

            // Initialize dynamic_object that will store the class and insert
            // LLVM code to call __static_init__ on this object.
            Value *obj = generate_llvm_dynamic_object_alloc();
            if (tree.children.size() > 2)
            {
                // Set child class parent.
                Value *parent = boost::apply_visitor(llvm_node_codegen(state), tree.children[1]);
                generate_llvm_dynamic_object_set_parent(obj, parent);
            }
            builder.CreateCall(ret, obj);
            
            Value *property = builder.CreateLoad(state.current_symbol_stack()["this"]);
            Value *prop_entry = generate_llvm_dynamic_object_get_property(property, className);
            builder.CreateStore(obj, prop_entry);
            
            state.current_symbol_stack()[className] = prop_entry;
            
            return obj;
        }

        Value *llvm_node_codegen::codegen_make_op(semantics::syntax_tree const &tree) const
        {
            IRBuilder<> &builder = state.module_builder();
            std::vector<Value*> params;
            Value *parent = boost::apply_visitor(llvm_node_codegen(state), tree.children[0]);
            
            Value *obj = generate_llvm_dynamic_object_alloc();
            generate_llvm_dynamic_object_set_parent(obj, parent);
            
            params.push_back(obj);
            for(int i = 1; i < tree.children.size(); i++)
            {
                params.push_back(boost::apply_visitor(llvm_node_codegen(state), tree.children[i]));
            }
            generate_llvm_method_call(obj, "__init__", params);
            return obj;
        }
        
        Value *llvm_node_codegen::generate_llvm_dynamic_object_alloc() const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            
            std::vector<const Type*> parameterTypes;
            const FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), parameterTypes, false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_dynamic_object_alloc", module);
            if (funPtr->getName() != "kite_dynamic_object_alloc")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_dynamic_object_alloc");
            }
            Value *obj = builder.CreateCall(funPtr);
            return obj;
        }
        
        void llvm_node_codegen::generate_llvm_dynamic_object_set_parent(Value *obj, Value *parent) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            
            std::vector<const Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            
            const FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), parameterTypes, false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_dynamic_object_set_parent", module);
            if (funPtr->getName() != "kite_dynamic_object_set_parent")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_dynamic_object_set_parent");
            }
            builder.CreateCall2(funPtr, obj, parent);
        }
        
        Value *llvm_node_codegen::generate_llvm_method_alloc(Value *method) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            
            std::vector<const Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            
            const FunctionType *ft = FunctionType::get(kite_type_to_llvm_type(semantics::OBJECT), parameterTypes, false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_method_alloc", module);
            if (funPtr->getName() != "kite_method_alloc")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_method_alloc");
            }
            return builder.CreateCall(funPtr, builder.CreateBitCast(method, kite_type_to_llvm_type(semantics::OBJECT)));
        }
        
        Value *llvm_node_codegen::generate_llvm_dynamic_object_get_property(Value *obj, std::string name) const
        {
            Module *module = state.current_module();
            IRBuilder<> &builder = state.module_builder();
            
            std::vector<const Type*> parameterTypes;
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::OBJECT));
            parameterTypes.push_back(kite_type_to_llvm_type(semantics::STRING));
            
            const FunctionType *ft = FunctionType::get(PointerType::getUnqual(kite_type_to_llvm_type(semantics::OBJECT)), parameterTypes, false);
            Function *funPtr = Function::Create(ft, Function::ExternalLinkage, "kite_dynamic_object_get_property", module);
            if (funPtr->getName() != "kite_dynamic_object_get_property")
            {
                funPtr->eraseFromParent();
                funPtr = module->getFunction("kite_dynamic_object_get_property");
            }
            return builder.CreateCall2(funPtr, obj, builder.CreateGlobalStringPtr(name.c_str()));
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
        
        const Type *llvm_node_codegen::kite_type_to_llvm_type(semantics::builtin_types type)
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
    }
}
