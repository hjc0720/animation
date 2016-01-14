CONFIG += console
CONFIG += dll
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

SOURCES += RDTimelineView.cpp RDObjHead.cpp RDScetionView.cpp RDSectionItem.cpp \
    rdkeyitem.cpp
SOURCES += RDStoryItem.cpp RDTimeMarker.cpp
SOURCES += RDSectionScene.cpp 

HEADERS += RDTimelineView.h RDObjHead.h RDScetionView.h RDSectionItem.h \
    rdkeyitem.h
HEADERS += RDStoryItem.h RDTimeMarker.h
HEADERS += RDSectionScene.h 

#RESOURCES = resource.qrc

DEPENDENCY_SHARE_LIBRARIES = RDCommon
DEPENDENCY_SHARE_LIBRARIES += RDCommonData
DEPENDENCY_SHARE_LIBRARIES += RDObject
DEPENDENCY_SHARE_LIBRARIES += RDWidget
include("../../depends.pri")
