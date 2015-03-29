TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src
#SUBDIRS += test
QT += widgets
test.depends += RDCommon
