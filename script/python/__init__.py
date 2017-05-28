
from .genclass import *
from .gentest import *

__add__ = ['genClass', 'genTest']

def genClass(classpath):
    genclass.generateCppClass(classpath)

def genTest(classpath):
    gentest.generateCppTest(classpath)

