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

#ifndef KITE_CODEGEN__SYNTAX_TREE_PRINTER_H
#define KITE_CODEGEN__SYNTAX_TREE_PRINTER_H

#include <semantics/syntax_tree.h>

namespace kite
{
    namespace codegen
    {
        const int tabsize = 4;
        std::string tab(int indent);
        
        struct syntax_tree_printer
        {
            int indent;
            syntax_tree_printer(int indent = 0);
            void operator()(semantics::syntax_tree const &tree) const;
        };
        
        struct syntax_tree_node_printer : boost::static_visitor<>
        {
            syntax_tree_node_printer(int indent = 0);
            void operator()(semantics::syntax_tree const& tree) const;
            void operator()(int const& num) const;
            void operator()(double const& num) const;
            void operator()(bool const& num) const;
            void operator()(std::string const& text) const;
            
            int indent;
        };
    }
}

#endif