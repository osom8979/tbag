#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from properties import *

HELP_MESSAGE = 'Usage: python ' + sys.argv[0] + ' {package/path/classname} {author}'

def generateCppTest(classpath):
    dic = createDefaultDictionary(classpath)

    if os.path.isfile(dic[TEST_FILEPATH]):
        print 'Exists header file: ' + dic[TEST_FILEPATH]
        exit(1)

    TEST_CONTENT = readFile(TEST_TEMPLATE)

    saveFile(dic[TEST_FILEPATH], replaceContent(TEST_CONTENT, dic))
    print 'Generate tester.'

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print HELP_MESSAGE
        exit(1)

    generateCppTest(sys.argv[1])
    pass

