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
 
#ifndef KITE_STDLIB__SYSTEM__METHOD_H
#define KITE_STDLIB__SYSTEM__METHOD_H

#include "dynamic_object.h"
#include "string_type.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            struct list;
            
            struct method : System::object
            {
                typedef std::map<std::string, std::string> method_arg_doc_map;
                
                static System::dynamic_object class_obj;
                static object_method_map method_map;
                
                void *method_ptr;
                System::object *this_ptr;
                int num_args;
                method_arg_doc_map arg_map;
                
                method(void *ptr) :
                    System::object(semantics::METHOD_TY), method_ptr(ptr), this_ptr(NULL) 
                {
                     // empty
                }
                
                static object *get_param_names(method *method);
                static object *get_param_doc(method *method, string *name);
                
                object *invoke() { return invoke(NULL); }
                object *invoke(object *param1, ...);
                object *invoke_with_arg_list(list *l);
                
                static void InitializeClass();
            };
        }
    }
}

extern "C"
{
    void *kite_method_alloc(void *method_ptr, int args);
    void *kite_method_verify_semantics(void *method, int args);
    void *kite_eval_code(void *code, int args, ...);
}

#endif
