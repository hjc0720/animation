// =====================================================================================
// 
//       Filename:  RDStory.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/08/11 21:48:26
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDStory.h"
#include "RDFileDataStream.h"
#include <QDebug>

RDStory::RDStory(const std::string& name)
    :m_Id(QUuid::createUuid())
     ,m_strName(name)
{
    m_bPass = false; 
    m_nPlayStartFrame = 0;
	m_nStoryLength = 1000000000;
}

void RDStory::Serialize(RDFileDataStream& buffer,bool bSave)
{
    buffer.Serialize(m_bPass,bSave);
    buffer.Serialize(m_nPlayStartFrame,bSave);
    buffer.Serialize(m_nStoryLength,bSave);
    buffer.Serialize(m_Id,bSave);
    buffer.Serialize(m_strName,bSave);
}
