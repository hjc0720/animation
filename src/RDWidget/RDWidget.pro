QT       += core gui

CONFIG += dll
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

SOURCES += RDColorWidget.cpp  RDCell.cpp RDVec3Widget.cpp
HEADERS  += RDColorWidget.h RDCell.h RDVec3Widget.h

DEPENDENCY_SHARE_LIBRARIES += RDCommon
include("../../depends.pri")
