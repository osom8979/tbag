#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import datetime
import getpass
import subprocess
import re

HELP_MESSAGE   = 'Usage: python ' + sys.argv[0] + ' {package/path/classname} {author}'

## Path & Directory setting.
#THIS_FILE_PATH  = os.path.abspath(main.__file__) # import __main__ as main
THIS_FILE_PATH   = os.path.realpath(__file__)
TOOLS_DIR        = os.path.dirname(THIS_FILE_PATH)
PROJECT_DIR      = os.path.normpath(TOOLS_DIR + '/..')
SOURCE_DIR       = os.path.normpath(PROJECT_DIR + '/')
TEMPLATE_DIR     = os.path.normpath(PROJECT_DIR + '/template')
HEADER_TEMPLATE  = os.path.join(TEMPLATE_DIR, 'class.hpp.in')
SOURCE_TEMPLATE  = os.path.join(TEMPLATE_DIR, 'class.cpp.in')
CONFIG_CMAKE     = os.path.join(PROJECT_DIR,  'INFORMATION')

def getProjectName():
    result = ''
    output = subprocess.check_output(['cat', CONFIG_CMAKE])

    for line in output.splitlines():
        if re.match(r'^NAME.*=.*$', line):
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
    exec "global {0}; {0} = '{0}'".format(name)

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
addGlobalString('HEADER_FILEPATH')
addGlobalString('SOURCE_FILEPATH')
addGlobalString('HEADER_BRIEF')
addGlobalString('SOURCE_BRIEF')
addGlobalString('INCLUDE_GUARD')
addGlobalString('INCLUDE_PAIR')
addGlobalString('NAMESPACE_OPEN')
addGlobalString('NAMESPACE_CLOSE')
addGlobalString('NAMESPACE_OPEN_COMMENT')
addGlobalString('NAMESPACE_CLOSE_COMMENT')
addGlobalString('NAMESPACE_BEGIN')
addGlobalString('NAMESPACE_END')

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

def getNamespaceBegin(classpath):
    names = classpath.split('/')[1:-1]
    if len(names) == 0:
        return ''
    maxlen = getMaxLength(names)
    result = ''
    for name in names:
        more_space = maxlen - len(name)
        result += "namespace {}{} {{\n".format(name, ' ' * more_space)
    return result.strip()

def getNamespaceEnd(classpath):
    names = classpath.split('/')[1:-1]
    names.reverse()
    if len(names) == 0:
        return ''
    result = ''
    for name in names:
        result += "}} // namespace {}\n".format(name)
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
        content = content.replace('${' + key + '}', dic[key])
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

    dic[NAMESPACE_BEGIN] = getNamespaceBegin(classpath)
    dic[NAMESPACE_END] = getNamespaceEnd(classpath)

    dic[HEADER_FILENAME] = dic[CLASSNAME] + dic[CPP_HEADER_EXTENSION]
    dic[SOURCE_FILENAME] = dic[CLASSNAME] + dic[CPP_SOURCE_EXTENSION]

    dic[HEADER_FILEPATH] = os.path.normpath(SOURCE_DIR + '/' +
                                            dic[CLASSPATH] +
                                            dic[CPP_HEADER_EXTENSION])
    dic[SOURCE_FILEPATH] = os.path.normpath(SOURCE_DIR + '/' +
                                            dic[CLASSPATH] +
                                            dic[CPP_SOURCE_EXTENSION])

    dic[HEADER_BRIEF] = dic[CLASSNAME] + ' class prototype.'
    dic[SOURCE_BRIEF] = dic[CLASSNAME] + ' class implementation.'

    dic[INCLUDE_GUARD] = getIncludeGuard(dic[PROJECT_NAMESPACE],
                                         dic[CLASSPATH],
                                         dic[CPP_HEADER_EXTENSION])
    dic[INCLUDE_PAIR]  = dic[CLASSPATH] + dic[CPP_HEADER_EXTENSION]

    dic[NAMESPACE_OPEN]  = 'NAMESPACE_' + dic[PROJECT_NAMESPACE].upper() + '_OPEN'
    dic[NAMESPACE_CLOSE] = 'NAMESPACE_' + dic[PROJECT_NAMESPACE].upper() + '_CLOSE'
    dic[NAMESPACE_OPEN_COMMENT]  = getDefaultFillString(dic[NAMESPACE_OPEN])
    dic[NAMESPACE_CLOSE_COMMENT] = getDefaultFillString(dic[NAMESPACE_CLOSE])
    return dic

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

