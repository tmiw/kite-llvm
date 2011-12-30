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
 
#ifndef KITE_STDLIB__SYSTEM_COLLECTIONS__QUEUE_H
#define KITE_STDLIB__SYSTEM_COLLECTIONS__QUEUE_H

#include <deque>
#include "stdlib/System/collections.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace collections
            {
                typedef std::deque<object*, gc_allocator<object*> > queue_contents_type;

                BEGIN_KITE_BASE_CLASS(queue)
                    private:
                        static object *s_enqueue(queue *list, object *item) { return list->enqueue(item); }
                        static object *s_dequeue(queue *list) { return list->dequeue(); }
                        static object *s_size(queue *list) { return list->size(); }
                        
                    public:
                        object *enqueue(object *item);
                        object *dequeue();
                        object *size();
                                                
                    BEGIN_KITE_CLASS_INITIALIZER    
                        KITE_METHOD_DEFINE(enqueue, 1, &queue::s_enqueue);
                        KITE_METHOD_DEFINE(dequeue, 0, &queue::s_dequeue);
                        KITE_METHOD_DEFINE(size, 0, &queue::s_size);
                    END_KITE_CLASS_INITIALIZER
                
                    queue_contents_type queue_contents;
                END_KITE_CLASS
            }
        }
    }
}

extern "C"
{
    void *kite_list_new();
    void kite_list_append(void *list, void *item);
}

#endif