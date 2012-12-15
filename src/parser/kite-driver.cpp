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

#include "kite-driver.hh"
#include "kite-parser.hh"

kite_driver::kite_driver()
    : trace_scanning(false), trace_parsing(false)
{ }

kite_driver::~kite_driver()
{ }

int kite_driver::parse (const std::string &f)
{
    file = f;
    scan_begin ();
    yy::kite_parser parser (*this);
    parser.set_debug_level (trace_parsing);
    int res = parser.parse ();
    scan_end ();
    return res;
}

void kite_driver::error (const yy::location &l, const std::string &m)
{
    std::stringstream ss;
    
    ss   << "parse error at file " << pos.begin.filename
         << " line " << pos.begin.line << " column " << pos.begin.column
         << ":" << m << std::endl;
         
    error(ss.str());
}

void kite_driver::error (const std::string &m)
{
    System::exceptions::exception *exc = 
        System::exceptions::FileError::Create(
            1,
            new System::string(
                m.c_str())
        );
    exc->throw_exception();
}