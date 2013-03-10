#-------------------------------------------------
#
# Project created by QtCreator 2010-11-30T21:32:06
#
#-------------------------------------------------

CONFIG += console
CONFIG += dll
QT += opengl
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

INCLUDEPATH += ../RDCommon

SOURCES += RDObject.cpp RDScene.cpp RDRenderData.cpp RDSceneRenderData.cpp RDNode.cpp RDCreateObj.cpp
SOURCES += RDImageObject.cpp 
SOURCES += RDRenderManager.cpp
SOURCES += RDLayer.cpp
HEADERS  += RDObject.h RDScene.h RDRenderData.h RDSceneRenderData.h RDNode.h RDCreateObj.h
HEADERS += RDImageObject.h
HEADERS  += RDRenderManager.h
SOURCES += RDLayer.h


DEPENDENCY_SHARE_LIBRARIES = RDCommon
DEPENDENCY_SHARE_LIBRARIES += RDCommonData
DEPENDENCY_SHARE_LIBRARIES += RDRenderManager
include("../../depends.pri")
