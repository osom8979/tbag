#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from .properties import *

HELP_MESSAGE = 'Usage: python ' + sys.argv[0] + ' {package/path/classname}'


# noinspection PyUnresolvedReferences
def generate_cpp_class(classpath):
    dic = create_default_dictionary(classpath)

    if os.path.isfile(dic[HEADER_FILEPATH]):
        print('Exists header file: ' + dic[HEADER_FILEPATH])
        exit(1)

    if os.path.isfile(dic[SOURCE_FILEPATH]):
        print('Exists header file: ' + dic[SOURCE_FILEPATH])
        exit(1)

    if dic[ROOT_CLASSPATH] == 'libtbag':
        header_content = read_file(HEADER_TEMPLATE)
        source_content = read_file(SOURCE_TEMPLATE)
    else:
        header_content = read_file(HEADER2_TEMPLATE)
        source_content = read_file(SOURCE2_TEMPLATE)

    save_file(dic[HEADER_FILEPATH], replace_content(header_content, dic))
    save_file(dic[SOURCE_FILEPATH], replace_content(source_content, dic))
    print('Generate c++ class.')


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print(HELP_MESSAGE)
        exit(1)
    generate_cpp_class(sys.argv[1])
