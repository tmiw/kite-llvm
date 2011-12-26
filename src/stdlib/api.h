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
 
#ifndef KITE_STDLIB__API_H
#define KITE_STDLIB__API_H

#include "System/dynamic_object.h"

namespace kite 
{
    namespace stdlib 
    {
        /**
         * Basic allocator method. Creates new object of the given type
         * and ensures all constructors are called. NOT INTENDED FOR PUBLIC
         * USE.
         */
        template<typename T>
        class ObjectAllocator
        {
        public:
            static T* NewObject()
            {
                return new T();
            }
            
            static void* GetAllocatorMethodPointer()
            {
                return (void*)&NewObject;
            }
        };
    }
}

/**
 * Defines a Kite class that inherits from a base class.
 * @param name Desired name of the class (relative to current namespace)
 * @param inherit_from Standard libary class to inherit from (which must eventually inherit System::dynamic_object)
 */
#define BEGIN_KITE_CHILD_CLASS(name, inherit_from) \
    struct name : inherit_from \
    { \
    private: \
        static void _SetAllocator() \
        { \
            class_object.obj_alloc_method = ObjectAllocator<name>::GetAllocatorMethodPointer(); \
        } \
        \
    public: \
        static kite::stdlib::System::dynamic_object class_object; \
        \
        name() : inherit_from(&class_object) { } \
        name(kite::stdlib::System::dynamic_object *parent) : inherit_from(parent) { }

/**
 * Begins class initializer definition.
 */
#define BEGIN_KITE_CLASS_INITIALIZER \
    public: \
        static void InitializeClass() \
        { \
            _SetAllocator();

/**
 * Adds definition of Kite method to class.
 * @param name Name of method.
 * @param num_parameters Number of expected input parameters (not including 'this' object).
 * @param function_pointer Pointer to a function that handles this method.
 */
#define KITE_METHOD_DEFINE(name, num_params, function_pointer) \
    class_object.add_method(#name, num_params, (void*)function_pointer)

/**
 * Adds definition of Kite operator to class.
 * @param operation The operator to override.
 * @param function_pointer Pointer to a function that handles this operator.
 */
#define KITE_OPERATOR_DEFINE(operation, function_pointer) \
    class_object.add_operator(operation, (void*)function_pointer)

/**
 * Ends class initializer definition.
 */
#define END_KITE_CLASS_INITIALIZER \
        }

/**
 * Closes definition of Kite class (base or child).
 */
#define END_KITE_CLASS \
    }

/**
 * Defines a Kite class that inherits from System::dynamic_object.
 * @param name Desired name of the class (relative to current namespace)
 */
#define BEGIN_KITE_BASE_CLASS(name) BEGIN_KITE_CHILD_CLASS(name, kite::stdlib::System::dynamic_object)

#endif