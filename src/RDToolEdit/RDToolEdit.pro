CONFIG += console
CONFIG += dll
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

SOURCES += RDToolManager.cpp RDBaseToolEdit.cpp RDBaseTool.cpp RDGroupTool.cpp RDEditerManager.cpp RDBaseEdit.cpp
SOURCES += RDImageTool.cpp RDSelectTool.cpp
SOURCES += RDImageEdit.cpp
SOURCES += RDSpaceCell.cpp
HEADERS += RDToolManager.h RDBaseToolEdit.h RDBaseTool.h RDGroupTool.h RDEditerManager.h RDBaseEdit.h
HEADERS += RDImageTool.h RDSelectTool.h
HEADERS += RDImageEdit.h
HEADERS += RDSpaceCell.h

#RESOURCES = resource.qrc

DEPENDENCY_SHARE_LIBRARIES = RDCommon
DEPENDENCY_SHARE_LIBRARIES += RDCommonData
DEPENDENCY_SHARE_LIBRARIES += RDObject
DEPENDENCY_SHARE_LIBRARIES += RDDocument
DEPENDENCY_SHARE_LIBRARIES += RDWidget
include("../../depends.pri")
