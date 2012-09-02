CONFIG += qt console
CONFIG += debug

TEMPLATE = app
TARGET = appTest

DEFINES += RDCOMMON_LIBRARY
DEFINES += __GTEST__

SOURCES += HMatrixTest.cpp HVector4fTest.cpp

HEADERS += HMatrixTest.h HVector4fTest.h

VERSION = 1.0.2

INCLUDEPATH += ../include
INCLUDEPATH += ../src/RDCommon/

#LIBS += -L../bin -lRDCommon
#LIBS += -L../lib -lgtest_main
#LIBS += -L../lib -lgtest

debug{
    DESTDIR  = ../bin_debug
}
else{
    DESTDIR  = ../bin
}
STATIC_LIBS_PATH = ../lib

DEPENDENCY_SHARE_LIBRARIES = RDCommon
DEPENDENCY_STATIC_LIBRARIES = gtest_main
include("../depends.pri")
