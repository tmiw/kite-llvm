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
 
#ifndef KITE_SEMANTICS__SYNTAX_TREE_H
#define KITE_SEMANTICS__SYNTAX_TREE_H

#include <deque>
#include <string>
#include <boost/variant/recursive_variant.hpp>
#include "constants.h"

namespace kite
{
    namespace semantics
    {
        struct syntax_tree;
        
        typedef boost::variant<
            boost::recursive_wrapper<syntax_tree>,
            int,
            double,
            bool,
            void*,
            std::string> syntax_tree_node;
        
        struct syntax_tree_position
        {
            int line;
            int column;
            std::string file;
            
            syntax_tree_position() { }
            
            template<typename PositionT>
            syntax_tree_position(PositionT &pos)
                : line(pos.begin.line), column(pos.begin.column), file(*pos.begin.filename)
            {
                // empty
            }
            
            syntax_tree_position(const syntax_tree_position &pos)
                : line(pos.line), column(pos.column), file(pos.file)
            {
                // empty
            }
        };
        
        struct syntax_tree
        {
            code_operation op;
            std::deque<syntax_tree_node> children;
            
            syntax_tree_position position;
            
            syntax_tree() { }
            
            template<typename PositionT>
            syntax_tree(PositionT &pos)
                : position(pos)
                { }
            
            syntax_tree(const syntax_tree &other)
                : op(other.op), children(other.children), position(other.position)
                { }
        };
    }
}

#endif