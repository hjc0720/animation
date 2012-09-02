#-------------------------------------------------
#
# Project created by QtCreator 2010-11-30T21:07:23
#
#-------------------------------------------------

#qt -= GUI
CONFIG += console
CONFIG += sse2 sse
#CONFIG += debug
include("../path.pri")

QMAKE_CXXFLAGS += -msse2

TEMPLATE = lib

DEFINES += RDCOMMON_LIBRARY
DEFINES += __GTEST__

SOURCES += HMatrixQ4F.cpp HVector3f.cpp HVector4f.cpp 
SOURCES += RDSpaceConvert.cpp
SOURCES += RDMd5.cpp

HEADERS += HMatrixQ4F.h HVector3f.h HVector4f.h 
HEADERS += RDSpaceConvert.h
HEADERS += RDMd5.h

CONFIG += dll
VERSION = 1.0.1

#LIBS += -lssl
#DESTDIR  = ../../bin
