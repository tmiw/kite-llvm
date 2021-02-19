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
    printf("Usage: %s [-hd]\n", app_name);
    printf("       -h: this message\n");
    printf("       -o: optimize compiled code\n");
    exit(0);
}

int main(int argc, char **argv)
{
    int ch;
    bool optimize_code = false;
    char *app_name = argv[0];
    
    while ((ch = getopt(argc, argv, "ho")) != -1)
    {
        switch(ch)
        {
            case 'o':
                optimize_code = true;
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

    cout << "Interactive Kite console" << endl;
    
    while(!cin.eof())
    {
        char buf[1024];
        string code;

        cout << "ikt> ";
        cin.getline(buf, 1024);
        code = buf;

        if (code.size() > 0)
        {
            KITE_EXCEPTION_RUN
                bool r;
                language::kite::syntax_tree ast; 
                r = ast.from_string(code);

                language::kite::kite::PushModule();
                System::object *retValue = language::kite::kite::ExecuteCode(ast);
                cout << "---> ";
                if (retValue) retValue->print();
                else cout << "null" << endl;
            KITE_EXCEPTION_CATCH
                System::exceptions::exception *exc = 
                    (System::exceptions::exception *)language::kite::kite::last_exception;
                System::dynamic_object *real_exc_class = (System::dynamic_object*)exc->parent;
                std::cout << ((System::string*)real_exc_class->properties["__name"])->string_val << ": ";
                exc->properties["message"]->print();
                exc->properties["trace"]->print();
            KITE_EXCEPTION_END
        }
    }

    return 0;
}
