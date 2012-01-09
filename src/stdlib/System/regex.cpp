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

#include "regex.h"
#include "exceptions/exception.h"
#include "regex/match_result.h"
#include "list.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace regex
            {
                object* regex::match(string *str)
                {
                    return match(str, new integer(0));
                }
                
                object* regex::match(string *str, integer *start)
                {
                    boost::match_results<string_type::const_iterator/*, gc_allocator<char>*/ > results;
                    string_type::const_iterator start_iter = str->string_val.begin();
                    string_type::const_iterator end_iter = str->string_val.end();
                    
                    for (int index = 0; index < start->val && start_iter != end_iter; index++)
                    {
                        start_iter++;
                    }
                    
                    if (regex_search(start_iter, end_iter, results, *regex_obj))
                    {
                        match_result *res = match_result::Create(0);
                        string_type test_string_prefix(start_iter, results[0].first);
                        res->properties["match_begin"] = new integer(test_string_prefix.size());
                        
                        string_type test_string(results[0].first, results[0].second);
                        res->properties["match_size"] = new integer(test_string.size());
                        
                        list *l = list::Create(0);
                        for (int index = 0; index < results.size(); index++)
                        {
                            string_type test_string(results[index].first, results[index].second);
                            l->list_contents.push_back(
                                new string(test_string)
                            );
                        }
                        res->properties["captures"] = l;
                        return res;
                    }
                    else
                    {
                        return NULL;
                    }
                }
                
                object* regex::replace(string *str, string *replace_with)
                {
                    return replace(str, replace_with, new integer(65535));
                }
                
                object* regex::replace(string *str, string *replace_with, integer *max)
                {
                    std::string result_string(str->string_val.c_str());
                    
                    for (int count = 0; count < max->val; count++)
                    {
                        std::string tmp_result_string;
                        
                        tmp_result_string = regex_replace(std::string(result_string.c_str()), *regex_obj, replace_with->string_val.c_str(), boost::regex_constants::format_first_only);
                        if (tmp_result_string == result_string) break;
                        result_string = tmp_result_string;
                    }
                    
                    return new string(result_string.c_str());
                }
                
                object* regex::split(string *str)
                {
                    return split(str, new integer(65535));
                }
                
                object* regex::split(string *str, integer *max)
                {
                    boost::regex_token_iterator<string_type::const_iterator> i(str->string_val.begin(), str->string_val.end(), *regex_obj, -1);
                    boost::regex_token_iterator<string_type::const_iterator> j;
                    
                    int count = 0;
                    list *vals = list::Create(0);
                    while (i != j)
                    {
                        if (count < max->val)
                        {
                            vals->list_contents.push_back(new string(i->str().c_str()));
                            count++;
                        }
                        else
                        {
                            string *s = (string*)vals->list_contents[vals->list_contents.size() - 1];
                            s->string_val += i->str().c_str();
                        }
                        i++;
                    }
                    
                    return vals;
                }
            }
        }
    }
}
REGISTER_KITE_CLASS(kite::stdlib::System::System, kite::stdlib::System::regex::regex)