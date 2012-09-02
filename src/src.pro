TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = RDCommon RDCommonData RDObject RDDocument RDWidget RDToolEdit RDTimeline RD3rdSupport RDBase RDApp 
RDBase.depdens += RDCommon RDDocument RDObject RDWidget
RDApp.depdens += RDCommon RDBase
RDDocument.depdens = RDObject
