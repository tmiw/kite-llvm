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

#include "syntax_tree.h"

#include <sstream>
#include <fstream>
#include <parser/parser.h>
#include <stdlib/System/dynamic_object.h>
#include <codegen/syntax_tree_printer.h>

namespace kite
{
    namespace stdlib
    {
        namespace language
        {
            namespace kite
            {
                bool syntax_tree::from_file(std::string file)
                {
                    std::ifstream stream(file.c_str());
                    return from_stream(stream, file);
                }

                bool syntax_tree::from_stream(std::istream &stream, std::string filename)
                {
                    return parser::kite_parser().parse(stream, &ast, filename);
                }

                bool syntax_tree::from_string(std::string &code)
                {
                    std::istringstream stream(code.c_str());
                    return from_stream(stream, "(stdin)");
                }

                void syntax_tree::print()
                {
                    codegen::syntax_tree_printer printer;
                    printer(*ast);
                }
            }
        }
    }
}
