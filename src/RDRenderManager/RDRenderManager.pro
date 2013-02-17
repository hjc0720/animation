#-------------------------------------------------
#
# Project created by QtCreator 2010-11-30T21:32:06
#
#-------------------------------------------------

CONFIG += console
CONFIG += dll
include("../path.pri")

 QT += opengl

TEMPLATE = lib
VERSION = 1.0.1

DEFINES += GL_GLEXT_PROTOTYPES

HEADERS += \
    RDTexture.h \
    RDRenderDevice.h\
    RDShaderProgram.h

SOURCES += \
    RDTexture.cpp \
    RDRenderDevice.cpp \
    RDShaderProgram.cpp

INCLUDEPATH += ../RDCommon

DEPENDENCY_SHARE_LIBRARIES = RDCommon
LIBS += -lGL
include("../../depends.pri")

