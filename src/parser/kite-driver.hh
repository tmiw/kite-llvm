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

#ifndef KITE_DRIVER_HH
#define KITE_DRIVER_HH

#include <string>
#include "semantics/syntax_tree.h"
#include "kite-parser.hh"

// Tell Flex the lexer's prototype.
#define YY_DECL \
    yy::kite_parser::token_type yylex(
        yy::kite_parser::semantic_type *yylval, \
        yy::kite_parser::location_type *yylloc, \
        kite_driver& driver)
YY_DECL;

// Define parser driver.
class kite_driver
{
public:
    kite_driver();
    virtual ~kite_driver();
    
    // Scanner methods.
    void scan_begin();
    void scan_end();
    bool trace_scanning;
    
    // Parser methods.
    int parse(const std::string &f);
    std::string file;
    bool trace_parsing;
    kite::semantics::syntax_tree *result;
    
    // Error handling.
    void error(const yy::location &l, const std::string &m);
    void error(const std::string &m);
};

#endif