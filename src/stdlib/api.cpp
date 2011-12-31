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
 
#include <stdarg.h>
#include <ffi/ffi.h>
#include "api.h"

void *api_call_method(int numargs, void *obj, void *initptr, va_list vl)
{
    ffi_cif cif;
    ffi_type *args[numargs + 1];
    void *values[numargs + 1];
    void **val_heap = (void**)malloc((numargs + 1) * sizeof(void*)); /*new void*[numargs + 1];*/
    void *rv;
    args[0] = &ffi_type_pointer;
    values[0] = &val_heap[0];
    for (int i = 0; i < numargs; i++)
    {
        args[i + 1] = &ffi_type_pointer;
        values[i + 1] = (void*)&val_heap[i + 1];
    }
    assert(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, numargs + 1, &ffi_type_pointer, args) == FFI_OK);
    val_heap[0] = (void*)obj;
    for (int i = 0; i < numargs; i++)
    {
        val_heap[i + 1] = va_arg(vl, void*);
    }
    ffi_call(&cif, (void(*)())initptr, &rv, values);
    va_end(vl);
    free(val_heap);
    /*delete[] val_heap;*/
    return rv;
}