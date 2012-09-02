CONFIG += console
CONFIG += dll
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

SOURCES += RDTimelineView.cpp RDObjHead.cpp RDTimeRuler.cpp RDScetionView.cpp
HEADERS += RDTimelineView.h RDObjHead.h RDTimeRuler.h RDScetionView.h

#RESOURCES = resource.qrc

DEPENDENCY_SHARE_LIBRARIES = RDCommon
DEPENDENCY_SHARE_LIBRARIES += RDCommonData
DEPENDENCY_SHARE_LIBRARIES += RDObject
DEPENDENCY_SHARE_LIBRARIES += RDWidget
include("../../depends.pri")
