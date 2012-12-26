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
 
#ifndef KITE_SEMANTICS__GC_HELPER_H
#define KITE_SEMANTICS__GC_HELPER_H

// Just some helper typedefs to ensure we use garbage collected objects.

#include <string>
#include <gc/gc_allocator.h>
#include <map>
#include <vector>
#include <deque>

namespace kite
{
    namespace semantics
    {
        typedef std::basic_string<char, std::char_traits<char>, gc_allocator<char> > gc_string;
        
        template<typename K, typename V>
        struct gc_map
        {
            typedef std::map<K, V, std::less<K>, gc_allocator<std::pair<const K, V> > > type;
        };
        
        template<typename V>
        struct gc_vector
        {
            typedef std::vector<V, gc_allocator<V> > type;
        };
        
        template<typename V>
        struct gc_deque
        {
            typedef std::deque<V, gc_allocator<V> > type;
        };
    }
}

#endif // !KITE_SEMANTICS__GC_HELPER_H