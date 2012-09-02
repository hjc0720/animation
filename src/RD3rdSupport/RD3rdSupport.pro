CONFIG += qt
CONFIG += dll
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

INCLUDEPATH += ../RDCommon

SOURCES += RDTimer.cpp RDThread.cpp
HEADERS  += RDTimer.h RDThread.h


DEPENDENCY_SHARE_LIBRARIES = RDCommon
include("../../depends.pri")
