#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from .properties import *

HELP_MESSAGE = 'Usage: python ' + sys.argv[0] + ' {package/path/classname} {author}'

def generateFullCppClass(classpath):
    dic = createDefaultDictionary(classpath)

    if os.path.isfile(dic[HEADER_FILEPATH]):
        print('Exists header file: ' + dic[HEADER_FILEPATH])
        exit(1)

    if os.path.isfile(dic[SOURCE_FILEPATH]):
        print('Exists header file: ' + dic[SOURCE_FILEPATH])
        exit(1)

    HEADER_CONTENT = readFile(FULL_HEADER_TEMPLATE)
    SOURCE_CONTENT = readFile(FULL_SOURCE_TEMPLATE)

    saveFile(dic[HEADER_FILEPATH], replaceContent(HEADER_CONTENT, dic))
    saveFile(dic[SOURCE_FILEPATH], replaceContent(SOURCE_CONTENT, dic))
    print('Generate c++ full class.')

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print(HELP_MESSAGE)
        exit(1)

    generateFullCppClass(sys.argv[1])
    pass

