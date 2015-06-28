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

QMAKE_CXXFLAGS += -msse4

TEMPLATE = lib
DEFINES += RDCOMMON_LIBRARY
DEFINES += __GTEST__

SOURCES += HMatrixQ4F.cpp HVector4f.cpp 
SOURCES += HVector3f.cpp 
SOURCES += RDSpaceConvert.cpp
SOURCES += RDMd5.cpp
SOURCES += RDBox.cpp

HEADERS += HMatrixQ4F.h HVector4f.h  
HEADERS += HVector3f.h 
HEADERS += RDSpaceConvert.h
HEADERS += RDMd5.h
HEADERS += RDBox.h

CONFIG += dll
VERSION = 1.0.1

#LIBS += -lssl
#DESTDIR  = ../../bin
