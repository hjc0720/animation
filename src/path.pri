#this file must be inculded after set config
#CONFIG += debug
#CONFIG -= release
#
#CONFIG -= debug
#CONFIG += release
QT += widgets
QMAKE_CXXFLAGS += -std=c++11 -pipe -Werror -Wall
debug{
    DESTDIR  = ../../bin_debug
    DEFINES += _DEBUG
    message("define debug")  
}
else{
    DESTDIR  = ../../bin
    message("define no debug")  
    #QMAKE_CXXFLAGS += -g
}
#CONFIG += debug
INCLUDEPATH += ../../include
