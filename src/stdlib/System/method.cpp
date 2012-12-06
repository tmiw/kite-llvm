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

#include "config.h"
#include <stdarg.h>
#include FFI_HEADER
#include <iostream>
#include <sstream>
#include "method.h"
#include "exceptions/InvalidArgument.h"
#include "../language/kite/syntax_tree.h"
#include "../language/kite.h"
#include "string.h"
#include "list.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            System::dynamic_object method::class_object;
            
            object *method::invoke(object *param1, ...)
            {
                ffi_cif cif;
                ffi_type *args[num_args + 1];
                void *values[num_args + 1];
                void **val_heap = (void**)malloc((num_args + 1) * sizeof(void*));
                void *rv;
                args[0] = &ffi_type_pointer;
                values[0] = &val_heap[0];
                for (int i = 0; i < num_args; i++)
                {
                    args[i + 1] = &ffi_type_pointer;
                    values[i + 1] = (void*)&val_heap[i + 1];
                }
                int rc = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, num_args + 1, &ffi_type_pointer, args);
                assert(rc == FFI_OK);
                val_heap[0] = (void*)this_ptr;
                va_list vl;
                va_start(vl, param1);
                if (param1 != NULL)
                {
                    val_heap[1] = param1;
                    for (int i = 0; i < num_args - 1; i++)
                    {
                        val_heap[i + 2] = va_arg(vl, void*);
                    }
                }
                ffi_call(&cif, (void(*)())method_ptr, &rv, values);
                va_end(vl);
                free(val_heap);
                return (object*)rv;
            }
            
            object *method::invoke_with_arg_list(list *l)
            {
                ffi_cif cif;
                ffi_type *args[num_args + 1];
                void *values[num_args + 1];
                void **val_heap = (void**)malloc((num_args + 1) * sizeof(void*)); /*new void*[numargs + 1];*/
                void *rv;
                args[0] = &ffi_type_pointer;
                values[0] = &val_heap[0];
                for (int i = 0; i < num_args; i++)
                {
                    args[i + 1] = &ffi_type_pointer;
                    values[i + 1] = (void*)&val_heap[i + 1];
                }
                int rc = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, num_args + 1, &ffi_type_pointer, args);
                assert(rc == FFI_OK);
                val_heap[0] = (void*)this_ptr;
                for (int i = 0; i < num_args; i++)
                {
                    val_heap[i + 1] = (void*)l->list_contents[i];
                }
                ffi_call(&cif, (void(*)())method_ptr, &rv, values);
                free(val_heap);
                return (object*)rv;
            }
            
            void method::InitializeClass()
            {
                class_object.properties["__name"] = new string("System.method");
            }
        }
    }
}
using namespace kite::stdlib;

void *kite_method_alloc(void *method_ptr, int args)
{
    System::method *method = new System::method(method_ptr);
    method->num_args = args;
    method->this_ptr = NULL;
    return (void*)method;
}

void *kite_method_verify_semantics(void *method, int args)
{
    System::method *methodObj = (System::method*)method;
    return methodObj->method_ptr;
}

void *kite_eval_code(void *code, int args, ...)
{   
    va_list vl;
    va_start(vl, args);
    void *ret = language::kite::kite::GenerateEvalMethod((System::object*)code, args, vl);
    va_end(vl);
    
    return ret;    
}