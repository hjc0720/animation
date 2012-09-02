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

RDObject::RDObject(RDObjectType nType)
    :m_nObjectVersion(g_nObjVersion)
     ,m_ObjID(QUuid::createUuid())
{
	m_nType = nType;
}

void RDObject::Lock()const
{
    m_pParent->Lock();
}
void RDObject::UnLock()const
{
    m_pParent->UnLock();
}

void RDObject::Save(RDFileDataStream& buffer)
{
    buffer << m_nObjectVersion;
    buffer << m_ObjID;
}
void RDObject::Load(RDFileDataStream& buffer)
{
    buffer >> m_nObjectVersion;
    buffer >> m_ObjID;
    m_nObjectVersion = g_nObjVersion;
}
