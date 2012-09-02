#-------------------------------------------------
#
# Project created by QtCreator 2010-11-30T21:32:06
#
#-------------------------------------------------

CONFIG += console
CONFIG += dll
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

INCLUDEPATH += ../RDCommon
INCLUDEPATH += ../RDObject

SOURCES += RDDocument.cpp RDProject.cpp 
HEADERS  += RDDocument.h RDProject.h 

#LIBS += -L../../bin -lRDCommon
DEPENDENCY_SHARE_LIBRARIES = RDCommon
DEPENDENCY_SHARE_LIBRARIES += RDObject
DEPENDENCY_SHARE_LIBRARIES += RDCommonData
include("../../depends.pri")
