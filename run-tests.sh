#!/bin/sh

num_pass=0
num_fail=0

for i in `find ./tests -name '*.kt' -print`; do
    OUT_EXTENSION=".out"
    OUT_POSTFIX=":"
    echo -n "$i$OUT_POSTFIX "
    ./kite $i | diff - $i$OUT_EXTENSION >/dev/null 2>&1
    if [ $? != 0 ]; then 
        num_fail=$((num_fail + 1))
        echo FAIL
    else
        num_pass=$((num_pass + 1))
        echo pass
    fi
done;

echo "$num_pass passed, $num_fail failed."
if [ $num_fail -gt 0 ]; then exit 1
fi
