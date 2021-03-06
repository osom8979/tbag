#!/usr/bin/env python
# -*- coding: utf-8 -*-

from .genclass import *
from .genfullclass import *
from .genpimplclass import *
from .gentest import *
from .gentext import *

__add__ = ['gen_class', 'gen_full_class', 'gen_pimpl_class', 'gen_test', 'generate_text_id_header']


def gen_class(classpath):
    genclass.generate_cpp_class(classpath)


def gen_full_class(classpath):
    genfullclass.generate_full_cpp_class(classpath)


def gen_pimpl_class(classpath):
    genpimplclass.generate_pimpl_cpp_class(classpath)


def gen_test(classpath):
    gentest.generate_cpp_test(classpath)


def generate_text_id_header(input_xml, output_header):
    gentext.generate_text_id_header(input_xml, output_header)
