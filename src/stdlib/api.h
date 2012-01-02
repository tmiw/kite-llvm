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

#include <ffi.h>
#include <stdarg.h>
#include "System/string.h"
#include "System/dynamic_object.h"
#include "language/kite.h"

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
        
        /**
         * Helper class to register type with Kite object system.
         * INTERNAL USE ONLY.
         */
        template<typename T>
        class ObjectRegistration
        {
        public:
            ObjectRegistration()
            {
                System::dynamic_object *root = language::kite::kite::root();
                T::class_object().properties["__name"] = new System::string(T::class_name().c_str());
                root->properties[T::class_name()] = &T::class_object();
            }

            static ObjectRegistration<T> &Get()
            {
                static ObjectRegistration<T> *registration_ptr = new ObjectRegistration<T>();
                return *registration_ptr;
            }
        };
    }
}

// Because the C preprocessor _doesn't_ do the right thing
// when __COUNTER__ and ## are involved.
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

void *api_call_method(int numargs, void *obj, void *funcptr, va_list vl);

/**
 * Defines a Kite class that inherits from a base class.
 * @param name Desired name of the class (relative to current namespace)
 * @param inherit_from Standard libary class to inherit from (which must eventually inherit System::dynamic_object)
 */
#define BEGIN_KITE_CHILD_CLASS(name, inherit_from) \
    struct name : inherit_from \
    { \
    private: \
        static void _SetAllocator(kite::stdlib::System::dynamic_object& class_obj) \
        { \
            class_obj.obj_alloc_method = ObjectAllocator<name>::GetAllocatorMethodPointer(); \
        } \
        \
    protected: \
        friend class ObjectAllocator<name>; \
        name() : inherit_from(&class_object()) { } \
        name(kite::stdlib::System::dynamic_object *parent) : inherit_from(parent) { } \
    public: \
        static name *Create(int numargs, ...) \
        { \
            name *obj = new name(); \
            std::string funcname("__init__"); \
            void *initptr = kite_find_funccall((int*)obj, funcname.c_str(), numargs + 1); \
            \
            if (initptr != NULL) \
            { \
                va_list vl; \
                va_start(vl, numargs); \
                api_call_method(numargs, (void*)obj, initptr, vl); \
            } \
            return obj; \
        } \
        static std::string &class_name() \
        { \
            static std::string full_name(#name); \
            return full_name; \
        } \
        \
        static const char *full_class_name() \
        { \
            static const char *empty_str = ""; \
            System::dynamic_object &class_obj = class_object(); \
            if (class_obj.properties.find("__name") != class_obj.properties.end()) \
            { \
                return ((kite::stdlib::System::string*)class_obj.properties["__name"])->string_val.c_str(); \
            } \
            return empty_str; \
        } \
        \
        static kite::stdlib::System::dynamic_object& class_object(bool suppress = false) \
        { \
            static kite::stdlib::System::dynamic_object *class_obj = new kite::stdlib::System::dynamic_object(); \
            if (class_obj->properties.find("__name") == class_obj->properties.end() && !suppress) \
            { \
                InitializeClass(*class_obj); \
            } \
            return *class_obj; \
        }

/**
 * Begins class initializer definition.
 */
#define BEGIN_KITE_CLASS_INITIALIZER \
    public: \
        static void InitializeClass(kite::stdlib::System::dynamic_object& class_obj) \
        { \
            _SetAllocator(class_obj);

/**
 * Adds definition of Kite method to class.
 * @param name Name of method.
 * @param num_parameters Number of expected input parameters (not including 'this' object).
 * @param function_pointer Pointer to a function that handles this method.
 */
#define KITE_METHOD_DEFINE(name, num_params, function_pointer) \
    class_obj.add_method(#name, num_params, (void*)function_pointer)

/**
 * Adds definition of Kite operator to class.
 * @param operation The operator to override.
 * @param function_pointer Pointer to a function that handles this operator.
 */
#define KITE_OPERATOR_DEFINE(operation, function_pointer) \
    class_obj.add_operator(operation, (void*)function_pointer)

/**
 * Adds definition of constructor to class.
 * @param num_params The number of parameters this constructor takes.
 * @param function_pointer Pointer to a function that initializes this object.
 */
#define KITE_CONSTRUCTOR_DEFINE(num_params, function_pointer) \
    KITE_METHOD_DEFINE(__init__, num_params, function_pointer)
    
/**
 * Ends class initializer definition.
 */
#define END_KITE_CLASS_INITIALIZER \
        }

/**
 * Closes definition of Kite class (base or child).
 */
#define END_KITE_CLASS \
        };

/**
 * Defines a Kite class that inherits from System::dynamic_object.
 * @param name Desired name of the class (relative to current namespace)
 */
#define BEGIN_KITE_BASE_CLASS(name) BEGIN_KITE_CHILD_CLASS(name, kite::stdlib::System::dynamic_object)

/**
 * Registers type with the Kite type system.
 * @param parent The parent namespace name (e.g. System).
 * @param name The current class name (e.g. string, not System::string or System.string).
 */
#define REGISTER_KITE_CLASS(parent, name) \
namespace kite { \
    namespace stdlib { \
        template<> \
        class ObjectRegistration<name> \
        { \
        public: \
            ObjectRegistration() \
            { \
                ObjectRegistration<parent> &parent_reg = ObjectRegistration<parent>::Get(); \
                std::string full_name = std::string(parent::full_class_name()) + "." + name::class_name(); \
                System::dynamic_object &class_obj = name::class_object(); \
                System::dynamic_object &parent_class_obj = parent::class_object(); \
                class_obj.properties["__name"] = new System::string(full_name.c_str()); \
                parent_class_obj.properties[name::class_name()] = &class_obj; \
            } \
            \
            static ObjectRegistration<name> &Get() \
            { \
                static ObjectRegistration<name> *registration_ptr = new ObjectRegistration<name>(); \
                return *registration_ptr; \
            } \
        }; \
        \
        ObjectRegistration<name> & TOKENPASTE2(RegistrationHelper_, __LINE__) = ObjectRegistration<name>::Get(); \
    } \
}

/**
 * Registers type with the Kite type system at the root level.
 * @param name The class' name.
 */
#define REGISTER_KITE_CLASS_AT_ROOT(name) \
    namespace kite { \
        namespace stdlib { \
            ObjectRegistration<name> & TOKENPASTE2(RegistrationHelper_, __LINE__) = ObjectRegistration<name>::Get(); \
        } \
    }

/**
 * Retrieves the integer value of an object's property.
 * @param obj The object to act upon.
 * @param prop_name The name of the property.
 */
#define KITE_GET_INTEGER_PROPERTY(obj, prop_name) ((kite::stdlib::System::integer*)obj->properties[#prop_name])->val

/**
 * Retrieves the boolean value of an object's property.
 * @param obj The object to act upon.
 * @param prop_name The name of the property.
 */
#define KITE_GET_BOOLEAN_PROPERTY(obj, prop_name) ((kite::stdlib::System::boolean*)obj->properties[#prop_name])->val

#endif
