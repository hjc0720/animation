QT       += core gui

CONFIG += dll
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

SOURCES += RDColorWidget.cpp  RDCell.cpp
HEADERS  += RDColorWidget.h RDCell.h

DEPENDENCY_SHARE_LIBRARIES += RDCommon
include("../../depends.pri")
