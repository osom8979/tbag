#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re


def get_children(path):
    result = []
    for parent, unused_subdirs, files in os.walk(path):
        for name in files:
            # result += [os.path.join(os.path.abspath(parent), name)]
            result += [os.path.join(parent, name)]
    return result


def get_children_with_match(path, regexp):
    result = []
    for cursor in get_children(path):
        if re.match(regexp, cursor) is not None:
            result += [cursor]
    return result


def is_empty_line(text):
    return text == '\n'


def find_semicolon(text):
    return text.find(';')


def run_migration(path, lines):
    remove_newline_re = re.compile(r'\n$')
    remove_comment_re = re.compile(r'[ \t]*//.*')
    is_macro_re = re.compile(r'^\s*#')
    is_nn_decl_re = re.compile(r'^\s*NNG?_DECL')
    sequence_point_completion = True

    result_lines = []

    for line in lines:
        if not sequence_point_completion:
            semicolon_index = find_semicolon(line)
            if semicolon_index == -1:
                result_lines.append(line)
            else:
                result_lines.append(line[:semicolon_index+1] + '*/' + line[semicolon_index+1:])
                sequence_point_completion = True
            continue

        if is_empty_line(line):
            result_lines.append('\n')
            continue

        if is_macro_re.match(line) is not None:
            result_lines.append(line)
            continue

        if is_nn_decl_re.match(line) is not None:
            semicolon_index = find_semicolon(line)
            if semicolon_index == -1:
                result_lines.append('/*' + line)
                sequence_point_completion = False
            else:
                result_lines.append('/*' + line[:semicolon_index+1] + '*/' + line[semicolon_index+1:])
            continue

        temp = remove_newline_re.sub('', line)
        temp = remove_comment_re.sub('', temp)
        if temp:
            result_lines.append(temp + '\n')

    with open(path, 'w') as f:
        f.writelines(result_lines)


def main():
    for path in get_children_with_match(os.getcwd(), r'.*\.h'):
        with open(path, 'r') as f:
            lines = f.readlines()
        run_migration(path, lines)


if __name__ == '__main__':
    main()