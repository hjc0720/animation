TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = RDCommon RDRenderManager RDCommonData RDObject RDDocument RDWidget RDToolEdit RDTimeline RD3rdSupport RDBase RDApp 
RDBase.depdens += RDCommon RDDocument RDObject RDWidget RDRenderManbger
RDApp.depdens += RDCommon RDBase
RDDocument.depdens = RDObject
