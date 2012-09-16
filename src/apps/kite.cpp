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

#include <iostream>
#include <cstdio>
#include <stdlib/language/kite.h>
#include <stdlib/language/kite/syntax_tree.h>
#include <stdlib/System/dynamic_object.h>
#include <stdlib/System/exceptions/exception.h>

using namespace kite;
using namespace std;
using namespace kite::stdlib;

void usage(char *app_name)
{
    printf("Usage: %s [-haod] [file]\n", app_name);
    printf("       -h: this message\n");
    printf("       -a: output parse tree\n");
    printf("       -o: optimize compiled code\n");
    printf("       -d: dump compiled code to stdout\n");
    printf("       -x: suppress execution of code\n");
    printf("       -g: automatically attach debugger on uncaught exceptions\n");
    printf("     file: the Kite file to execute (default: standard input)\n");
    exit(0);
}

int main(int argc, char **argv)
{
    int ch;
    bool output_ast = false;
    bool optimize_code = false;
    bool dump_code = false;
    bool suppress_exec = false;
    bool attach_debugger = false;
    char *app_name = argv[0];
    
    while ((ch = getopt(argc, argv, "haodxg")) != -1)
    {
        switch(ch)
        {
            case 'a':
                output_ast = true;
                break;
            case 'o':
                optimize_code = true;
                break;
            case 'd':
                dump_code = true;
                break;
            case 'x':
                suppress_exec = true;
                break;
            case 'g':
                attach_debugger = true;
                break;
            case 'h':
            default:
                usage(app_name);
        }
    }
    
    argc -= optind;
    argv += optind;
         
    language::kite::kite::InitializeRuntimeSystem(app_name, argc, argv);
    language::kite::kite::enable_optimizer = optimize_code;
    System::exceptions::exception::debugger_attach_on_uncaught_exception = attach_debugger;
    
    bool r;
    language::kite::syntax_tree ast; 
    
    if (argc > 0) r = ast.from_file(argv[0]);
    else r = ast.from_stream(cin, "(stdin)");

    if (r)
    {
        if (output_ast)
        {
            ast.print();
        }
        
        language::kite::kite::ExecuteCode(ast, suppress_exec); 
        if (dump_code) language::kite::kite::DumpCompiledCode();

        return 0;
    }
    else
    {
        std::cout << "Parsing failed\n";
        return 1;
    }
}
