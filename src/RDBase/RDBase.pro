#-------------------------------------------------
#
# Project created by hjc0720@gmail.com 
# Sun May 15 10:55:40 CST 2011
#
#-------------------------------------------------

QT       += core gui
 QT += opengl

CONFIG += dll
CONFIG += sse2 sse
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

SOURCES += mainwindow.cpp RDRenderWidget.cpp RDToolBar.cpp
SOURCES += RDSetSceneDlg.cpp RDPropertySheet.cpp
HEADERS  += mainwindow.h RDRenderWidget.h RDToolBar.h
HEADERS  += RDSetSceneDlg.h RDPropertySheet.h

DEPENDENCY_SHARE_LIBRARIES = RDCommon
DEPENDENCY_SHARE_LIBRARIES += RDRenderManager
DEPENDENCY_SHARE_LIBRARIES += RDCommonData
DEPENDENCY_SHARE_LIBRARIES += RDDocument
DEPENDENCY_SHARE_LIBRARIES += RDObject
DEPENDENCY_SHARE_LIBRARIES += RDToolEdit
DEPENDENCY_SHARE_LIBRARIES += RD3rdSupport
DEPENDENCY_SHARE_LIBRARIES += RDWidget
DEPENDENCY_SHARE_LIBRARIES += RDTimeline
include("../../depends.pri")

TRANSLATIONS = RDBase_zh.ts
