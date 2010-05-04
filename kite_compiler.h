/*
 *  kite_compiler.h
 *  kite-llvm
 *
 *  Created by Mooneer Salem on 5/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef KITE_COMPILER_H
#define KITE_COMPILER_H

/*
 * Kite compiler state object
 */
typedef struct kite_compiler_t /*! Compiler state object. */
{
    char *file /*! Current file. */;
    int verMajor /*! Minimum major version. */;
    int verMinor /*! Minimum minor version. */;
    int verPatch /*! Minimum patch version. */;
    int currentLine /*! Current line number. */;
    int currentCol /*! Current column number. */;
    char *curStr /*! Current string being read. */;
} kite_compiler_t;

#endif // KITE_COMPILER_H