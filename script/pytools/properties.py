#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import datetime
import getpass
import subprocess
import re

## Path & Directory setting.
#THIS_FILE_PATH      = os.path.abspath(main.__file__) # import __main__ as main
THIS_FILE_PATH       = os.path.realpath(__file__)
SCRIPT_DIR           = os.path.dirname(THIS_FILE_PATH)
PROJECT_DIR          = os.path.normpath(SCRIPT_DIR + '/../..')
SOURCE_DIR           = os.path.normpath(PROJECT_DIR + '/')
TEST_DIR             = os.path.normpath(PROJECT_DIR + '/tester')
TEMPLATE_DIR         = os.path.normpath(PROJECT_DIR + '/template')
HEADER_TEMPLATE      = os.path.join(TEMPLATE_DIR, 'class.hpp.in')
SOURCE_TEMPLATE      = os.path.join(TEMPLATE_DIR, 'class.cpp.in')
FULL_HEADER_TEMPLATE = os.path.join(TEMPLATE_DIR, 'full-class.hpp.in')
FULL_SOURCE_TEMPLATE = os.path.join(TEMPLATE_DIR, 'full-class.cpp.in')
TEST_TEMPLATE        = os.path.join(TEMPLATE_DIR, 'test.cpp.in')
CONFIG_CMAKE         = os.path.join(PROJECT_DIR,  'INFORMATION')

PROJECT_TYPE_LIB = 'lib'
PROJECT_TYPE_EXE = 'exe'

def getProjectName():
    result = ''
    output = subprocess.check_output(['cat', CONFIG_CMAKE])

    for line in output.decode('utf-8').splitlines():
        if re.match('^NAME.*=.*$', line):
            result = line[line.find('=')+1:]
            break

    if len(result) == 0:
        return 'main' # Default project name.
    return result

## Pre-defined properties:
PROJECT_NAME     = getProjectName()
PROJECT_CONFIG   = PROJECT_NAME + '/config.h'
HEADER_EXTENSION = '.hpp'
SOURCE_EXTENSION = '.cpp'

def addGlobalString(name):
    exec("global {0}; {0} = '{0}'".format(name))

addGlobalString('PROJECT_NAMESPACE')
addGlobalString('CONFIG_HEADER_PATH')
addGlobalString('CPP_HEADER_EXTENSION')
addGlobalString('CPP_SOURCE_EXTENSION')
addGlobalString('CLASSPATH')
addGlobalString('AUTHOR')
addGlobalString('DATE')
addGlobalString('CLASSNAME')
addGlobalString('HEADER_FILENAME')
addGlobalString('SOURCE_FILENAME')
addGlobalString('TEST_SUFFIX')
addGlobalString('TEST_FILENAME')
addGlobalString('HEADER_FILEPATH')
addGlobalString('SOURCE_FILEPATH')
addGlobalString('TEST_FILEPATH')
addGlobalString('INCLUDE_GUARD')
addGlobalString('INCLUDE_PAIR')
addGlobalString('NAMESPACE_OPEN')
addGlobalString('NAMESPACE_CLOSE')
addGlobalString('NAMESPACE_OPEN_COMMENT')
addGlobalString('NAMESPACE_CLOSE_COMMENT')
addGlobalString('NAMESPACE_LIST_BEGIN')
addGlobalString('NAMESPACE_LIST_END')
addGlobalString('NAMESPACE_LIST_USING')
addGlobalString('ROOT_CLASSPATH')
addGlobalString('ROOT_CLASSPATH_LOWER')
addGlobalString('ROOT_CLASSPATH_UPPER')
addGlobalString('SUBPROJECT_NAME')
addGlobalString('SUBPROJECT_NAME_LOWER')
addGlobalString('SUBPROJECT_NAME_UPPER')
addGlobalString('SUBPROJECT_TYPE')

def getDate():
    today = datetime.date.today()
    date = str(today.year) + '-'
    if today.month < 10:
        date += '0'
    date += str(today.month) + '-'
    if today.day < 10:
        date += '0'
    date += str(today.day)
    return date

def getFillString(str, count):
    result = ''
    for i in range(0, count):
        result += str
    return result

def getDefaultFillString(basestr):
    return getFillString('-', len(basestr) - 3)

def getIncludeGuard(namespace, classpath, extension):
    return '__INCLUDE_' + namespace.upper() + '__' + \
        classpath.replace('/', '_').upper() + \
        extension.replace('.', '_').upper() + '__'

def getMaxLength(names):
    maxlen = 0
    for name in names:
        if len(name) > maxlen:
            maxlen = len(name)
    return maxlen

def getNamespaceListBegin(classpath):
    names = classpath.split('/')[1:-1]
    if len(names) == 0:
        return ''
    maxlen = getMaxLength(names)
    result = ''
    for name in names:
        more_space = maxlen - len(name)
        result += "namespace {}{} {{\n".format(name, ' ' * more_space)
    return result.strip()

def getNamespaceListEnd(classpath):
    names = classpath.split('/')[1:-1]
    names.reverse()
    if len(names) == 0:
        return ''
    result = ''
    for name in names:
        result += "}} // namespace {}\n".format(name)
    return result.strip()

def getNamespaceListUsing(classpath):
    names = classpath.split('/')[0:-1]
    if len(names) == 0:
        return ''
    result = ''
    append = ''
    for name in names:
        append += name
        result += "using namespace {};\n".format(append)
        append += '::'
    return result.strip()

def readFile(path):
    if os.path.isfile(path):
        with open(path, 'rb') as f:
            return f.read()
    return None

def saveFile(path, content):
    if not os.path.isfile(path):
        parent = os.path.dirname(path)
        if not os.path.exists(parent):
            os.makedirs(parent)
        with open(path, 'wb') as f:
            f.write(content)

def replaceContent(content, dic):
    for key in dic:
        content = content.replace(('${' + key + '}').encode(), dic[key].encode())
    return content

def createDefaultDictionary(classpath):
    dic = {}
    dic[PROJECT_NAMESPACE]    = PROJECT_NAME
    dic[CONFIG_HEADER_PATH]   = PROJECT_CONFIG
    dic[CPP_HEADER_EXTENSION] = HEADER_EXTENSION
    dic[CPP_SOURCE_EXTENSION] = SOURCE_EXTENSION

    dic[CLASSPATH] = classpath
    dic[AUTHOR]    = getpass.getuser()
    dic[DATE]      = getDate()
    dic[CLASSNAME] = dic[CLASSPATH][dic[CLASSPATH].rfind('/') + 1:]

    dic[NAMESPACE_LIST_BEGIN] = getNamespaceListBegin(classpath)
    dic[NAMESPACE_LIST_END]   = getNamespaceListEnd(classpath)
    dic[NAMESPACE_LIST_USING] = getNamespaceListUsing(classpath)

    dic[TEST_SUFFIX] = 'Test'

    dic[HEADER_FILENAME] = dic[CLASSNAME] + dic[CPP_HEADER_EXTENSION]
    dic[SOURCE_FILENAME] = dic[CLASSNAME] + dic[CPP_SOURCE_EXTENSION]
    dic[TEST_FILENAME]   = dic[CLASSNAME] + dic[TEST_SUFFIX] + dic[CPP_SOURCE_EXTENSION]

    dic[HEADER_FILEPATH] = os.path.normpath(SOURCE_DIR + '/' +
                                            dic[CLASSPATH] +
                                            dic[CPP_HEADER_EXTENSION])
    dic[SOURCE_FILEPATH] = os.path.normpath(SOURCE_DIR + '/' +
                                            dic[CLASSPATH] +
                                            dic[CPP_SOURCE_EXTENSION])
    dic[TEST_FILEPATH]   = os.path.normpath(TEST_DIR + '/' +
                                            dic[CLASSPATH] + dic[TEST_SUFFIX] +
                                            dic[CPP_SOURCE_EXTENSION])

    dic[INCLUDE_GUARD] = getIncludeGuard(dic[PROJECT_NAMESPACE],
                                         dic[CLASSPATH],
                                         dic[CPP_HEADER_EXTENSION])
    dic[INCLUDE_PAIR]  = dic[CLASSPATH] + dic[CPP_HEADER_EXTENSION]

    dic[NAMESPACE_OPEN]  = 'NAMESPACE_' + dic[PROJECT_NAMESPACE].upper() + '_OPEN'
    dic[NAMESPACE_CLOSE] = 'NAMESPACE_' + dic[PROJECT_NAMESPACE].upper() + '_CLOSE'
    dic[NAMESPACE_OPEN_COMMENT]  = getDefaultFillString(dic[NAMESPACE_OPEN])
    dic[NAMESPACE_CLOSE_COMMENT] = getDefaultFillString(dic[NAMESPACE_CLOSE])

    dic[ROOT_CLASSPATH] = classpath.split('/')[0]
    dic[ROOT_CLASSPATH_LOWER] = dic[ROOT_CLASSPATH].lower()
    dic[ROOT_CLASSPATH_UPPER] = dic[ROOT_CLASSPATH].upper()

    if dic[ROOT_CLASSPATH_LOWER][0:3] == 'lib':
        dic[SUBPROJECT_NAME] = dic[ROOT_CLASSPATH_LOWER][3:]
        dif[SUBPROJECT_TYPE] = PROJECT_TYPE_LIB
    else:
        dic[SUBPROJECT_NAME] = dic[ROOT_CLASSPATH_LOWER]
        dif[SUBPROJECT_TYPE] = PROJECT_TYPE_EXE
    dic[SUBPROJECT_NAME_LOWER] = dic[SUBPROJECT_NAME].lower()
    dic[SUBPROJECT_NAME_UPPER] = dic[SUBPROJECT_NAME].upper()
    return dic

if __name__ == '__main__':
    pass

