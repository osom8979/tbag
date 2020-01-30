#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import re
from xml.etree import ElementTree as ET

HELP_MESSAGE = 'Usage: python ' + sys.argv[0] + ' {input_xml} {output_header}'
ROOT_TAG = 'resource'
TEXT_TAG = 'text'
NAME_ATTR = 'name'

HEADER_PREFIX = """
#ifndef __INCLUDE__AUTO_GEN__TEXT_ID_H__
#define __INCLUDE__AUTO_GEN__TEXT_ID_H__

"""
HEADER_SUFFIX = '\n#endif\n'
DEFINE_PREFIX = 'TEXT_ID_'


# noinspection PyUnresolvedReferences
def generate_text_id_header(input_xml, output_header):
    doc = ET.parse(input_xml)
    root = doc.getroot()

    # check the root tag.
    if root.tag != ROOT_TAG:
        return False

    with open(output_header, 'w') as f:
        f.write(HEADER_PREFIX)
        for elem in root.findall(TEXT_TAG):
            original = elem.attrib[NAME_ATTR]
            name = re.sub(r'[^a-zA-Z0-9_]', '_', original).upper()
            text = re.sub(r'"', '\\"', original)
            f.write('#define {}{} "{}"\n'.format(DEFINE_PREFIX, name, text))
        f.write(HEADER_SUFFIX)

    print('Generate text id header.')


if __name__ == '__main__':
    if len(sys.argv) <= 2:
        print(HELP_MESSAGE)
        exit(1)
    generate_text_id_header(sys.argv[1], sys.argv[2])
