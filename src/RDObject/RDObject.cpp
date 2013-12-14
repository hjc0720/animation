// =====================================================================================
// 
//       Filename:  RDObject.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/03/11 21:47:31
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDObject.h"
#include "RDFileDataStream.h"
#include "RDNode.h"

const int g_nObjVersion = 0;

RDObject::RDObject()
     :m_ObjID(QUuid::createUuid())
{
}

void RDObject::Lock()const
{
    m_pParent->Lock();
}
void RDObject::UnLock()const
{
    m_pParent->UnLock();
}

void RDObject::Serialize(RDFileDataStream& buffer,bool bSave)
{
    int nVersion = g_nObjVersion;
    buffer.Serialize(nVersion,bSave);
    buffer.Serialize(m_ObjID,bSave);
}
