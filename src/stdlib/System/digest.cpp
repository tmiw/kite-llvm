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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include "digest.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            static unsigned char result[MD5_DIGEST_LENGTH];
            
            // Get the size of the file by its file descriptor
            static unsigned long get_size_by_fd(int fd) 
            {
                struct stat statbuf;
                if(fstat(fd, &statbuf) < 0) exit(-1);
                return statbuf.st_size;
            }
            
            object* digest::md5(string *message)
            {
                ::MD5((unsigned char*)message->string_val.c_str(), message->string_val.size(), result);
                return new string((char*)&result);
            }
            
            object* digest::md5file(string *file)
            {
                int file_descript = open(file->string_val.c_str(), O_RDONLY);
                if(file_descript < 0) exit(-1);

                size_t file_size = get_size_by_fd(file_descript);
                
                char *file_buffer = (char*)mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
                ::MD5((unsigned char*) file_buffer, file_size, result);
                munmap(file_buffer, file_size);
                
                return new string((char*)&result);
            }
        }
    }
}
REGISTER_KITE_CLASS(kite::stdlib::System::System, kite::stdlib::System::digest)
