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
    m_nStoryLength = 1000000000LL;
}

void RDStory::Serialize(RDJsonDataStream& buffer, Json::Value &parent)
{
    buffer.Serialize(parent,"pass",m_bPass,false);
    buffer.Serialize(parent,"start",m_nPlayStartFrame,0LL);
    buffer.Serialize(parent,"length",m_nStoryLength,1000000000LL);
    buffer.Serialize(parent,"id",m_Id ,QUuid::createUuid());
    buffer.Serialize(parent,"name",m_strName,std::string());
}
