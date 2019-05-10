#!/usr/bin/env bash

function rename_symbol {
    echo "Rename symbol: $1 -> $2"
    find . -name '*.c' -or -name '*.h' -depth 1 | xargs sed -i .tmp -e "s/[[:<:]]$1[[:>:]]/$2/g"
}

echo 'Remove QueryPerformanceCounter declaration.'
sed -i .tmp -e 's/^    \(int __stdcall QueryPerformanceCounter(.*\)/    \/\/\1/g' gestures.h

echo 'Remove QueryPerformanceFrequency declaration.'
sed -i .tmp -e 's/^    \(int __stdcall QueryPerformanceFrequency(.*\)/    \/\/\1/g' gestures.h

echo 'Remove FormatText macro.'
sed -i .tmp -e 's/\(#define FormatText.*\)/\/\/\1/g' raylib.h

echo 'Remove SubText macro.'
sed -i .tmp -e 's/\(#define SubText.*\)/\/\/\1/g' raylib.h

echo 'Remove ShowWindow macro.'
sed -i .tmp -e 's/\(#define ShowWindow.*\)/\/\/\1/g' raylib.h

rename_symbol Rectangle       Rectangle2
rename_symbol UnhideWindow    UnhideWindow_
rename_symbol SwapBuffers     SwapBuffers_
rename_symbol DrawText        DrawText_
rename_symbol FormatText      TextFormat
rename_symbol SubText         TextSubtext
rename_symbol ShowWindow      UnhideWindow_
rename_symbol GetCurrentTime  GetCurrentTime_

echo "Rename symbol (raymath.h): far -> far_"
sed -i .tmp -e "s/[[:<:]]far[[:>:]]/far_/g" raymath.h

echo "Rename symbol (raymath.h): near -> near_"
sed -i .tmp -e "s/[[:<:]]near[[:>:]]/near_/g" raymath.h

echo 'Remove all tmp files.'
rm *.tmp

echo 'Done.'

