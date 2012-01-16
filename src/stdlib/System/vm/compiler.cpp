/*****************************************************************************
 * Copyright (c) 2012, Mooneer Salem
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

#include <stdlib/language/kite.h>
#include <stdlib/language/kite/syntax_tree.h>
#include "compiler.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace vm
            {
                object* compiler::compileFile(string *file)
                {
                    language::kite::syntax_tree ast;
                    
                    std::string p(std::string(file->string_val.c_str()));
                    if (ast.from_file(p))
                    {
                        dynamic_object *ctx = new dynamic_object();
                        language::kite::kite::ExecuteCode(ast, ctx);
                        return ctx;
                    }
                    else
                    {
                        return NULL;
                    }
                }
                
                object* compiler::compileString(string *code)
                {
                    language::kite::syntax_tree ast;
                    
                    std::string p(code->string_val.c_str());
                    if (ast.from_string(p))
                    {
                        dynamic_object *ctx = new dynamic_object();
                        language::kite::kite::ExecuteCode(ast, ctx);
                        return ctx;
                    }
                    else
                    {
                        return NULL;
                    }
                }
            }
        }
    }
}
REGISTER_KITE_CLASS(kite::stdlib::System::vm::vm, kite::stdlib::System::vm::compiler)