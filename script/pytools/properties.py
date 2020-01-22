#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import datetime
import getpass
import subprocess
import re

# Path & Directory setting.
# THIS_FILE_PATH = os.path.abspath(main.__file__)  # import __main__ as main
THIS_FILE_PATH = os.path.realpath(__file__)
SCRIPT_DIR = os.path.dirname(THIS_FILE_PATH)
PROJECT_DIR = os.path.normpath(SCRIPT_DIR + '/../..')
SOURCE_DIR = os.path.normpath(PROJECT_DIR + '/')
TEST_DIR = os.path.normpath(PROJECT_DIR + '/tester')
TEMPLATE_DIR = os.path.normpath(PROJECT_DIR + '/template')
HEADER_TEMPLATE = os.path.join(TEMPLATE_DIR, 'class.hpp.in')
SOURCE_TEMPLATE = os.path.join(TEMPLATE_DIR, 'class.cpp.in')
HEADER2_TEMPLATE = os.path.join(TEMPLATE_DIR, 'class2.hpp.in')
SOURCE2_TEMPLATE = os.path.join(TEMPLATE_DIR, 'class2.cpp.in')
FULL_HEADER_TEMPLATE = os.path.join(TEMPLATE_DIR, 'full-class.hpp.in')
FULL_SOURCE_TEMPLATE = os.path.join(TEMPLATE_DIR, 'full-class.cpp.in')
FULL_HEADER2_TEMPLATE = os.path.join(TEMPLATE_DIR, 'full-class2.hpp.in')
FULL_SOURCE2_TEMPLATE = os.path.join(TEMPLATE_DIR, 'full-class2.cpp.in')
PIMPL_HEADER_TEMPLATE = os.path.join(TEMPLATE_DIR, 'pimpl-class.hpp.in')
PIMPL_SOURCE_TEMPLATE = os.path.join(TEMPLATE_DIR, 'pimpl-class.cpp.in')
PIMPL_HEADER2_TEMPLATE = os.path.join(TEMPLATE_DIR, 'pimpl-class2.hpp.in')
PIMPL_SOURCE2_TEMPLATE = os.path.join(TEMPLATE_DIR, 'pimpl-class2.cpp.in')
TEST_TEMPLATE = os.path.join(TEMPLATE_DIR, 'test.cpp.in')
CONFIG_CMAKE = os.path.join(PROJECT_DIR, 'INFORMATION')

PROJECT_TYPE_LIB = 'lib'
PROJECT_TYPE_EXE = 'exe'


def get_project_name():
    result = ''
    output = subprocess.check_output(['cat', CONFIG_CMAKE])

    for line in output.decode('utf-8').splitlines():
        if re.match('^NAME.*=.*$', line):
            result = line[line.find('=')+1:]
            break

    if len(result) == 0:
        return 'main'  # Default project name.
    return result


# Pre-defined properties
PROJECT_NAME = get_project_name()
PROJECT_CONFIG = PROJECT_NAME + '/config.h'
HEADER_EXTENSION = '.hpp'
SOURCE_EXTENSION = '.cpp'


def add_global_variable(name):
    exec("global {0}; {0} = '{0}'".format(name))


add_global_variable('PROJECT_NAMESPACE')
add_global_variable('CONFIG_HEADER_PATH')
add_global_variable('CPP_HEADER_EXTENSION')
add_global_variable('CPP_SOURCE_EXTENSION')
add_global_variable('CLASSPATH')
add_global_variable('AUTHOR')
add_global_variable('DATE')
add_global_variable('CLASSNAME')
add_global_variable('HEADER_FILENAME')
add_global_variable('SOURCE_FILENAME')
add_global_variable('TEST_SUFFIX')
add_global_variable('TEST_FILENAME')
add_global_variable('HEADER_FILEPATH')
add_global_variable('SOURCE_FILEPATH')
add_global_variable('TEST_FILEPATH')
add_global_variable('INCLUDE_GUARD')
add_global_variable('INCLUDE_PAIR')
add_global_variable('NAMESPACE_OPEN')
add_global_variable('NAMESPACE_CLOSE')
add_global_variable('NAMESPACE_OPEN_COMMENT')
add_global_variable('NAMESPACE_CLOSE_COMMENT')
add_global_variable('CLASSNAME_IMPLEMENT_COMMENT')
add_global_variable('CLASSNAME_IMPLEMENT_HORIZON')
add_global_variable('NAMESPACE_FULL_LIST_BEGIN')
add_global_variable('NAMESPACE_FULL_LIST_END')
add_global_variable('NAMESPACE_FULL_LIST_USING')
add_global_variable('NAMESPACE_LIST_BEGIN')
add_global_variable('NAMESPACE_LIST_END')
add_global_variable('NAMESPACE_LIST_USING')
add_global_variable('ROOT_CLASSPATH')
add_global_variable('ROOT_CLASSPATH_LOWER')
add_global_variable('ROOT_CLASSPATH_UPPER')
add_global_variable('SUBPROJECT_NAME')
add_global_variable('SUBPROJECT_NAME_LOWER')
add_global_variable('SUBPROJECT_NAME_UPPER')
add_global_variable('SUBPROJECT_TYPE')
add_global_variable('EXPORT_SYMBOL')


def get_date():
    today = datetime.date.today()
    date = str(today.year) + '-'
    if today.month < 10:
        date += '0'
    date += str(today.month) + '-'
    if today.day < 10:
        date += '0'
    date += str(today.day)
    return date


def get_fill_string(text, count):
    result = ''
    for i in range(0, count):
        result += text
    return result


def get_include_guard(namespace, classpath, extension):
    return '__INCLUDE_' + namespace.upper() + '__' + \
           classpath.replace('/', '_').upper() + \
           extension.replace('.', '_').upper() + '__'


def get_max_length(names):
    maxlen = 0
    for name in names:
        if len(name) > maxlen:
            maxlen = len(name)
    return maxlen


def get_namespace_list_begin(classpath, strip=0):
    names = classpath.split('/')[strip:-1]
    if len(names) == 0:
        return ''
    maxlen = get_max_length(names)
    result = ''
    for name in names:
        more_space = maxlen - len(name)
        result += "namespace {}{} {{\n".format(name, ' ' * more_space)
    return result.strip()


def get_namespace_list_end(classpath, strip=0):
    names = classpath.split('/')[strip:-1]
    names.reverse()
    if len(names) == 0:
        return ''
    result = ''
    for name in names:
        result += "}} // namespace {}\n".format(name)
    return result.strip()


def get_namespace_list_using(classpath, strip=0):
    names = classpath.split('/')[strip:-1]
    if len(names) == 0:
        return ''
    result = ''
    append = ''
    for name in names:
        append += name
        result += "using namespace {};\n".format(append)
        append += '::'
    return result.strip()


def read_file(path):
    if os.path.isfile(path):
        with open(path, 'rb') as f:
            return f.read()
    return None


def save_file(path, content):
    if not os.path.isfile(path):
        parent = os.path.dirname(path)
        if not os.path.exists(parent):
            os.makedirs(parent)
        with open(path, 'wb') as f:
            f.write(content)


def replace_content(content, dic):
    for key in dic:
        content = content.replace(('${' + key + '}').encode(), dic[key].encode())
    return content


# noinspection PyUnresolvedReferences
def create_default_dictionary(classpath):
    dic = dict()
    dic[PROJECT_NAMESPACE] = PROJECT_NAME
    dic[CONFIG_HEADER_PATH] = PROJECT_CONFIG
    dic[CPP_HEADER_EXTENSION] = HEADER_EXTENSION
    dic[CPP_SOURCE_EXTENSION] = SOURCE_EXTENSION

    dic[CLASSPATH] = classpath
    dic[AUTHOR] = getpass.getuser()
    dic[DATE] = get_date()
    dic[CLASSNAME] = dic[CLASSPATH][dic[CLASSPATH].rfind('/') + 1:]

    dic[NAMESPACE_FULL_LIST_BEGIN] = get_namespace_list_begin(classpath)
    dic[NAMESPACE_FULL_LIST_END] = get_namespace_list_end(classpath)
    dic[NAMESPACE_FULL_LIST_USING] = get_namespace_list_using(classpath)

    dic[NAMESPACE_LIST_BEGIN] = get_namespace_list_begin(classpath, 1)
    dic[NAMESPACE_LIST_END] = get_namespace_list_end(classpath, 1)
    dic[NAMESPACE_LIST_USING] = get_namespace_list_using(classpath)

    dic[TEST_SUFFIX] = 'Test'

    dic[HEADER_FILENAME] = dic[CLASSNAME] + dic[CPP_HEADER_EXTENSION]
    dic[SOURCE_FILENAME] = dic[CLASSNAME] + dic[CPP_SOURCE_EXTENSION]
    dic[TEST_FILENAME] = dic[CLASSNAME] + dic[TEST_SUFFIX] + dic[CPP_SOURCE_EXTENSION]

    dic[HEADER_FILEPATH] = os.path.normpath(SOURCE_DIR + '/' +
                                            dic[CLASSPATH] +
                                            dic[CPP_HEADER_EXTENSION])
    dic[SOURCE_FILEPATH] = os.path.normpath(SOURCE_DIR + '/' +
                                            dic[CLASSPATH] +
                                            dic[CPP_SOURCE_EXTENSION])
    dic[TEST_FILEPATH] = os.path.normpath(TEST_DIR + '/' +
                                          dic[CLASSPATH] + dic[TEST_SUFFIX] +
                                          dic[CPP_SOURCE_EXTENSION])

    dic[INCLUDE_GUARD] = get_include_guard(dic[PROJECT_NAMESPACE],
                                           dic[CLASSPATH],
                                           dic[CPP_HEADER_EXTENSION])
    dic[INCLUDE_PAIR] = dic[CLASSPATH] + dic[CPP_HEADER_EXTENSION]

    dic[NAMESPACE_OPEN] = 'NAMESPACE_' + dic[PROJECT_NAMESPACE].upper() + '_OPEN'
    dic[NAMESPACE_CLOSE] = 'NAMESPACE_' + dic[PROJECT_NAMESPACE].upper() + '_CLOSE'
    dic[NAMESPACE_OPEN_COMMENT] = get_fill_string('-', len(dic[NAMESPACE_OPEN])-3)
    dic[NAMESPACE_CLOSE_COMMENT] = get_fill_string('-', len(dic[NAMESPACE_CLOSE])-3)

    dic[CLASSNAME_IMPLEMENT_COMMENT] = dic[CLASSNAME] + ' implementation'
    dic[CLASSNAME_IMPLEMENT_HORIZON] = get_fill_string('-', len(dic[CLASSNAME_IMPLEMENT_COMMENT]))

    dic[ROOT_CLASSPATH] = classpath.split('/')[0]
    dic[ROOT_CLASSPATH_LOWER] = dic[ROOT_CLASSPATH].lower()
    dic[ROOT_CLASSPATH_UPPER] = dic[ROOT_CLASSPATH].upper()

    if dic[ROOT_CLASSPATH_LOWER][0:3] == 'lib':
        dic[SUBPROJECT_NAME] = dic[ROOT_CLASSPATH_LOWER][3:]
        dic[SUBPROJECT_TYPE] = PROJECT_TYPE_LIB
    else:
        dic[SUBPROJECT_NAME] = dic[ROOT_CLASSPATH_LOWER]
        dic[SUBPROJECT_TYPE] = PROJECT_TYPE_EXE
    dic[SUBPROJECT_NAME_LOWER] = dic[SUBPROJECT_NAME].lower()
    dic[SUBPROJECT_NAME_UPPER] = dic[SUBPROJECT_NAME].upper()

    if dic[ROOT_CLASSPATH] == 'libtbag':
        dic[EXPORT_SYMBOL] = 'TBAG_API'
    elif dic[ROOT_CLASSPATH] == 'libtbui':
        dic[EXPORT_SYMBOL] = 'TBUI_API'
    else:
        dic[EXPORT_SYMBOL] = ''

    return dic


if __name__ == '__main__':
    pass
