#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from .properties import *

HELP_MESSAGE = 'Usage: python ' + sys.argv[0] + ' {package/path/classname}'


# noinspection PyUnresolvedReferences
def generate_cpp_test(classpath):
    dic = create_default_dictionary(classpath)

    if os.path.isfile(dic[TEST_FILEPATH]):
        print('Exists header file: ' + dic[TEST_FILEPATH])
        exit(1)

    test_content = read_file(TEST_TEMPLATE)

    save_file(dic[TEST_FILEPATH], replace_content(test_content, dic))
    print('Generate tester.')


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print(HELP_MESSAGE)
        exit(1)
    generate_cpp_test(sys.argv[1])
