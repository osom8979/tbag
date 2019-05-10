#!/usr/bin/env bash

function rename_symbol {
    echo "Rename symbol: $1 -> $2"
    find . -name '*.c' -or -name '*.h' -depth 1 | xargs sed -i .tmp -e "s/[[:<:]]$1[[:>:]]/$2/g"
}

rename_symbol Rectangle     Rectangle2
rename_symbol UnhideWindow  UnhideWindow_
rename_symbol SwapBuffers   SwapBuffers_
rename_symbol DrawText      DrawText_

echo 'Remove QueryPerformanceCounter declaration.'
sed -i .tmp -e 's/\(.*int __stdcall QueryPerformanceCounter(.*\)/\/\/\1/g' gestures.h

echo 'Remove QueryPerformanceFrequency declaration.'
sed -i .tmp -e 's/\(.*int __stdcall QueryPerformanceFrequency(.*\)/\/\/\1/g' gestures.h

echo 'Remove all tmp files.'
rm *.tmp

echo 'Done.'

