#!/bin/sh

for i in `find src -name '*.cpp'`; do
    echo "Compiling $i..."
    # -DBOOST_SPIRIT_DEBUG 
    g++ -c -g -Os -Isrc/ -I/opt/local/include `llvm-config --cppflags` $i
done

echo "Linking..."
g++ -o kite -g -Os -Isrc/ -I/opt/local/include `llvm-config --ldflags` `llvm-config --libs` *.o
