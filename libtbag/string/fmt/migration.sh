#!/usr/bin/env bash

find . -name '*.h' -o -name '*.cc' | \
    xargs sed -i .tmp 's/^#include "fmt\/\(.*\)"$/#include <libtbag\/string\/fmt\/\1>/g'
find . -name '*.h' -o -name '*.cc' | \
    xargs sed -i .tmp 's/^#include "\(.*\)"$/#include <libtbag\/string\/fmt\/\1>/g'
find . -name '*.tmp' | xargs rm

echo '# /* ==[[ MIGRATION BEGIN ]]== */'
echo '#if defined(FMT_API)'
echo '#undef FMT_API'
echo '#include <libtbag/predef.hpp>'
echo '#define FMT_API TBAG_API'
echo '#endif'
echo '# /* ==[[ MIGRATION END ]]== */'

