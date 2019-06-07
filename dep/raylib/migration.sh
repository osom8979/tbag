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

echo 'Rename symbol suffix: BITMAPINFOHEADER -> BITMAPINFOHEADER_'
find . -name '*.c' -or -name '*.h' | xargs sed -i .tmp -e 's/BITMAPINFOHEADER\([^_]\)/BITMAPINFOHEADER_\1/g'

rename_symbol Rectangle       Rectangle2
rename_symbol UnhideWindow    UnhideWindow_
rename_symbol SwapBuffers     SwapBuffers_
rename_symbol DrawText        DrawText_
rename_symbol DrawTextEx      DrawTextEx_
rename_symbol LoadImage       LoadImage_
rename_symbol FormatText      TextFormat
rename_symbol SubText         TextSubtext
rename_symbol ShowWindow      UnhideWindow_
rename_symbol GetCurrentTime  GetCurrentTime_
rename_symbol CloseWindow     CloseWindow_
rename_symbol ShowCursor      ShowCursor_

echo "Rename symbol: {far,near} -> {far_,near_}"
for i in raymath.h rlgl.h; do
    sed -i .tmp -e "s/[[:<:]]far[[:>:]]/far_/g" $i
    sed -i .tmp -e "s/[[:<:]]near[[:>:]]/near_/g" $i
done

echo 'Remove all tmp files.'
find . -name '*.tmp' | xargs rm

echo 'Update raylib.h file:'
echo 'RLAPI void SetEndDrawingCallback(void(*cb)(void));'

echo 'Update core.c file:'
echo 'static void (*end_drawing_callback)(void) = NULL;'
echo 'void SetEndDrawingCallback(void(*cb)(void))'
echo '{ end_drawing_callback = cb; }'

echo 'Update EndDrawing() method (core.c)'
echo 'if (end_drawing_callback) { end_drawing_callback(); }'

echo 'Done.'

