#!/usr/bin/env bash

find . -name '*.c' -or -name '*.h' -depth 1 | xargs sed -i .tmp -e 's/[[:<:]]Rectangle[[:>:]]/Rectangle2/g'
rm *.tmp

