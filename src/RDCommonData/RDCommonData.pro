#this project implement the common function, but it may use the qt function,not as RDCommon projcet which does not depend on qt.
qt -= GUI
CONFIG += console
CONFIG += sse2 sse
CONFIG += dll
include("../path.pri")

TEMPLATE = lib
VERSION = 1.0.1

SOURCES = RDResourceManager.cpp RDResource.cpp RDBuffer.cpp RDSwapChain.cpp
SOURCES += RDImageResource.cpp RDFileDataStream.cpp 
SOURCES += RDSection.cpp RDBaseKey.cpp RDStory.cpp RDKeyList.cpp

HEADERS = RDResourceManager.h RDResource.h RDBuffer.h RDSwapChain.h
HEADERS += RDImageResource.h RDFileDataStream.h 
HEADERS += RDSection.h RDBaseKey.h RDStory.h RDKeyList.h

DEPENDENCY_SHARE_LIBRARIES = RDCommon
include("../../depends.pri")

