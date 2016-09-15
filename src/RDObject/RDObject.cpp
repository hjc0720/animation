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

RDObject::RDObject()
     :m_ObjID(QUuid::createUuid())
     ,m_pParent(nullptr)
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

void RDObject::Serialize(RDJsonDataStream& buffer, Json::Value &parent)
{
    buffer.Serialize(parent,"id",m_ObjID,QUuid::createUuid());
}
