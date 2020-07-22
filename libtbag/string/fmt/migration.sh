#!/usr/bin/env bash

find . -name '*.h' -o -name '*.cc' | \
    xargs sed -i .tmp 's/^#include "fmt\/\(.*\)"$/#include <libtbag\/string\/fmt\/\1>/g'
find . -name '*.h' -o -name '*.cc' | \
    xargs sed -i .tmp 's/^#include "\(.*\)"$/#include <libtbag\/string\/fmt\/\1>/g'
find . -name '*.tmp' | xargs rm

echo '// Update core.h file:'
echo '# /* ==[[ MIGRATION BEGIN ]]== */'
echo '#ifndef FMT_API'
echo '#include <libtbag/predef.hpp>'
echo '#define FMT_API TBAG_API'
echo '#define FMT_EXTERN_TEMPLATE_API FMT_API'
echo '#endif'
echo '# /* ==[[ MIGRATION END ]]== */'

