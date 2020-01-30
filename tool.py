#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""! Project tools script.
@author zer0
@date   2015-12-19
"""

import os
import sys
# noinspection PyUnresolvedReferences
import __main__ as main
from optparse import OptionParser


SCRIPT_PATH = os.path.abspath(main.__file__)
RUNTIME_DIR = os.path.dirname(SCRIPT_PATH)
SCRIPT_DIR = os.path.join(RUNTIME_DIR, 'script')
sys.path.insert(0, SCRIPT_DIR)

# noinspection PyUnresolvedReferences
import pytools as tools


TEMPLATE_DIR = os.path.join(RUNTIME_DIR, 'template')

PROJECT_CMAKE_NAME = 'project.cmake'
TEMPLATE_EXTENSION = '.in'
PROJECT_CMAKE_TEMPLATE_NAME = PROJECT_CMAKE_NAME + TEMPLATE_EXTENSION
PROJECT_CMAKE_TEMPLATE = os.path.join(TEMPLATE_DIR, PROJECT_CMAKE_TEMPLATE_NAME)
CONFIG_CMAKE_PATH = 'config.cmake'
CONFIG_COMMAND = 'cmake -L -P' + CONFIG_CMAKE_PATH

CMD_HELP = 'help'
CMD_CLASS = 'class'
CMD_FULL = 'full'
CMD_PIMPL = 'pimpl'
CMD_TEST = 'test'
CMD_TEXT = 'text'
CMD_UUID = 'uuid'

CMD_MAP = {
    CMD_HELP: 'Show this help message and exit.',
    CMD_CLASS: 'Generate default c++ source/header files.',
    CMD_FULL: 'Generate default c++ full-class source/header files.',
    CMD_PIMPL: 'Generate default c++ pimpl-class source/header files.',
    CMD_TEST: 'Generate default gtest tester file.',
    CMD_TEXT: 'Generate text id header.',
    CMD_UUID: 'Make a random UUID.',
}

CMD_MESSAGE = "\nCommand list:\n"
for key, val in CMD_MAP.items():
    CMD_MESSAGE += '{}\t{}\n'.format(key, val)

ENABLE_DEFAULT_HELP_MESSAGE = False


def print_command_help():
    print(CMD_MESSAGE)


def get_environment_path():
    return SCRIPT_PATH


def parse_arguments(argv, print_help=True):
    command = None

    if len(argv) >= 2:
        arg1 = argv[1]
        for cursor in CMD_MAP.keys():
            if cursor == arg1:
                command = cursor
                argv.pop(1)
                break

    parser = OptionParser(add_help_option=ENABLE_DEFAULT_HELP_MESSAGE)
    parser.set_usage('Usage: ' + sys.argv[0] + ' {COMMAND} {OPTIONS}')
    parser.add_option('-c', '--config-path',
                      dest='config_path',
                      metavar='PATH',
                      default=CONFIG_CMAKE_PATH,
                      help='Set the configure file path.')
    options, args = parser.parse_args(argv)

    if print_help is True and (command is None or command == CMD_HELP):
        parser.print_help()
        print_command_help()

    return command, options


def main_class():
    if len(sys.argv) <= 1:
        print("Usage: python {} {} {{package/path/classname}}".format(sys.argv[0], CMD_CLASS))
        exit(1)
    tools.gen_class(sys.argv[1])


def main_full():
    if len(sys.argv) <= 1:
        print("Usage: python {} {} {{package/path/classname}}".format(sys.argv[0], CMD_FULL))
        exit(1)
    tools.gen_full_class(sys.argv[1])


def main_pimpl():
    if len(sys.argv) <= 1:
        print("Usage: python {} {} {{package/path/classname}}".format(sys.argv[0], CMD_PIMPL))
        exit(1)
    tools.gen_pimpl_class(sys.argv[1])


def main_test():
    if len(sys.argv) <= 1:
        print("Usage: python {} {} {{package/path/classname}}".format(sys.argv[0], CMD_TEST))
        exit(1)
    tools.gen_test(sys.argv[1])


def main_text():
    if len(sys.argv) <= 2:
        print("Usage: python {} {} {{input_xml}} {{output_header}}".format(sys.argv[0], CMD_TEXT))
        exit(1)
    tools.gen_test(sys.argv[1], sys.argv[2])


def main_uuid():
    import uuid
    print(uuid.uuid4())


def main():
    command, _ = parse_arguments(sys.argv)
    if command is None or command == CMD_HELP:
        return
    elif command == CMD_CLASS:
        main_class()
    elif command == CMD_FULL:
        main_full()
    elif command == CMD_PIMPL:
        main_pimpl()
    elif command == CMD_TEST:
        main_test()
    elif command == CMD_TEXT:
        main_text()
    elif command == CMD_UUID:
        main_uuid()


if __name__ == '__main__':
    main()
