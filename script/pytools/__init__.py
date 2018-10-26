#!/usr/bin/env python
# -*- coding: utf-8 -*-

from .genclass import *
from .genfullclass import *
from .genpimplclass import *
from .gentest import *

__add__ = ['genClass', 'genFullClass', 'genPimplClass', 'genTest']

def genClass(classpath):
    genclass.generateCppClass(classpath)

def genFullClass(classpath):
    genfullclass.generateFullCppClass(classpath)

def genPimplClass(classpath):
    genpimplclass.generatePimplCppClass(classpath)

def genTest(classpath):
    gentest.generateCppTest(classpath)

