#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from properties import *

HELP_MESSAGE = 'Usage: python ' + sys.argv[0] + ' {package/path/classname} {author}'

def generateCppClass(classpath):
    dic = createDefaultDictionary(classpath)

    if os.path.isfile(dic[HEADER_FILEPATH]):
        print 'Exists header file: ' + dic[HEADER_FILEPATH]
        exit(1)

    if os.path.isfile(dic[SOURCE_FILEPATH]):
        print 'Exists header file: ' + dic[SOURCE_FILEPATH]
        exit(1)

    HEADER_CONTENT = readFile(HEADER_TEMPLATE)
    SOURCE_CONTENT = readFile(SOURCE_TEMPLATE)

    saveFile(dic[HEADER_FILEPATH], replaceContent(HEADER_CONTENT, dic))
    saveFile(dic[SOURCE_FILEPATH], replaceContent(SOURCE_CONTENT, dic))
    print 'Generate c++ class.'

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print HELP_MESSAGE
        exit(1)

    generateCppClass(sys.argv[1])
    pass

